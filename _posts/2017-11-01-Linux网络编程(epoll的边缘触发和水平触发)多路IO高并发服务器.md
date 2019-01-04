---
layout:     post
title:      Linux网络编程(epoll的边缘触发和水平触发)多路IO高并发服务器
subtitle:   epoll的两种模式-LT模式和高速度ET模式
date:       2017-11-01
times:      :23:45:37
author:     chensong
header-img: img/2017-11-01/bg-epoll.jpg
catalog: 		true
tags:
    - 网络编程
---



## 前言

1. epoll是linux服务器中使用
2. 在android系统中使用进程保护机制
3. 任务队列的使用

## 正文

### 一，epoll函数

```
不管是select还是poll，都是需要遍历数组，轮询。
select支持1024个客户端以下的并发服务器足够，select 在大量并发，少量活跃的情况下效率比较低。
推荐使用epoll模型
epoll也可以突破 1024限制 ，不跨平台。 

epoll的函数介绍
创建 epoll的树根
int epoll_create(int size);
		○ size 大小 ，节点的个数
		○ Since Linux 2.6.8, the size argument is ignored，内核2.6.8以后可以忽略，但是必须大于0
		○ 返回值
			§ 返回 文件描述符-->树根


typedef union epoll_data {
               void        *ptr; epoll反应堆的实现
               int          fd;
               uint32_t     u32;
               uint64_t     u64;
           } epoll_data_t;

           struct epoll_event {
               uint32_t     events;      /* Epoll events */
               epoll_data_t data;        /* User data variable */
           };

epoll树节点的控制 -- 上树 - 下树 -- 修改节点
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
		○ epfd epoll的树根，epoll_create的返回值
		○ op 选项 
			§ EPOLL_CTL_ADD 上树
			§ EPOLL_CTL_MOD 修改
			§ EPOLL_CTL_DEL  下树 
		○ fd 文件描述符，要监听的描述符
		○ event  要监听的事件
			§ EPOLLIN 可读
			§ EPOLLOUT 可写
			§ EPOLLERR 异常 

等待epoll监听结果返回
int epoll_wait(int epfd, struct epoll_event *events,
                      int maxevents, int timeout);
		○ epfd  根节点
		○ events  传出参数，已经设置好监听了，所以不需要传入，是一个数组
		○ maxevents 数组的上限 
		○ timeout 超时，等同于poll
			§ -1  代表永久阻塞，直到有监控的事件发生
			§ 0    代表非阻塞，立即返回
			§ >0  限时等待，毫秒级，直到有监控的事件发生或者超时返回
		○ 返回值
			§ 成功 返回就绪事件个数 
	
	
创建根节点 
上树  
循环等待事件产生 
	处理事件 

练习1： 利用epoll监控管道的读端   
		○ 创建管道 
		○ 创建子进程 
			§ 子进程 发信息  
			§ 关闭读端 fd[0] 
		○ 父进程 接收信息 
			§ 关闭写端 fd[1]
			§ epoll监控 
				□ 根节点
				□ 上树
				□ 循环等待事件 产生 
				□ 打印到屏幕 
			§ 回收子进程 
	
	练习2： 用epoll实现并发服务器 
		○ 创建socket
		○ 绑定bind 端口复用
		○ 监听listen
		○ 处理epoll的事情
			§ 创建根节点
			§ 添加监听节点 
			§ 循环等待epoll返回 
				□ 新连接 
					® 得到新连接，上树
				□ 客户端写数据
读数据，发回去
```


###	二，服务器编码


使用epoll的套路是：

epoll_create();
epoll_ctl()   //添加和删除
epoll_wait()   //阻塞等待

```
/*************************************************************************
	> File Name: server_epoll.c
	> Author: songli
	> QQ: 2734030745
	> Mail: 15850774503@163.com
    > CSDN: http://my.csdn.net/Poisx
	> github: https://github.com/chensongpoixs
	> Created Time: Wed 01 Nov 2017 10:51:02 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include "wrap.h"

#define SERV_PORT 8888


int main(int argc, char *argv[])
{
    int listenfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    int i, optval = 1;
    int nready, fepol;
    char strip[16];



    //创建socket
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    //绑定ip和port
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //端口复用
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    //监听Listen
    Listen(listenfd, 128);

    //===             epoll init 操作        ==================
    fepol = epoll_create(FD_SETSIZE);
    //创建红黑树
    struct epoll_event ev, evs[FD_SETSIZE];
    ev.data.fd = listenfd;
    ev.events = EPOLLIN; //监听事件
    epoll_ctl(fepol, EPOLL_CTL_ADD, listenfd, &ev);

    while (1) {
        //设置阻塞事件epoll_wait
        nready = epoll_wait(fepol, evs, FD_SETSIZE, -1);

        printf("nready :%d\n", nready);
        if (nready > 0) {
            for (i = 0; i < nready; i++) {
                //判断是否有新的client链接
                if (evs[i].data.fd == listenfd) { // listenfd 描述符是监听有新的client链接信息
                    if (evs[i].events & EPOLLIN) {
                        int connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &len);
                        printf("client :ip:%s, port:%d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, strip, sizeof(strip)),  ntohs(cliaddr.sin_port));
                        if (connfd > 0) {
                            //添加一个树
                            ev.data.fd = connfd;
                            epoll_ctl(fepol, EPOLL_CTL_ADD, connfd, &ev);
                        }
                    } 
                }
                else {
                    //判断是否有client发送信息
                    if (evs[i].events & EPOLLIN) {
                        char buf[6] = {0};
                        int ret = Read(evs[i].data.fd, buf, sizeof(buf)-1);
                        if (ret < 0) {
                            perror("Read error ");
                            Close(evs[i].data.fd);
                            ev.data.fd = evs[i].data.fd;
                            epoll_ctl(fepol, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
                        }
                        else if (ret == 0) {
                            printf("client 退出\n");
                            Close(evs[i].data.fd);
                            ev.data.fd = evs[i].data.fd;
                            epoll_ctl(fepol, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
                        }
                        else {
                            Write(STDOUT_FILENO, buf, ret);
                            Write(evs[i].data.fd, buf, ret);
                        }
                    }
                }
            }
        }
    }
    

    Close(fepol);
    Close(listenfd);
	return 0;
}

```

![](http://img.blog.csdn.net/20171101232516708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUG9pc3g=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


###	三，epoll的边缘触发

<font color = red >

**区别是只通知一次**





```
EPOLL事件有两种模型：
	Edge Triggered (ET) 边缘触发只有数据到来才触发，不管缓存区中是否还有数据。
Level Triggered (LT) 水平触发只要有数据都会触发。


EPOLLET epoll的ET模式选项  
struct epoll_event ev;
ev.data.fd = lfd;
ev.events = EPOLLIN | EPOLLET; //设置读事件的ET模式

ET模式  高速模式  == ET + 非阻塞 
如果使用ET模式需要的注意事项 
		○ 内核如果只通知一次，必须要一次把数据读完
		○ 必须使用非阻塞模式 


```

</font>

```
/*************************************************************************
	> File Name: server_epoll.c
	> Author: songli
	> QQ: 2734030745
	> Mail: 15850774503@163.com
    > CSDN: http://my.csdn.net/Poisx
	> github: https://github.com/chensongpoixs
	> Created Time: Wed 01 Nov 2017 10:51:02 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include "wrap.h"
#include <fcntl.h>
#include <errno.h>

#define SERV_PORT 8888


int main(int argc, char *argv[])
{
    int listenfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    int i, optval = 1;
    int nready, fepol;
    char strip[16];
    int flags;



    //创建socket
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    //绑定ip和port
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //端口复用
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    //监听Listen
    Listen(listenfd, 128);
    //++++++++++++++++       设置非阻塞       ++++++++++++
    flags = fcntl(listenfd, F_GETFL);
    flags |= O_NONBLOCK;  //非阻塞事件
    fcntl(listenfd, F_SETFL, flags);


    //===             epoll init 操作        ==================
    fepol = epoll_create(FD_SETSIZE);
    //创建红黑树
    struct epoll_event ev, evs[FD_SETSIZE];
    ev.data.fd = listenfd;
    //ev.events = EPOLLIN; //监听事件
    //+++++++++++     ET 模式   边缘++++++++++++++++++++++++
    ev.events = EPOLLIN | EPOLLET; //监听事件

    epoll_ctl(fepol, EPOLL_CTL_ADD, listenfd, &ev);

    while (1) {
        //设置阻塞事件epoll_wait
        nready = epoll_wait(fepol, evs, FD_SETSIZE, -1);

        printf("nready :%d\n", nready);
        if (nready > 0) {
            for (i = 0; i < nready; i++) {
                //判断是否有新的client链接
                if (evs[i].data.fd == listenfd) { // listenfd 描述符是监听有新的client链接信息
                    if (evs[i].events & EPOLLIN) {
                        int connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &len);
                        printf("client :ip:%s, port:%d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, strip, sizeof(strip)),  ntohs(cliaddr.sin_port));
                        if (connfd > 0) {
                            //添加一个树
                            //+++++++++++++ET 模式  边缘++++++++++++++++++++++++++++++++
                            ev.data.fd = connfd;
                            ev.events = EPOLLIN | EPOLLET;
                            // ==========   非阻塞事件=================
                            flags = fcntl(connfd, F_GETFL);
                            flags |= O_NONBLOCK; //非阻塞
                            fcntl(connfd, F_SETFL, flags);
                            epoll_ctl(fepol, EPOLL_CTL_ADD, connfd, &ev);
                        }
                    } 
                }
                else {
                    //判断是否有client发送信息
                    if (evs[i].events & EPOLLIN) {
                        char buf[6] = {0};
                        int ret;
                        while (1) {
                            ///遍历read数组数据
                            ret = Read(evs[i].data.fd, buf, sizeof(buf)-1);
                            if (ret < 0) {
                                if (errno == EAGAIN)
                                    break;
                                perror("Read error ");
                                Close(evs[i].data.fd);
                                ev.data.fd = evs[i].data.fd;
                                epoll_ctl(fepol, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
                                break;
                            }
                            else if (ret == 0) {
                                printf("client 退出\n");
                                Close(evs[i].data.fd);
                                ev.data.fd = evs[i].data.fd;
                                epoll_ctl(fepol, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
                                break;
                            }
                            else {
                                Write(STDOUT_FILENO, buf, ret);
                                Write(evs[i].data.fd, buf, ret);
                            }
                        }   
                    }
                }
            }
        }
    }
    

    Close(fepol);
    Close(listenfd);
	return 0;
}

```


![](http://img.blog.csdn.net/20171101234339474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUG9pc3g=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 结语
