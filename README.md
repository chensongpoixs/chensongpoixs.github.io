## Welcome to GitHub Pages

### 一, 博客使用

/_post/


```
---
layout:     post
title:      网络协议详解
subtitle:   ARP协议，DNS协议，IP协议，TCP协议，IP路由和TCP状态转换图
date:       2018-02-12
times:      23::09::56
author:     chensong
header-img: img/2018-02-12/bg_tcp.jpg
catalog: 	 true
tags:
    - TCP/IP协议详解
---

```


### 二, 遇到一些抗

#### 1,  博客中格式 

正确

```
---
layout:     post
title:      Linux网络编程(epoll的边缘触发和水平触发)多路IO高并发服务器
subtitle:   epoll的两种模式-LT模式和高速度ET模式
date:       2017-11-01
author:     chensong
header-img: img/2017-11-01/bg-epoll.jpg
catalog: 		true
tags:
    - 网络编程
---

```

错误

```
---
layout:     post
title:      Linux网络编程(epoll的边缘触发和水平触发)多路IO高并发服务器
subtitle:   epoll的两种模式-LT模式和高速度ET模式
date:       2017-11-01
author:     chensong
header-img: img/2017-11-01/bg-epoll.jpg
catalog: 		true
tags:
	- 网络编程
---

```

错误使用tab键   在tags中不能使用tab键 , 否则找不到网页

|提交|版本|
|:---:|:---:|
|错误提交是|[df120f0](https://github.com/chensongpoixs/chensongpoixs.github.io/commit/df120f078c9c0dc1c22db130f3df6e992a7fc445 "df120f0")|
|正确提交是|[7f6e773](https://github.com/chensongpoixs/chensongpoixs.github.io/commit/7f6e7736f680234f538463614adc3f0ce2d5b3d4 "7f6e773")|



#### 2， 添加 disqus评论功能

问题是加载不了 ---> 要翻墙

>We were unable to load Disqus. If you are a moderator please see our troubleshooting guide.


是配置没有配置对

_config.yal文件中


```
# 评论系统
# Disqus（https://disqus.com/）
disqus_username: chensongpoixs
```

配置不对

[https://disqus.com/admin/create/ ](https://disqus.com/admin/create/  "到官网")


![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2019-01-29/disqus_username.png?raw=true)


应该配置： chensongpoixs-github-io-1

正确的配置

```
# 评论系统
# Disqus（https://disqus.com/）
disqus_username: chensongpoixs-github-io-1
```

效果图：

![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2019-01-29/disqus_username_url.png?raw=true)






