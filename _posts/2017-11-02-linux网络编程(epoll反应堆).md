---
layout:     post
title:      linux网络编程(epoll反应堆)
subtitle:   反应堆的使用
date:       2017-11-02
times:      23::13::32
author:     chensong
header-img: img/2017-11-02/bg_epoll_heap.jpg
catalog: 		true
tags:
    - 网络编程
---



## 前言

对于epoll封装高并发服务器模式使用

## 正文


### 一，epoll反应堆

调用过程

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2017-11-02/epoll_heap.png?raw=true)


### 二，epoll编程

```
/*************************************************************************
	> File Name: server_epoll_btree.c
	> Author: songli
	> QQ: 2734030745
	> Mail: 15850774503@163.com
    > CSDN: http://my.csdn.net/Poisx
	> github: https://github.com/chensongpoixs
	> Created Time: Thu 02 Nov 2017 09:43:30 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define SERV_PROT 8888

typedef struct xxevent {
    int fd; // 描述符
    int events;  //事件
    void (*call_back)(int fd, int events, void *arg); //call
    void *arg;
    char buf[FD_SETSIZE];
    int buflen;
    int fped;
}xevent;

//btree 树根
int fepol = 0;

xevent myevents[FD_SETSIZE]; 

//读取数据
void readData(int fd, int events, void *arg);

//添加事件
void eventadd(int fd, int events, void (*call_back)(int, int, void *), void *arg, xevent *ev) {

    printf("方法名：%s \n", __FUNCTION__);
    ev->fd = fd;
    ev->events = events;
    ev->arg = arg;
    ev->call_back = call_back;
    //添加事件
    struct epoll_event epv;
    epv.data.ptr = ev;
    epv.events = events;    
    epoll_ctl(fepol, EPOLL_CTL_ADD, fd, &epv); //添加到红黑树上
}

//设置事件的操作
void eventset(int fd, int events, void (*call_back)(int, int, void*), void *arg, xevent *ev) {

    printf("方法名：%s\n", __FUNCTION__);
    ev->fd = fd;
    ev->events = events;
    ev->arg = arg;
    ev->call_back = call_back;
    struct epoll_event epv;
    epv.data.ptr = ev;
    epv.events = events;
    //修改节点事件
    epoll_ctl(fepol, EPOLL_CTL_MOD, fd, &epv);
}

//发送信息
void sendData(int fd, int events, void *arg) {
    
    printf("方法名：%s\n", __FUNCTION__);
    xevent *ev = arg;
    write(fd, ev->buf, ev->buflen);

    //修改事件
    eventset(fd, EPOLLIN, readData, arg, ev);
}


//删除节点
void eventdel(xevent *ev, int fd, int events) {
    
    printf("方法名：%s\n", __FUNCTION__);
    ev->fd = 0;
    ev->events = 0;
    memset(ev->buf, 0x00, sizeof(ev->buf));
    ev->buflen = 0;
    ev->arg = NULL;
    struct epoll_event epv;
    epv.data.ptr = NULL; //指针
    epv.events = events; //事件
    //删除节点
    epoll_ctl(fepol, EPOLL_CTL_DEL, fd, &epv);

}


//读取数据
void readData(int fd, int events, void *arg) {

    printf("方法名：%s\n", __FUNCTION__);
    xevent *ev = arg;
    ev->buflen = read(fd, ev->buf, sizeof(ev->buf));
    if (ev->buflen > 0)  { //client发送信息
        //添加事件发送信息
        eventset(fd, EPOLLOUT, sendData, arg, ev);
    } else if (ev->buflen == 0) {
        printf("client 退出\n");
        close(fd);
        //删除节点
        eventdel(ev, fd, events);
    }else {
        perror("read error");
        close(fd);
        //删除节点
        eventdel(ev, fd, events);
    }
}

//新的client链接
void initAccept(int fd, int events, void *arg) {
    
    printf("方法名：%s\n", __FUNCTION__);
    //   client 的信息的初始化操作
    struct sockaddr_in cliaddr;
    char strip[16];
    socklen_t len = sizeof(cliaddr);
    int i;

    int connfd = accept(fd, (struct sockaddr *)&cliaddr, &len);
    if (connfd > 0) {
        printf("client ip:%s,port:%d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, strip, sizeof(strip)), htons(cliaddr.sin_port));
        for (i = 0; i < FD_SETSIZE; i++ ) {
            //判断是否有空位
            if (myevents[i].fd == 0) 
                break; 
        }
    }
    //添加事件操作     client de 信息
    eventadd(connfd, EPOLLIN, readData, &myevents[i], &myevents[i]);
}


int main(int argc, char *argv[]){
    
    int listenfd;
    struct sockaddr_in servaddr;
    int optval = 1;

    //创建socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //绑定ip和port
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PROT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);


    //端口复用
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(servaddr));

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)  {
        perror("bind error");
        return -1;
    }

    //监听listen
    listen(listenfd, 128);

    //+++++++++++++        epoll init 操作           ++++++++++++++++===
    fepol = epoll_create(FD_SETSIZE); //创建btree 

    struct epoll_event events[FD_SETSIZE];

    //添加事件
    eventadd(listenfd, EPOLLIN, initAccept, &myevents[FD_SETSIZE -1], &myevents[FD_SETSIZE - 1]);


    while (1) {
        
        //阻塞事件
        int  ret = epoll_wait(fepol, events, FD_SETSIZE, -1);
        if (ret > 0) {
            int i;
            for (i = 0; i < ret; i++ ) {
                xevent *ev = events[i].data.ptr;
                //判断是否有事件
                if (ev->events & events[i].events) {
                    ev->call_back(ev->fd, ev->events, ev);
                }
            }
        }
    }

	return 0;
}

```

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2017-11-02/server_epoll_event.gif?raw=true)

## 结语

看上面例子有助于看第三方库封装
