---
layout:     post
title:      boost库asio源码分析之数据结构
subtitle:   分析连接, 发送, 接受数据的流程
date:       2019-01-22
times:      05::53::45
author:     chensong
header-img: img/2018-01-23/post-bg-mp4.jpg
catalog: 	 true
tags:
    - boost库asio源码探秘
    - 网络编程
    - 设计模式
---

## 前言

源码之前，了无秘密。
                                                       ——侯捷

Boost库是一个可移植、提供源代码的C++库，作为标准库的后备，是C++标准化进程的开发引擎之一。Boost库由C++标准委员会库工作组成员发起，其中有些内容有望成为下一代C++标准库内容。在C++社区中影响甚大，是不折不扣的“准”标准库。

boost.asio是Boost库中非常著名的I/O组件，是用于网络和低层IO编程的跨平台C++库，为开发者提供了C++环境下稳定的异步模型。其在性能、移植性、扩展性等方面均为人称道，甚至被很多业内人士称为“网络神器”。asio是目前唯一有希望进入C++标准库以弥补标准库在网络方面的缺失的C++网络库，因此对asio的学习在某种意义上可以说是学习C++网络编程的必修课。

当前网络上从用户角度介绍asio的文献很多也很完善，所以本文决定另辟蹊径，从asio源码角度出发，由内而外、深入浅出地剖析asio的架构和设计理念，将asio的一切秘密呈现在读者眼前。。

分析boost库1.6版本
我们讨论是linux平台的

## 正文

boost库asio 为了适应不同平台使用采用了策略模式
 

### 一,  boost库asio的一些数据结构

boost库中的回调函数的使用

task_io_service_operation 模板在asio 处理任务的 一直在回调do_perform函数 处理完成结束

使用模板方法模式

下面是一个简单例子


```
#include <iostream>
#include <cstdlib>

class operation
{
public:
  void complete(const int& ec, std::size_t bytes)
  {
    printf("[%s][%d]\n", __FUNCTION__, __LINE__);
    func_(this, ec, bytes);
  }
  void destroy()
  {
    printf("[%s][%d]\n", __FUNCTION__, __LINE__);
    func_(this, int(), 0);
  }
protected:
  typedef void (*func_type)(operation *, const int&, std::size_t);
  operation(func_type func) :func_(func)
  {
    printf("[%s][%d]\n", __FUNCTION__, __LINE__); 
  }
private:
  func_type func_;
};

class reactor_op :public operation
{
public:
  bool perform()
  {
    printf("[%s][%d]\n", __FUNCTION__, __LINE__);
    return perform_func_(this);
  }
protected:
  typedef bool(*perform_func_type)(reactor_op*);
  reactor_op(perform_func_type perform_func, func_type complete_func)
    :operation(complete_func),
    perform_func_(perform_func) 
  {
    printf("[%s][%d]\n", __FUNCTION__, __LINE__);
  }
private:
  perform_func_type perform_func_;
};


class reactive_op_base : public reactor_op
{
public:
  reactive_op_base(func_type complete_func)
    : reactor_op(&reactive_op_base::do_perform, complete_func)
  {
    printf("[%s][%d]\n", __FUNCTION__, __LINE__);
  }
  //这里处理任务 直到任务处理完成结束提出
  static bool do_perform(reactor_op * base)
  {
    reactive_op_base * r(static_cast<reactive_op_base*>(base));
    // r->perform(); // cycle
    printf("[%s][%d]function = %p\n", __FUNCTION__, __LINE__, r);
    
      //r->complete(34, 34);
    return true;
  }
protected:
private:

};

class reactive_op : public reactive_op_base
{
public:
  reactive_op()
    :reactive_op_base(&reactive_op::do_complete)
  {
    printf("[%s][%d]\n", __FUNCTION__, __LINE__);
  }
  static void do_complete(operation* base, const int &, std::size_t)
  {
    reactor_op * o(static_cast<reactor_op*>(base));
    printf("[%s][%d]function = %p\n", __FUNCTION__, __LINE__, o);
    o->perform();
    printf("[%s][%d]\n", __FUNCTION__, __LINE__);
  }
protected:
private:
};

int main(int argc, char *argv[])
{
  reactive_op reactive;
  printf("[%s][%d]function = %p\n", __FUNCTION__, __LINE__, &reactive);
  operation * o(static_cast<operation*>(&reactive));
  o->complete(4, 5);
  return EXIT_SUCCESS;
}


```

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2019-01-22/task_opercation.png?raw=true)



C++中类中的回调函数  


```
class AA
{
public:
//AA
//{}
// ~AA
//{}
typedef void (*static_back)(int);   // static 
typedef void (AA::*call_back)(int);
protected:
private:
//std::string name;
static_back _func; // sizeof(AA) == 8    // CentOS 64
call_back func_;  // sizeof(AA) == 16    // CentOS 64
//void *a;   // sizeof(AA) == 8  ---> class 4---> CentOS 64
};
```


### 二, tcp 

```
  /// The type of a TCP endpoint.
  typedef basic_endpoint<tcp> endpoint;
 /// The TCP socket type.
  typedef basic_stream_socket<tcp> socket;

  /// The TCP acceptor type.
  typedef basic_socket_acceptor<tcp> acceptor;

  /// The TCP resolver type.
  typedef basic_resolver<tcp> resolver;

```


这四个类模板是干什么呢

1. basic_endpoint
2. basic_stream_socket
3. basic_socket_acceptor
4. basic_resolver

#### 1, basic_endpoint

basic_endpoint是管理IP地址和port端口的

boost/asio/ip/basic_endpoint.hpp

basic_endpoint.hpp 管理boost/asio/ip/detail/endpoint.hpp

endpoint.hpp管理address.hpp

区分ipv4和ipv6的工具类
boost/asio/ip/address.hpp是管理 boost/asio/ip/address_v4.hpp boost/asio/ip/address_v6.hpp的工具类


下面是boost中定义ipv4和ipv6的结构体在boost/asio/detail/socket_types.hpp

```
struct socket_addr_type { int sa_family; };
struct sockaddr_in4_type { int sin_family;
  in4_addr_type sin_addr; u_short_type sin_port; };
struct sockaddr_in6_type { int sin6_family;
  in6_addr_type sin6_addr; u_short_type sin6_port;
  u_long_type sin6_flowinfo; u_long_type sin6_scope_id; };
```


#### 2, basic_stream_socket



stream_socket_service
basic_socket
basic_io_object
socket_base
reactive_socket_service



主要的是模板是reactive_socket_service 继承reactive_socket_service_base

管理'发送数据'和'接受数据'

start_op 实际调用 epoll_reactor中方法start_op和post_immediate_completion方法

reactive_socket_service_base模板中start_op方法

```
void reactive_socket_service_base::start_op(
    reactive_socket_service_base::base_implementation_type& impl,
    int op_type, reactor_op* op, bool is_continuation,
    bool is_non_blocking, bool noop)
{
  if (!noop)
  {
    if ((impl.state_ & socket_ops::non_blocking)
        || socket_ops::set_internal_non_blocking(   // flags IO 
          impl.socket_, impl.state_, true, op->ec_))
    {
      reactor_.start_op(op_type, impl.socket_,
          impl.reactor_data_, op, is_continuation, is_non_blocking);
      return;
    }
  }

  reactor_.post_immediate_completion(op, is_continuation);
}

```


其实basic_socket_acceptor和basic_resolver模板和basic_stream_socket都是差不多

basic_socket_acceptor 的new client连接的处理
basic_stream_socket 是 发送和接受的处理

basic_resolver 这个模板目前我在看源码分析

### 四, socket的tcp三次握手

具体的分析可以结合TCP的"四次握手"关闭. TCP是全双工的信道, 可以看作两条单工信道, TCP连接两端的两个端点各负责一条. 当对端调用close时, 虽然本意是关闭整个两条信道, 但本端只是收到FIN包. 按照TCP协议的语义, 表示对端只是关闭了其所负责的那一条单工信道, 仍然可以继续接收数据. 也就是说, 因为TCP协议的限制, 一个端点无法获知对端的socket是调用了close还是shutdown.

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2019-01-22/tcp_disconnect.jpg?raw=true)

对一个已经收到FIN包的socket调用read方法, 如果接收缓冲已空, 则返回0, 这就是常说的表示连接关闭. 但第一次对其调用write方法时, 如果发送缓冲没问题, 会返回正确写入(发送). 但发送的报文会导致对端发送RST报文, 因为对端的socket已经调用了close, 完全关闭, 既不发送, 也不接收数据. 所以, 第二次调用write方法(假设在收到RST之后), 会生成SIGPIPE信号, 导致进程退出.

为了避免进程退出, 可以捕获SIGPIPE信号, 或者忽略它, 给它设置SIG_IGN信号处理函数:

boost库设计忽略此信号

```
socket_type socket(int af, int type, int protocol,
    boost::system::error_code& ec)
{
  clear_last_error();
#if defined(BOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
  socket_type s = error_wrapper(::WSASocketW(af, type, protocol, 0, 0,
        WSA_FLAG_OVERLAPPED), ec);
  if (s == invalid_socket)
    return s;

  if (af == BOOST_ASIO_OS_DEF(AF_INET6))
  {
    // Try to enable the POSIX default behaviour of having IPV6_V6ONLY set to
    // false. This will only succeed on Windows Vista and later versions of
    // Windows, where a dual-stack IPv4/v6 implementation is available.
    DWORD optval = 0;
    ::setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
        reinterpret_cast<const char*>(&optval), sizeof(optval));
  }

  ec = boost::system::error_code();

  return s;
#elif defined(__MACH__) && defined(__APPLE__) || defined(__FreeBSD__)
  socket_type s = error_wrapper(::socket(af, type, protocol), ec);
  if (s == invalid_socket)
    return s;

  int optval = 1;
  int result = error_wrapper(::setsockopt(s,
        SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)), ec); // 对端 关闭close socket , 第二次调用write会报 信号SIGPIPE 程序崩溃  Broken pipe向一个没有读端的管道写数据 , 设置SO_NOSIGPIPE 就忽略信号 会返回错误码了
  if (result != 0) // RST 
  {
    ::close(s);
    return invalid_socket;
  }

  return s;
#else
  int s = error_wrapper(::socket(af, type, protocol), ec);
  if (s >= 0)
    ec = boost::system::error_code();
  return s;
#endif
}
```

## 结语



