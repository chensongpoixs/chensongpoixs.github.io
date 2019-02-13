## Welcome to GitHub Pages

博客的搭建修改自 [Hux](https://github.com/Huxpro/huxpro.github.io), [BY blog](http://qiubaiying.top "BY blog"),    [贾鹏辉](https://github.com/crazycodeboy/crazycodeboy.github.io "贾鹏辉")

### 一, 致谢

1. 这个模板是从这里 Hux fork 的,   感谢 Hux, By blog, 贾鹏辉 这三位作者。
2. 感谢 Jekyll、Github Pages 和 Bootstrap!


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


### 四, 提交记录

|时间|文件|备注|
|:---|:---|:---|
|2019-01-26|/_includes/head.html|添加对数学表达式的支持|
|2019-01-26|/sitemap.xml|添加boost源码分析的文章Site map|
|2019-01-29|/_post/2017-07-03-C_C++类型转换和异常处理.md|C++异常处理|
|2019-01-29|/_layout/post.html |添加disqus评论功能|
|2019-01-29|/about.html|[统计文章篇数](https://github.com/chensongpoixs/chensongpoixs.github.io/commit/90560f5f4f51aef119b83878645c48bc28bd16d2 "90560f5f")|
|2019-01-30|/_includes, /_layout, /js, /css|文章分类搜索功能|
|2019-01-30|/css/hux-blog.min.css|[字体的颜色和背景颜色](59d55b0f84cb2939fbe6ee24c98a7d2ac31b907e) |
|2019-01-21|/_post/2017-06-16-vim的设置和gdb调试使用.md|gdb调试|
|2019-02-02|/_post/2019-02-02-socket选项.md|socket选项|
|2019-02-10|/_post/2018-02-12-TCP状态转换图.md|IP协议详解部分|
|2019-02-13|/_post/2018-02-12-TCP状态转换图.md|ARP协议原理和DNS协议原理|





