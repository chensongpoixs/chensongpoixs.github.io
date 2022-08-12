---
layout:     post
title:      WebRTC中RTP协议详解
subtitle:   视频质量(Qos)、RTCP/RTP 
date:       2022-05-29
times:       22::45::01 
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - 网络编程
    - RTC服务质量(Qos)
  
---

 
# WebRTC中RTP协议详解


@[TOC](WebRTC中RTP协议详解)

</font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">



<font color='red'>WebRTC专题开嗨鸭 ！！！  </font>
 
一、 WebRTC 线程模型

  [1、WebRTC中线程模型和常见线程模型介绍](https://chensongpoixs.github.io/2021/12/11/WebRTC%E4%B8%AD%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E5%92%8C%E5%B8%B8%E8%A7%81%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E4%BB%8B%E7%BB%8D/#/)

 [2、WebRTC网络PhysicalSocketServer之WSAEventselect模型使用](https://chensongpoixs.github.io/2022/01/02/WebRTC%E7%BD%91%E7%BB%9CPhysicalSocketServer%E4%B9%8BWSAEventselect%E6%A8%A1%E5%9E%8B%E4%BD%BF%E7%94%A8/)

二、 WebRTC媒体协商




三、 WebRTC 音频数据采集

四、 WebRTC 音频引擎(编解码和3A算法) 

五、 WebRTC 视频数据采集

六、 WebRTC 视频引擎( 编解码)

七、 WebRTC  网络传输

[1、WebRTC的ICE之STUN协议](https://blog.csdn.net/Poisx/article/details/124521731)

[2、WebRTC的ICE之Dtls/SSL/TLSv1.x协议详解](https://blog.csdn.net/Poisx/article/details/124918704)

八、 WebRTC服务质量(Qos)

[1、WebRTC中RTCP协议详解](https://blog.csdn.net/Poisx/article/details/121364934)

[2、WebRTC中RTP协议详解](#)


九、 NetEQ

十、 Simulcast与SVC


## 前言
RTP协议 在流媒体中是传输媒体数据的载体


### 一、RTP/RTCP在协议栈中的位置

![在这里插入图片描述](https://img-blog.csdnimg.cn/77b7a88670de416598ddb4229a80c711.png)

#### 1、 RTP传输流程图

![在这里插入图片描述](https://img-blog.csdnimg.cn/8f12eec946c64c45b4cd3139c5803220.png)
### 二、RTP Header 的结构图

![在这里插入图片描述](https://img-blog.csdnimg.cn/77aefec56e73416987603c9dd2018f4a.png)

#### 1、RTP Header 协议字段解析

v   :  版本
P   :  填充数据 1:代表有填充数据   0: 没有补充数据
X   :  是否有扩展头，1： 有扩展头  0：没有扩展头
 <font color='red'> CC : 这个源那些人产生的 csrc的个数 几个贡献者</font>
M  ：代表视频帧的最后一个帧 1：是视频帧的最后一帧， 0：不是最后一帧视频帧
PT :  代表payloadType， 数据负载类型  例如： 音频：ops：101， 视频：127
Sequence Number: 数据有序
timestamp： 产生时间戳
ssrc：源（多路视频流ssrc判断）
csrc：数据有几个贡献者


#### 2、 视频帧分包

![在这里插入图片描述](https://img-blog.csdnimg.cn/a5c2b4e50c22419f8409357d64a26762.png)



序号1~3:为一个完整视频帧（I、P、B）

s：的意思是视频一帧分包 开始位置 e是结束一帧标志位

#### 3、 RTP的扩展头

![在这里插入图片描述](https://img-blog.csdnimg.cn/d7c4432559b842e1b83625ab975dfa2b.png)

##### ①、 扩展头Profile 分为两种类型

profile：占用两个字节 

两种类型分别是下载
1. profile值为<font color='red'>0XBE、0XDE</font>、扩展项的ID和len占一个字节
2. profile值为<font color='red'>0X10、0X0X</font>，扩展项的ID和len占两个字节

##### ②、扩展头length

表示后面跟着的扩展头有几个字节
扩展头长度以4字节为单位

如length为3，说明扩展头长度占12字节

<font color='red'>1.  一个字节扩展头如图所示</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/4e2ff968f82a401b8d5ec8ab572c7c24.png)

![2.](https://img-blog.csdnimg.cn/1e3b19ec804646fe88ea83a6ba6ba2ea.png)
 
 <font color='red'> 2. 两个字节的扩展头例子</font>
 
![在这里插入图片描述](https://img-blog.csdnimg.cn/4e2aadb450914ffdaa241f635c5244f1.png)

### 三、WebRTC用到的扩展头

![在这里插入图片描述](https://img-blog.csdnimg.cn/da34832c2a2a440c908dc4543b45d7fa.png)

#### 1. WebRTC 中扩展头详细信息对应代码与声明

src/api/rtp_paramerters.h 中的RtpExtension结构 
src/api/rtp_parameters.cc 中实现
src/modules/rtp_rtcp/source/rtp_header_extensions.cc 格式详细

### 四、 完整的RTP头

![在这里插入图片描述](https://img-blog.csdnimg.cn/4eb6c1fbc0074be9b3336c08fba4d120.png)

## 总结：

[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)
