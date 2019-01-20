---
layout:     post
title:      Linux网络编程(epoll的边缘触发和水平触发)多路IO高并发服务器
subtitle:   epoll的两种模式-LT模式和高速度ET模式
date:       2017-11-01
times:      23::45::37
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


#### 1, fcntl的介绍

fcntl函数可以改变已经打开的文件的性质

```
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd, ... /* arg */ );  // return -1 error
```
fcntl函数有5种功能

1. 复制一个现有的文件描述符 (cmd = F_DUPFD)
2. 获得/设置文件描述符标记 (cmd = F_GETFD 或 F_SETFD)
3. 获得/设置文件状态标志 (cmd = F_GETFL 或 F_SETFL)
4. 获取/设置异步I/O所有权 (cmd = F_GETOWN 或 F_SETOWN)
5. 获取/设置记录锁 (cmd = F_GETLK, F_SETLK 或 F_SETLKW)

十种文件描

|cmd|descriptor|
|:---:|:---|
|F_DUPFD|复制文件描述符filedes。 新文件描述符作为函数值返回。它是尚未打开的各描述符中大于或等于第三个参数值(取为整型值)中各值的最小值。 新描述符与filedes共享同一文件表项 但是，新描述符有它自己的一套文件描述符标志， 其FD_CLOEXEC文件描述符标志被清除(这表示描述符在通过一个exec时还是保持有效)|
|F_GETFD|对应于filedes的文件描述符标志作为函数值返回。 当前只定义了一个文件描述符标志FD_CLOEXEC|
|F_SETFD|对于filedes设置文件描述符标志。新标志值按第三个参数(取为整型值)设置|

#### 2, timerfd_create函数

```
int timerfd_create(int clockid, int flags);
 
它是用来创建一个定时器描述符timerfd
 
第一个参数：clockid指定时间类型，有两个值：
 
CLOCK_REALTIME :Systemwide realtime clock. 系统范围内的实时时钟
 
CLOCK_MONOTONIC:以固定的速率运行，从不进行调整和复位 ,它不受任何系统time-of-day时钟修改的影响
 
第二个参数：flags可以是0或者O_CLOEXEC/O_NONBLOCK。
 
返回值：timerfd（文件描述符）
```
timerfd_settime函数:

用于启动和停止定时器，fd为timerfd_create获得的定时器文件描述符，flags为0表示是相对定时器，为TFD_TIMER_ABSTIME表示是绝对定时器。const struct itimerspec *new_value表示设置超时的时间。

int timerfd_settime(int ufd, int flags, const struct itimerspec * utmr, struct itimerspec * otmr);

此函数用于设置新的超时时间，并开始计时。
ufd，timerfd_create返回的文件句柄。
flags，为1代表设置的是绝对时间；为0代表相对时间。
utmr为需要设置的时间。
otmr为定时器这次设置之前的超时时间。
一般来说函数返回0代表设置成功。


```
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
 
#define MX_EVNTS 10
#define EPL_TOUT 3000
#define MX_CNT 5
 
struct param{
	struct itimerspec its;
	int tfd;
};
 
void *strt_eplth(void *arg)
{
	struct epoll_event evnts[MX_EVNTS];
	int *eplfd = (int *)arg;
	int n = -1;
	size_t i,cnt = 0;
	while(1){
		n = epoll_wait(*eplfd,evnts,MX_EVNTS,EPL_TOUT);
		if(n == -1){
			perror("epoll_wait() error");
			break;
		}else if(n == 0){
			printf("time out %d sec expired\n",EPL_TOUT / 1000);
			break;
		}
		for(i = 0; i < n;i++){
			struct param *pm = (struct param *)(evnts[i].data.ptr);
			printf("tfd: %d\ninitial expiration: %ld\ninterval: %ld\n\n",
				pm->tfd,
				(long)(pm->its.it_value.tv_sec),
				(long)(pm->its.it_interval.tv_sec));
			if(epoll_ctl(*eplfd,EPOLL_CTL_DEL,pm->tfd,NULL) != 0){
				perror("epoll_ctl(DEL) error in thread");
				break;
			}
			struct epoll_event ev;
			ev.events = EPOLLIN | EPOLLET;
			pm->its.it_value.tv_sec =
				pm->its.it_value.tv_sec +
				pm->its.it_interval.tv_sec;
			ev.data.ptr = pm;
			if(timerfd_settime(pm->tfd,TFD_TIMER_ABSTIME,&(pm->its),NULL) != 0){
				perror("timerfd_settime() error in thread");
				break;
			}
			if(epoll_ctl(*eplfd,EPOLL_CTL_ADD,pm->tfd,&ev) != 0){
				perror("epoll_ctl(ADD) error in thread");
				break;
			}
		}
		if(++cnt == MX_CNT){
			printf("cnt reached MX_CNT, %d\n",MX_CNT);
			break;
		}
	}
	close(*eplfd);
	pthread_exit(NULL);
}
 
int create_timerfd(struct itimerspec *its,time_t interval)
{
	int tfd = timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK);
	if(tfd < 0){
		perror("timerfd_create() error");
		return -2;
	}
	struct timespec nw;
	if(clock_gettime(CLOCK_MONOTONIC,&nw) != 0){
		perror("clock_gettime() error");
		return -1;
	}
	its->it_value.tv_sec = nw.tv_sec + interval;
	its->it_value.tv_nsec = 0;
	its->it_interval.tv_sec = interval;
	its->it_interval.tv_nsec = 0;
	return tfd;
}
 
int main(int argc, char *argv[])
{
	time_t INTERVAL = 2;
	struct itimerspec its;
	int tfd = create_timerfd(&its,INTERVAL);
	if(tfd < 0)
		return -1;
	int eplfd = epoll_create1(0);
	if(eplfd < 0){
		perror("epoll_create1() error");
		return -1;
	}
	struct param pm;
	pm.its = its;
	pm.tfd = tfd;
	if(timerfd_settime(pm.tfd,TFD_TIMER_ABSTIME,&(pm.its),NULL) != 0){
		perror("timerfd_settime() error");
		return -1;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.ptr = &pm;
	if(epoll_ctl(eplfd,EPOLL_CTL_ADD,pm.tfd,&ev) != 0){
		perror("epoll_ctl() error");
		return -1;
	}
	pthread_t pid;
	if(pthread_create(&pid,NULL,strt_eplth,(void *)&eplfd) != 0){
		perror("pthread_create() error");
		return -1;
	}
 
	//// add another timerfd.
	INTERVAL = 1;
	struct itimerspec its2;
	int tfd2 = create_timerfd(&its2,INTERVAL);
	if(tfd2 < 0)
		return -1;
	struct param pm2;
	pm2.its = its2;
	pm2.tfd = tfd2;
	if(timerfd_settime(pm2.tfd,TFD_TIMER_ABSTIME,&(pm2.its),NULL) != 0){
		perror("timerfd_settime() error");
		return -1;
	}
	struct epoll_event ev2;
	ev2.events = EPOLLIN | EPOLLET;
	ev2.data.ptr = &pm2;
	if(epoll_ctl(eplfd,EPOLL_CTL_ADD,pm2.tfd,&ev2) != 0){
		perror("epoll_ctl() error");
		return -1;
	}
 
	if(pthread_join(pid,NULL) != 0){
		perror("pthread_join() error");
		return -1;
	}
	close(tfd);
	close(tfd2);
	return 0;
}
```


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

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2017-11-01/server.gif?raw=true)


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

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2017-11-01/epoll_ET.gif?raw=true)

## 结语
