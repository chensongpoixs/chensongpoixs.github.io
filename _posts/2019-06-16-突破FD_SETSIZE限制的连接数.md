---
layout:     post
title:      突破FD_SETSIZE限制的连接数
subtitle:   boost库的asio的中fd_set的适配器的设计
date:       2019-06-16
times:      23::21::30
author:     chensong
header-img: img/2019-01-02/bg_moudle.jpg
catalog: 	 true
tags:
    - 网络编程
    - boost库asio源码探秘
    - 设计模式
---

## 前言

  这个也我在公司的使用自己写库clib中在win上连接服务器时中只能连接63个的客户端，这个我一开始还以为是自己的配置文件中配置问题呢  就没有注意这个问题。  直到最近自己服务器上业务写完了， 需要压力测试时， 这个问题我可查有几天， 一开始我以为是自己编码中写死了呢， 为什么这样说呢！ 在我的知识体系中select最大连接数是1024， 我找很长时间 最后在发现在win 上 FD_SETSIZE的宏定义是64。 这才定位到问题。


  使用原生的socket写 服务器或者写客户端都会使用监听文件描述符 读写的状态, 都遇到文件描述符到1024或者64的连接限制, 但是linux内核2.6以后有了epoll就可以完美突破这个限制, 当是在win上和其它平台没有epoll支持, 就要使用select 和poll等等 函数,它们都是连接数 的限制的。


还有一次我面试一家游戏公司， 面试官问到我知道linux上最大连接数， 我当时以为是 一个命令呢， ulimit修改linux连接数呢，现在我了解一点 连接数关于编程方面的限制。与大家分享一下
  
## 正文

### 一, 简单介绍 fd_set使用 

#### 1, fd_set 结构中定义结构体的大小

```
typedef struct fd_set {
        u_int fd_count;               /* how many are SET? */
        SOCKET  fd_array[FD_SETSIZE];     /* an array of SOCKETs */// FD_SETSIZE：  最大连接数据
} fd_set;
```


#### 2, 添加文件描述符到 fd_set中的数组中fd_array

```
#define FD_SET(fd, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set FAR *)(set))->fd_count; __i++) { \ // 查找fd_array数组中文件描述符的位置
        if (((fd_set FAR *)(set))->fd_array[__i] == (fd)) { \
            break; \
        } \
    } \
    if (__i == ((fd_set FAR *)(set))->fd_count) { \
        if (((fd_set FAR *)(set))->fd_count < FD_SETSIZE) { \  // 插入文件描述符到数组中 并且增加 管理的文件多少 fd_count  
            ((fd_set FAR *)(set))->fd_array[__i] = (fd); \
            ((fd_set FAR *)(set))->fd_count++; \
        } \
    } \
} while(0, 0)

```

#### 3, 在数组中删除文件描述符 

```
#define FD_CLR(fd, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set FAR *)(set))->fd_count ; __i++) { \
        if (((fd_set FAR *)(set))->fd_array[__i] == fd) { \
            while (__i < ((fd_set FAR *)(set))->fd_count-1) { \
                ((fd_set FAR *)(set))->fd_array[__i] = \
                    ((fd_set FAR *)(set))->fd_array[__i+1]; \
                __i++; \
            } \
            ((fd_set FAR *)(set))->fd_count--; \
            break; \
        } \
    } \
} while(0, 0)
```



```


extern int PASCAL FAR __WSAFDIsSet(SOCKET fd, fd_set FAR *);

#define FD_ZERO(set) (((fd_set FAR *)(set))->fd_count=0)

#define FD_ISSET(fd, set) __WSAFDIsSet((SOCKET)(fd), (fd_set FAR *)(set))
```



如果你还是不了解可以看我以前写文章地址：https://chensongpoixs.github.io/2017/10/31/Linux网络编程之IO复用(select函数的使用)/

### 二, 分析问题怎么突破FD_SETSIZE的限制

#### 1, FD_SETSIZE的win宏定义是64

我们只要自己定义  fd_set 的结构 数组的大小就可以了

这里我们就要使用适配器(fd_set_adapter)怎么一个东西 , 我相信写android的界面的知道怎么一个东西 ， 我也前几天写android东西，才想起以前写android一些东西， 发现android一些设计模式太多了， 适合我们学习的设计模式的，挺搞笑的 我连java的基本List数据结构都不会使用， 我以为跟C++的数据结构一样的使用方式呢， 足足花了我半天的时间，我才知道要 new一下才可以使用， 要给这个对象申请内存空间。  毕竟android系统是google开源出来了， 里面有很多东西要学习的


这个适配器我也看boost中源码有适配器对win做了适配器，boost库也是C++中著名网络库，


我是借鉴boost1.6是动态增加数组 


```
namespace chen {
	class cfd_set_adapter
	{
	private:
		enum { default_fd_set_size = 1024 };
		struct _fd_set {
			uint32		m_count; //typedef unsigned int    u_int;
			socket_type m_fd_array[1];//动态申请数组大小
		};
	public:
		explicit cfd_set_adapter();
		~cfd_set_adapter();
	public:
		bool	init();
		void	destroy();
	public:
		bool set(socket_type descriptor);
		
		// FD_ISSET
		bool is_set(socket_type descriptor) const;

		// []
		operator fd_set*();
		
		void reset();


		socket_type max_descriptor() const
		{
			return m_max_descriptor_fd;
		}
	private:
		//扩容
		void reserve(uint32 size);
	private:
		cfd_set_adapter(const cfd_set_adapter&);
		cfd_set_adapter& operator=(const cfd_set_adapter&);
	private:
		_fd_set*			m_fd_set;
		uint32				m_capacity;// 数组的大小
		uint32				m_max_descriptor_fd;
	};
} // namespace chen
```


#### 2, 分析boost 库中怎么使用select 适配器的fd_set_adapter



使用在主要在run方法中

主要思想是 文件描述符队列 op_queue<operation>  （连接上文件描述符 ）

fd_sets_是分别放read,write和except三个数组

然后把队列中文件描述符放到fd_set_三个文件描述符数组中 在放到select函数中

```
void select_reactor::run(bool block, op_queue<operation>& ops)
{
  boost::asio::detail::mutex::scoped_lock lock(mutex_);

#if defined(BOOST_ASIO_HAS_IOCP)
  // Check if the thread is supposed to stop.
  if (stop_thread_)
    return;
#endif // defined(BOOST_ASIO_HAS_IOCP)

  // Set up the descriptor sets.
  for (int i = 0; i < max_select_ops; ++i)
    fd_sets_[i].reset();
  fd_sets_[read_op].set(interrupter_.read_descriptor());
  socket_type max_fd = 0;
  bool have_work_to_do = !timer_queues_.all_empty();
  for (int i = 0; i < max_select_ops; ++i)
  {
    have_work_to_do = have_work_to_do || !op_queue_[i].empty();
    fd_sets_[i].set(op_queue_[i], ops);
    if (fd_sets_[i].max_descriptor() > max_fd)
      max_fd = fd_sets_[i].max_descriptor();
  }

#if defined(BOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
  // Connection operations on Windows use both except and write fd_sets.
  have_work_to_do = have_work_to_do || !op_queue_[connect_op].empty();
  fd_sets_[write_op].set(op_queue_[connect_op], ops);
  if (fd_sets_[write_op].max_descriptor() > max_fd)
    max_fd = fd_sets_[write_op].max_descriptor();
  fd_sets_[except_op].set(op_queue_[connect_op], ops);
  if (fd_sets_[except_op].max_descriptor() > max_fd)
    max_fd = fd_sets_[except_op].max_descriptor();
#endif // defined(BOOST_ASIO_WINDOWS) || defined(__CYGWIN__)

  // We can return immediately if there's no work to do and the reactor is
  // not supposed to block.
  if (!block && !have_work_to_do)
    return;

  // Determine how long to block while waiting for events.
  timeval tv_buf = { 0, 0 };
  timeval* tv = block ? get_timeout(tv_buf) : &tv_buf;

  lock.unlock();

  // Block on the select call until descriptors become ready.
  boost::system::error_code ec;
  int retval = socket_ops::select(static_cast<int>(max_fd + 1),
      fd_sets_[read_op], fd_sets_[write_op], fd_sets_[except_op], tv, ec);

  // Reset the interrupter.
  if (retval > 0 && fd_sets_[read_op].is_set(interrupter_.read_descriptor()))
  {
    interrupter_.reset();
    --retval;
  }

  lock.lock();

  // Dispatch all ready operations.
  if (retval > 0)
  {
#if defined(BOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
    // Connection operations on Windows use both except and write fd_sets.
    fd_sets_[except_op].perform(op_queue_[connect_op], ops);
    fd_sets_[write_op].perform(op_queue_[connect_op], ops);
#endif // defined(BOOST_ASIO_WINDOWS) || defined(__CYGWIN__)

    // Exception operations must be processed first to ensure that any
    // out-of-band data is read before normal data.
    for (int i = max_select_ops - 1; i >= 0; --i)
      fd_sets_[i].perform(op_queue_[i], ops);
  }
  timer_queues_.get_ready_timers(ops);
}

```

### 三, 测试效果图
因为在win限制是64 我使用200 机器人连接gateway gateway的session回话id 从1000 到800,    机器人每30秒发送一个数据包。

下面的测试流程图

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2019-06-16/1024_fd_setsize.gif?raw=true)



源码路径

https://github.com/chensongpoixs/clib


## 结语




