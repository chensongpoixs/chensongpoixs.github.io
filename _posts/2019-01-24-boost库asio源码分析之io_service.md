---
layout:     post
title:      boost库asio源码分析之io_service
subtitle:   任务队列的的调度
date:       2019-01-24
times:      21::07::01
author:     chensong
header-img: img/2019-01-24/bg_io_service.jpg
catalog: 	 true
tags:
    - boost库asio
    - 设计模式
---


## 前言

任务队列的的调度的使用


![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2019-01-24/io_service.jpg?raw=true)


## 正文


### 一, boost库asio io_service 的介绍

io_servie是接口类，为实现跨平台，采用了策略模式，所有接口均有impl_type实现。根据平台不同impl_type分为

win_iocp_io_service Win版本的实现，这里主要分析Linux版本。

task_io_service 非win平台下的实现，其代码结构为：

detail/task_io_service_fwd.hpp 简单声明task_io_service名称

detail/task_io_service.hpp 声明task_io_service的方法和属性

detail/impl/task_io_service.ipp 具体实现文件

队列中的任务类型为opertioan，原型其实是typedef task_io_service_operation operation，其实现文件在detail/task_io_service_operation.hpp中，当队列中的任务被执行时，就是task_io_service_operation:complete被调用的时候。

下面是task_io_service类的图

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2019-01-24/task_io_service_operaction.png?raw=true)


### 二, task_io_service::run方法




实际调用do_run_one函数

#### 1， 优先处理队列中的任务 

回调task_io_service_operation::complete函数处理 队列任务

任务队列在上面一篇文章已经介绍过来 是 使用设计模式中 模板方法模式

类的关系图

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2019-01-24/accept_send_recv.png?raw=true)

#### 2， 没有任务时

没有任务实际时阻塞 _task->run方法中  task_变量类型为reactor，在linux平台实现为epoll_reactor，实现代码文件为detail/impl/epoll_reactor.ipp，run方法实际上执行的是epoll_wait，run阻塞在epoll_wait上等待事件到来，并且处理完事件后将需要回调的函数push到io_servie的任务队列中，虽然epoll_wait是阻塞的，但是它提供了interrupt函数，该interrupt是如何实现的呢，它向epoll_wait添加一个文件描述符，该文件描述符中有8个字节可读，这个文件描述符是专用于中断epoll_wait的，他被封装到select_interrupter中，select_interrupter实际上实现是eventfd_select_interrupter，在构造的时候通过pipe系统调用创建两个文件描述符，然后预先通过write_fd写8个字节，这8个字节一直保留。在添加到epoll_wait中采用EPOLLET水平触发，这样，只要select_interrupter的读文件描述符添加到epoll_wait中，立即中断epoll_wait。很是巧妙。


#### 3， Run方法的原则是

1. 有任务立即执行任务，尽量使所有的线程一起执行任务
2. 若没有任务，阻塞在epoll_wait上等待io事件
3. 若有新任务到来，并且没有空闲线程，那么先中断epoll_wait,先执行任务
4. 若队列中有任务，并且也需要epoll_wait监听事件，那么非阻塞调用epoll_wait（timeout字段设置为0），待任务执行完毕在阻塞在epoll_wait上。
5. 几乎对线程的使用上达到了极致。
6. 从这个函数中可以知道，在使用ASIO时，io_servie应该尽量多，这样可以使其epoll_wait占用的时间片最多，这样可以最大限度的响应IO事件，降低响应时延。但是每个task_io_servie::run占用一个线程，所以task_io_servie最佳应该和CPU的核数相同。



![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2019-01-24/async_accept.jpg?raw=true)


Linux下查看某个进程的线程数量

>top -Hp 进程号

## 结语




