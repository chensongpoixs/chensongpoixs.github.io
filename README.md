## Welcome to GitHub Pages

博客的搭建修改自 Hux， By blog, 贾鹏辉

更为详细的教程戳这 《利用 GitHub Pages 快速搭建个人博客》 或 wiki 

### 致谢

1. 这个模板是从这里 Hux fork 的,   感谢 Hux, By blog, 贾鹏辉 这三位作者。
2. 感谢 Jekyll、Github Pages 和 Bootstrap!


### 遇到一些抗

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