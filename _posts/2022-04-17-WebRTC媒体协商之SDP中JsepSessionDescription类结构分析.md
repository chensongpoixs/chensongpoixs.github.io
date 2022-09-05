
---
layout:     post
title:      WebRTC媒体协商之SDP中JsepSessionDescription类结构分析
subtitle:   SDP、媒体协商
date:       2022-04-17
times:       17::27::54
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - 网络编程
  
---


# WebRTC媒体协商之SDP中JsepSessionDescription类结构分析


@[TOC](WebRTC媒体协商之SDP中JsepSessionDescription类结构分析)

</font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">



<font color='red'>WebRTC专题开嗨鸭 ！！！  </font>
 
 
一、 WebRTC 线程模型

[1、WebRTC中线程模型和常见线程模型介绍](https://chensongpoixs.github.io/2021/12/11/WebRTC%E4%B8%AD%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E5%92%8C%E5%B8%B8%E8%A7%81%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E4%BB%8B%E7%BB%8D/#/)

[2、WebRTC网络PhysicalSocketServer之WSAEventselect模型使用](https://chensongpoixs.github.io/2022/01/02/WebRTC%E7%BD%91%E7%BB%9CPhysicalSocketServer%E4%B9%8BWSAEventselect%E6%A8%A1%E5%9E%8B%E4%BD%BF%E7%94%A8/)

二、 WebRTC媒体协商

[1、WebRTC媒体协商之SDP中JsepSessionDescription类结构分析]()

三、 WebRTC 音频数据采集

[1、WebRTC源码之音频设备播放流程源码分析](https://chensongpoixs.github.io/2022/08/14/WebRTC%E6%BA%90%E7%A0%81%E4%B9%8B%E9%9F%B3%E9%A2%91%E8%AE%BE%E5%A4%87%E6%92%AD%E6%94%BE%E6%B5%81%E7%A8%8B%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/)

[2、WebRTC源码之音频设备的录制流程源码分析](https://chensongpoixs.github.io/2022/08/14/WebRTC%E6%BA%90%E7%A0%81%E4%B9%8B%E9%9F%B3%E9%A2%91%E8%AE%BE%E5%A4%87%E7%9A%84%E5%BD%95%E5%88%B6%E6%B5%81%E7%A8%8B%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/#/)

四、 WebRTC 音频引擎(编解码和3A算法) 

五、 WebRTC 视频数据采集

六、 WebRTC 视频引擎( 编解码)

七、 WebRTC  网络传输

[1、WebRTC的ICE之STUN协议](https://blog.csdn.net/Poisx/article/details/124521731)

[2、WebRTC的ICE之Dtls/SSL/TLSv1.x协议详解](https://chensongpoixs.github.io/2022/05/30/WebRTC%E7%9A%84ICE%E4%B9%8BDtls_SSL_TLSv1.x%E5%8D%8F%E8%AE%AE%E8%AF%A6%E8%A7%A3/)

八、 WebRTC服务质量(Qos)

[1、WebRTC中RTCP协议详解](https://blog.csdn.net/Poisx/article/details/121364934)

[2、WebRTC中RTP协议详解](https://chensongpoixs.github.io/2022/05/29/WebRTC%E4%B8%ADRTP%E5%8D%8F%E8%AE%AE%E8%AF%A6%E8%A7%A3/#/)

[3、WebRTC之NACK、RTX 在什么时机判断丢包发送NACK请求和RTX丢包重传](https://chensongpoixs.github.io/2022/05/30/WebRTC%E4%B9%8BNACK-RTX-%E5%9C%A8%E4%BB%80%E4%B9%88%E6%97%B6%E6%9C%BA%E5%88%A4%E6%96%AD%E4%B8%A2%E5%8C%85%E5%8F%91%E9%80%81NACK%E8%AF%B7%E6%B1%82%E5%92%8CRTX%E4%B8%A2%E5%8C%85%E9%87%8D%E4%BC%A0/)

[4、WebRTC源码之视频质量统计数据的数据结构分析](https://chensongpoixs.github.io/2022/07/26/WebRTC%E6%BA%90%E7%A0%81%E4%B9%8B%E8%A7%86%E9%A2%91%E8%B4%A8%E9%87%8F%E7%BB%9F%E8%AE%A1%E6%95%B0%E6%8D%AE%E4%B8%AD%E5%9F%BA%E7%A1%80%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E5%88%86%E6%9E%90/)

九、 NetEQ

十、 Simulcast与SVC
 
 

# 前言

<font color=#999AAA >WebRTC是音视频行业的标杆， 如果要学习音视频， WebRTC是进入音视频行业最好方法， 里面可以有成熟方案， 例如：音频中3A 算法、网络评估、自适应码流、Simulcast、SVC等等 ， 非常适合刚刚进入音视频行业小伙伴哈^_^  我也是哦， 以后再音视频行业长期打算的小伙伴的学习项目。 里面有大量知识点  </font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

<font color=#999AAA >提示：以下是本篇文章正文内容，下面案例可供参考


![在这里插入图片描述](https://img-blog.csdnimg.cn/47fbe163b9b54209a30be965baeba1d3.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAY2hlbl9zb25nXw==,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)


```javascript
v=0
o=- 2900607855288071062 2 IN IP4 127.0.0.1
s=-
t=0 0

///////////////////////////////////////
///////////ContentGroup ///////////////
// 0 :代表音频轨道 1：代表视频轨道
a=group:BUNDLE 0 1
///////////////////////////////////////

a=extmap-allow-mixed
a=msid-semantic: WMS glIuBfu3Z16JuczMQJqmBsbVl78PysmTooIR

////////////////////////////////////////////////////////////////////////
///////////////////如何确定SDP中的媒体////////////////////////////////////////////////////
// 				 pc->AddTrack(track, streamids);=======>>>>> SDP audio <-> video 
// 				 track->kind() // 获取audio/video 类型
//               
//////////////////////////////////////////////////////////////////////
///  m => RtpTransceiver 
//////////////////////////////////////////////////////////////////////
m=audio 9 UDP/TLS/RTP/SAVPF 111 63 103 104 9 0 8 106 105 13 110 112 113 126
c=IN IP4 0.0.0.0
a=rtcp:9 IN IP4 0.0.0.0


////////////////////////////////////////////////////////

///////////TransportDescription//
/////////////////////////////////////////////
a=ice-ufrag:nB3E
a=ice-pwd:D5H9cpTPTLWPwvtv5vEP5Efp
a=ice-options:trickle
a=ice-lite // ice-lite --半验证  stun 
a=fingerprint:sha-256 1B:AC:67:EF:C1:F7:08:2D:DB:73:71:84:53:ED:18:08:96:61:AF:8D:D3:9A:1F:42:85:A4:71:0D:B0:AB:89:0A
a=setup:actpass
//////////////////////////////////////////////////////////////////////

a=mid:0
a=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level
a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
a=extmap:3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
a=extmap:4 urn:ietf:params:rtp-hdrext:sdes:mid
a=sendrecv
a=msid:glIuBfu3Z16JuczMQJqmBsbVl78PysmTooIR a9b63a87-fd12-49fb-910a-c13827512947
a=rtcp-mux
a=rtpmap:111 opus/48000/2
a=rtcp-fb:111 transport-cc
a=fmtp:111 minptime=10;useinbandfec=1
a=rtpmap:63 red/48000/2
a=fmtp:63 111/111
a=rtpmap:103 ISAC/16000
a=rtpmap:104 ISAC/32000
a=rtpmap:9 G722/8000
a=rtpmap:0 PCMU/8000
a=rtpmap:8 PCMA/8000
a=rtpmap:106 CN/32000
a=rtpmap:105 CN/16000
a=rtpmap:13 CN/8000
a=rtpmap:110 telephone-event/48000
a=rtpmap:112 telephone-event/32000
a=rtpmap:113 telephone-event/16000
a=rtpmap:126 telephone-event/8000
a=ssrc:3645401565 cname:CzCKROnFlEhgxbpP
a=ssrc:3645401565 msid:glIuBfu3Z16JuczMQJqmBsbVl78PysmTooIR a9b63a87-fd12-49fb-910a-c13827512947

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

m=video 9 UDP/TLS/RTP/SAVPF 96 97 127 121 125 107 108 109 124 120 123 119 35 36 98 99 100 101 114 115 116 117 118
c=IN IP4 0.0.0.0 // WebRTC不使用这行
a=rtcp:9 IN IP4 0.0.0.0

/////////////////////////////////
///////////TransportDescription//
////////////////////////////////
// 验证使用的 用户名
a=ice-ufrag:nB3E 
// 验证时使用的： 密码
a=ice-pwd:D5H9cpTPTLWPwvtv5vEP5Efp
a=ice-options:trickle 
a=ice-lite // ice-lite --半验证  stun 
// 两个作用 ： 
// 1. 验证加密算法中途有没有被修改
// 2. 加密数据
a=fingerprint:sha-256 1B:AC:67:EF:C1:F7:08:2D:DB:73:71:84:53:ED:18:08:96:61:AF:8D:D3:9A:1F:42:85:A4:71:0D:B0:AB:89:0A
// 角色 是客户端还是服务器
a=setup:actpass
//////////////////////////////////
a=mid:1
a=extmap:14 urn:ietf:params:rtp-hdrext:toffset
a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
a=extmap:13 urn:3gpp:video-orientation
a=extmap:3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
a=extmap:5 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay
a=extmap:6 http://www.webrtc.org/experiments/rtp-hdrext/video-content-type
a=extmap:7 http://www.webrtc.org/experiments/rtp-hdrext/video-timing
a=extmap:8 http://www.webrtc.org/experiments/rtp-hdrext/color-space
a=extmap:4 urn:ietf:params:rtp-hdrext:sdes:mid
a=extmap:10 urn:ietf:params:rtp-hdrext:sdes:rtp-stream-id
a=extmap:11 urn:ietf:params:rtp-hdrext:sdes:repaired-rtp-stream-id
a=sendrecv
a=msid:glIuBfu3Z16JuczMQJqmBsbVl78PysmTooIR d6f796d6-5df8-4308-bc4a-fd1b37ac24b9
a=rtcp-mux  // 
a=rtcp-rsize
a=rtpmap:96 VP8/90000
a=rtcp-fb:96 goog-remb
a=rtcp-fb:96 transport-cc
a=rtcp-fb:96 ccm fir
a=rtcp-fb:96 nack
a=rtcp-fb:96 nack pli
a=rtpmap:97 rtx/90000
a=fmtp:97 apt=96
a=rtpmap:127 H264/90000
a=rtcp-fb:127 goog-remb
a=rtcp-fb:127 transport-cc
a=rtcp-fb:127 ccm fir
a=rtcp-fb:127 nack
a=rtcp-fb:127 nack pli
a=fmtp:127 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42001f
a=rtpmap:121 rtx/90000
a=fmtp:121 apt=127
a=rtpmap:125 H264/90000
a=rtcp-fb:125 goog-remb
a=rtcp-fb:125 transport-cc
a=rtcp-fb:125 ccm fir
a=rtcp-fb:125 nack
a=rtcp-fb:125 nack pli
a=fmtp:125 level-asymmetry-allowed=1;packetization-mode=0;profile-level-id=42001f
a=rtpmap:107 rtx/90000
a=fmtp:107 apt=125
a=rtpmap:108 H264/90000
a=rtcp-fb:108 goog-remb
a=rtcp-fb:108 transport-cc
a=rtcp-fb:108 ccm fir
a=rtcp-fb:108 nack
a=rtcp-fb:108 nack pli
a=fmtp:108 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f
a=rtpmap:109 rtx/90000
a=fmtp:109 apt=108
a=rtpmap:124 H264/90000
a=rtcp-fb:124 goog-remb
a=rtcp-fb:124 transport-cc
a=rtcp-fb:124 ccm fir
a=rtcp-fb:124 nack
a=rtcp-fb:124 nack pli
a=fmtp:124 level-asymmetry-allowed=1;packetization-mode=0;profile-level-id=42e01f
a=rtpmap:120 rtx/90000
a=fmtp:120 apt=124
a=rtpmap:123 H264/90000
a=rtcp-fb:123 goog-remb
a=rtcp-fb:123 transport-cc
a=rtcp-fb:123 ccm fir
a=rtcp-fb:123 nack
a=rtcp-fb:123 nack pli
a=fmtp:123 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=4d001f
a=rtpmap:119 rtx/90000
a=fmtp:119 apt=123
a=rtpmap:35 H264/90000
a=rtcp-fb:35 goog-remb
a=rtcp-fb:35 transport-cc
a=rtcp-fb:35 ccm fir
a=rtcp-fb:35 nack
a=rtcp-fb:35 nack pli
a=fmtp:35 level-asymmetry-allowed=1;packetization-mode=0;profile-level-id=4d001f
a=rtpmap:36 rtx/90000
a=fmtp:36 apt=35
a=rtpmap:98 VP9/90000
a=rtcp-fb:98 goog-remb
a=rtcp-fb:98 transport-cc
a=rtcp-fb:98 ccm fir
a=rtcp-fb:98 nack
a=rtcp-fb:98 nack pli
a=fmtp:98 profile-id=0
a=rtpmap:99 rtx/90000
a=fmtp:99 apt=98
a=rtpmap:100 VP9/90000
a=rtcp-fb:100 goog-remb
a=rtcp-fb:100 transport-cc
a=rtcp-fb:100 ccm fir
a=rtcp-fb:100 nack
a=rtcp-fb:100 nack pli
a=fmtp:100 profile-id=2
a=rtpmap:101 rtx/90000
a=fmtp:101 apt=100
a=rtpmap:114 H264/90000
a=rtcp-fb:114 goog-remb
a=rtcp-fb:114 transport-cc
a=rtcp-fb:114 ccm fir
a=rtcp-fb:114 nack
a=rtcp-fb:114 nack pli
a=fmtp:114 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=64001f
a=rtpmap:115 rtx/90000
a=fmtp:115 apt=114
a=rtpmap:116 red/90000
a=rtpmap:117 rtx/90000
a=fmtp:117 apt=116
a=rtpmap:118 ulpfec/90000
a=ssrc-group:FID 4197684522 390635467 // StreamParams 
a=ssrc:4197684522 cname:CzCKROnFlEhgxbpP
a=ssrc:4197684522 msid:glIuBfu3Z16JuczMQJqmBsbVl78PysmTooIR d6f796d6-5df8-4308-bc4a-fd1b37ac24b9
a=ssrc:390635467 cname:CzCKROnFlEhgxbpP
a=ssrc:390635467 msid:glIuBfu3Z16JuczMQJqmBsbVl78PysmTooIR d6f796d6-5df8-4308-bc4a-fd1b37ac24b9



):  Received candidate :{
   "candidate" : "candidate:34015052 1 udp 2122194687 192.168.0.109 59726 typ host generation 0 ufrag Z277 network-id 4 network-cost 10",
   "sdpMLineIndex" : 0,
   "sdpMid" : "0"
}
Received candidate :{
   "candidate" : "candidate:3093342881 1 udp 2122260223 169.254.119.31 59725 typ host generation 0 ufrag Z277 network-id 3",
   "sdpMLineIndex" : 0,
   "sdpMid" : "0"
}

 

```

# SDP 宏观的分类

![在这里插入图片描述](https://img-blog.csdnimg.cn/f129b59a15474b9991398a12d4536f00.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAY2hlbl9zb25nXw==,size_20,color_FFFFFF,t_70,g_se,x_16)



![在这里插入图片描述](https://img-blog.csdnimg.cn/d7539fc3596b4bbe8866ac82b51c9f80.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAY2hlbl9zb25nXw==,size_20,color_FFFFFF,t_70,g_se,x_16)



# 一、 JsepSessionDescription类 的SDP总信息结构


# 二、SessionDescription
## 2.1、 ContentGroup
content_name:  bundle 

ContentNames: 0 代表音频 1代表视频

```cpp
a=group:BUNDLE 0 1
```
# 三、ContentInfo 


![在这里插入图片描述](https://img-blog.csdnimg.cn/08644ba4593e468f93e6dd8350ff1a01.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAY2hlbl9zb25nXw==,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)


name

MediaProtocolType : 协议

rejected

bundle_only: 是否同用一个通道

MediaContentDescription:  音视频的的信息


# 三、TransportInfos
## 3.1  TransportDescription
```
enum ConnectionRole {
  CONNECTIONROLE_NONE = 0,
  CONNECTIONROLE_ACTIVE,
  CONNECTIONROLE_PASSIVE,
  CONNECTIONROLE_ACTPASS,
  CONNECTIONROLE_HOLDCONN,
};

 // These are actually ICE options (appearing in the ice-options attribute in
  // SDP).
  // TODO(deadbeef): Rename to ice_options.
  std::vector<std::string> transport_options;
  std::string ice_ufrag;
  std::string ice_pwd; //密码
  IceMode ice_mode;  // 连接服务器时服务器是否进行校验 ， 是否双方都要进行校验
  ConnectionRole connection_role; // 是服务器、客户端， 还是即可以是服务器又可以客户端

  std::unique_ptr<rtc::SSLFingerprint> identity_fingerprint;
```



<font color='red'>a = setup 主要是表示dtls的协商过程中角色的问题，谁是客户端，谁是服务器

<font color='red'>a = setup:actpass 既可以是客户端，也可以是服务器

<font color='red'>a = setup : active 客户端

<font color='red'>a = setup : passive 服务器

<font color='red'>由客户端先发起client hello
</font>


# 四、Candidate

![在这里插入图片描述](https://img-blog.csdnimg.cn/76cc01f22dbc48f3aa197fddfd617973.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAY2hlbl9zb25nXw==,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)

# 总结


[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)

