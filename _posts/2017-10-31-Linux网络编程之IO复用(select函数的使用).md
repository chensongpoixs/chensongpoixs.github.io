---
layout:     post
title:      Linux网络编程之IO复用(select函数的使用)
subtitle:   select函数的使用-文件描述符集合的位图
date:       2017-10-31
times:      23::22::46
author:     chensong
header-img: img/2017-10-31/bg_select.jpg
catalog: 		true
tags:
    - 网络编程
---



## 前言

select 函数是控制 文件描述符集合的位图

## 正文


```
fd_set 文件描述符集合 位图 
struct timeval {
               long    tv_sec;         /* seconds */
               long    tv_usec;        /* microseconds */
           };

int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
		 nfds 代表三个集合中最大的文件描述符+1
		 readfds 读的集合  这三个集合都是传入传出
		 writefds 写的集合
		 exceptfds 异常的集合
		 timeout 超时时间 
			 NULL 代表永久阻塞 -- 直到监控的事件发生
			 全填0 立即返回，不管是否有监控的事件产生
			 填具体的值，限时等待 -- 直到监控的事件发生或者时间到了返回
	返回值
			 成功 
				 返回 三个文件描述符中就绪事件的个数
				 失败 返回 -1，设置errno



	文件描述符集合操作函数
	将fd从set中清除--置0
	 void FD_CLR(int fd, fd_set *set);
	判断fd是否在集合中
       int  FD_ISSET(int fd, fd_set *set);
			§ 返回值
				□ 成功 返回 1
				□ 失败 返回 0
	将fd添加到集合中   -- 置1 
	 void FD_SET(int fd, fd_set *set);
	将set集合清空 
       void FD_ZERO(fd_set *set);

```


### 一，TCP的三次回首

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2017-10-31/Three_Lookbacks_of_TCP.jpg?raw=true)

**关闭状态 FIN_WAIT_1**

### 二，端口复用技术

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2017-10-31/tcp.png?raw=true)

```
int getsockopt(int sockfd, int level, int optname,
                      void *optval, socklen_t *optlen);
       int setsockopt(int sockfd, int level, int optname,
                      const void *optval, socklen_t optlen);

```

### 三，IO复用的高并发服务器

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2017-10-31/High_concurrency_server_for_IO_reuse.png?raw=true)

```
/*************************************************************************
	> File Name: server_select_sum.c
	> Author: songli
	> QQ: 2734030745
	> Mail: 15850774503@163.com
    > CSDN: http://my.csdn.net/Poisx
	> github: https://github.com/chensongpoixs
	> Created Time: Tue 31 Oct 2017 10:10:16 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "wrap.h"
#include <sys/select.h>
#include <ctype.h>

#define SERV_PORT 8888

int main(int argc, char *argv[])
{
    int listenfd, maxfd, sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char buf[256], strip[16];
    int i, optval = 1;
    int client[FD_SETSIZE];
    fd_set rset, allset;
    int nready = 0; // select
    int maxindex = -1;
    //1,创建socket
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    //2，绑定ip和port
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //设置端口复用
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    //3，监听Listen
    Listen(listenfd, 128);
    //4，设置描述符集合
    FD_ZERO(&rset);
    FD_SET(listenfd, &rset);

    //副本
    maxfd = listenfd;
    allset = rset;
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;
    while (1)
    {
        rset = allset;
        //5，阻塞select事件
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (nready < 0)
            perror("select error");
        //5.1 判断是否client链接
        if (FD_ISSET(listenfd, &rset))
        {
            int connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &len);
            printf("client ip:%s, port:%d\n", 
                   inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, strip, sizeof(strip)),
                  ntohs(cliaddr.sin_port));
            for (i = 0; i < FD_SETSIZE; i++)
            {
                if (client[i] < 0)
                {
                    client[i] = connfd; //记录client描述符
                    break;
                }
            }
            if (i == FD_SETSIZE)
            {
                fputs("too manys clients\n", stderr);
                exit(1);
            }
            printf("i:%d\n", i);
            FD_SET(connfd, &allset); //client的描述符添加到集合中
            if (i > maxindex)
                maxindex = i;

            if (maxfd < connfd )
                maxfd = connfd; //最大描述符数量
            printf("maxindex:%d\n", maxindex);
            printf("maxfd:%d\n", maxfd);
            printf("nready:%d\n", nready);
            if (--nready == 0)
                continue;
        }
        //5.2 read信息的发送
        for (i = 0; i <= maxindex; i++)
        {
            printf("client[%d] = %d\n", i, client[i]);
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) //有client发送信息
            {
                int ret = Read(sockfd, buf, sizeof(buf));
                if (ret < 0)
                {
                    perror("Read error");
                    client[i] = -1;
                    Close(sockfd);
                    FD_CLR(sockfd, &allset); ///去除描述符
                }
                else if (ret == 0) //client 退出
                {
                    client[i] = -1;
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                }
                else 
                {
                    int j;
                    for (j = 0; j < ret; j++)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    Write(sockfd, buf, ret);
                }
                if (--nready == 0)  //处理完成返回
                    break; 
            }
        }
    }
    Close(listenfd);

	return 0;
}

```

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2017-10-31/High_concurrency_server_for_IO_reuse.gif?raw=true)


## 结语
