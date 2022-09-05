---
layout:     post
title:      WebRTC中RTCP协议详解
subtitle:   WebRTC、RTCP、Fackback
date:       2021-11-16
times:       23::50::33 
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - AEC
  
---


# WebRTC中RTCP协议详解


@[TOC](WebRTC中RTCP协议详解)

</font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">


<font color='red'>WebRTC中RTCP协议详解  </font>


<font color='red'>WebRTC专题开嗨鸭 ！！！  </font>
 

 
一、 WebRTC 线程模型

[1、WebRTC中线程模型和常见线程模型介绍](https://chensongpoixs.github.io/2021/12/11/WebRTC%E4%B8%AD%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E5%92%8C%E5%B8%B8%E8%A7%81%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E4%BB%8B%E7%BB%8D/#/)

[2、WebRTC网络PhysicalSocketServer之WSAEventselect模型使用](https://chensongpoixs.github.io/2022/01/02/WebRTC%E7%BD%91%E7%BB%9CPhysicalSocketServer%E4%B9%8BWSAEventselect%E6%A8%A1%E5%9E%8B%E4%BD%BF%E7%94%A8/)

二、 WebRTC媒体协商

[1、WebRTC媒体协商之SDP中JsepSessionDescription类结构分析](https://chensongpoixs.github.io/2022/04/17/WebRTC%E5%AA%92%E4%BD%93%E5%8D%8F%E5%95%86%E4%B9%8BSDP%E4%B8%ADJsepSessionDescription%E7%B1%BB%E7%BB%93%E6%9E%84%E5%88%86%E6%9E%90/)

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

[1、WebRTC中RTCP协议详解](https://chensongpoixs.github.io/2021/11/16/WebRTC%E4%B8%ADRTCP%E5%8D%8F%E8%AE%AE%E8%AF%A6%E8%A7%A3/)

[2、WebRTC中RTP协议详解](https://chensongpoixs.github.io/2022/05/29/WebRTC%E4%B8%ADRTP%E5%8D%8F%E8%AE%AE%E8%AF%A6%E8%A7%A3/#/)

[3、WebRTC之NACK、RTX 在什么时机判断丢包发送NACK请求和RTX丢包重传](https://chensongpoixs.github.io/2022/05/30/WebRTC%E4%B9%8BNACK-RTX-%E5%9C%A8%E4%BB%80%E4%B9%88%E6%97%B6%E6%9C%BA%E5%88%A4%E6%96%AD%E4%B8%A2%E5%8C%85%E5%8F%91%E9%80%81NACK%E8%AF%B7%E6%B1%82%E5%92%8CRTX%E4%B8%A2%E5%8C%85%E9%87%8D%E4%BC%A0/)

[4、WebRTC源码之视频质量统计数据的数据结构分析](https://chensongpoixs.github.io/2022/07/26/WebRTC%E6%BA%90%E7%A0%81%E4%B9%8B%E8%A7%86%E9%A2%91%E8%B4%A8%E9%87%8F%E7%BB%9F%E8%AE%A1%E6%95%B0%E6%8D%AE%E4%B8%AD%E5%9F%BA%E7%A1%80%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E5%88%86%E6%9E%90/)

九、 NetEQ

十、 Simulcast与SVC
 

# 前言

<font color=#999AAA >RTCP包 </font>


![在这里插入图片描述](https://img-blog.csdnimg.cn/cbabdffd58ef4c11afaa0dd3f1bc99bb.png)


一共1500字节（网络最大包的大小）
1. Mac Header：14字节
2. Ip Header ： 20字节
3. UDP Header：8字节
4. Data： 可变长度
5. Mac Tailer：4字节



<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

<font color=#999AAA >提示：以下是本篇文章正文内容，下面案例可供参考

 



## 一、RTCP Header   头结构
![在这里插入图片描述](https://img-blog.csdnimg.cn/4edfdd394fe14045ad851fe9496cfa3b.png)

### 1、 RTCP Header 对应的数据结构

```
/* Struct for RTCP common header. */
			struct CommonHeader
			{
#if defined(MS_LITTLE_ENDIAN)
				uint8_t count : 5;  // 一个包中Report Block个数
				uint8_t padding : 1;// 填充标识， 最后一个填充字节是（）个数
				uint8_t version : 2;
#elif defined(MS_BIG_ENDIAN)
				uint8_t version : 2;
				uint8_t padding : 1;
				uint8_t count : 5;
#endif
				uint8_t packetType : 8; // 不同RTCP包的类型
				uint16_t length : 16;   // 16位，包长度（包括头）。[数值为（N-1）个4字节]
			};
```

### 2、 RTCP Header 说明

![在这里插入图片描述](https://img-blog.csdnimg.cn/e50a99248c724ce8b58d9cab955b6ecc.png)

### 3、 RTCP Header 在代码中解析的流程的为代码

```cpp
Packet* Packet::Parse(const uint8_t* data, size_t len)
		{
			MS_TRACE();

			// First, Currently parsing and Last RTCP packets in the compound packet.
			Packet* first{ nullptr };
			Packet* current{ nullptr };
			Packet* last{ nullptr };

			while (len > 0u)
			{
				if (!Packet::IsRtcp(data, len))
				{
					MS_WARN_TAG(rtcp, "data is not a RTCP packet");

					return first;
				}

				auto* header     = const_cast<CommonHeader*>(reinterpret_cast<const CommonHeader*>(data));
				// 这一步 ？？？ 是不是很有问题啊     ========>  数据的 要加上头的大小的哈
				size_t packetLen = static_cast<size_t>(ntohs(header->length) + 1) * 4;

				if (len < packetLen)
				{
					MS_WARN_TAG(
					  rtcp,
					  "packet length exceeds remaining data [len:%zu, "
					  "packet len:%zu]",
					  len,
					  packetLen);

					return first;
				}

				switch (Type(header->packetType))
				{
					case Type::SR:
					{
						// 1. PT= 200 发送反馈包
						current = SenderReportPacket::Parse(data, packetLen);

						if (!current)
							break;

						if (header->count > 0)
						{
							Packet* rr = ReceiverReportPacket::Parse(data, packetLen, current->GetSize());

							if (!rr)
								break;

							current->SetNext(rr);
						}

						break;
					}

					case Type::RR:
					{
						//2. PT = 201  接受多少包发送给对端
						current = ReceiverReportPacket::Parse(data, packetLen);

						break;
					}

					case Type::SDES:
					{
						// 3. PT = 202  对媒体源的描述
						current = SdesPacket::Parse(data, packetLen);

						break;
					}

					case Type::BYE:
					{
						// 4. PT = 203 不需要传输的数据
						current = ByePacket::Parse(data, packetLen);

						break;
					}

					case Type::APP:
					{
						// 5. PT = 204 应用自定义信息
						current = nullptr;

						break;
					}

					case Type::RTPFB:
					{
						// 6. PT = 205 反馈信息
						current = FeedbackRtpPacket::Parse(data, packetLen);

						break;
					}

					case Type::PSFB:
					{
						// 7. PT= 206  负载情况 反馈信息
						current = FeedbackPsPacket::Parse(data, packetLen);

						break;
					}

					case Type::XR:
					{
						// 8. PT = 207  扩展头
						current = ExtendedReportPacket::Parse(data, packetLen);

						break;
					}

					default:
					{
						MS_WARN_TAG(rtcp, "unknown RTCP packet type [packetType:%" PRIu8 "]", header->packetType);

						current = nullptr;
					}
				}

				if (!current)
				{
					std::string packetType = Type2String(Type(header->packetType));

					if (Type(header->packetType) == Type::PSFB)
					{
						packetType +=
						  " " + FeedbackPsPacket::MessageType2String(FeedbackPs::MessageType(header->count));
					}
					else if (Type(header->packetType) == Type::RTPFB)
					{
						packetType +=
						  " " + FeedbackRtpPacket::MessageType2String(FeedbackRtp::MessageType(header->count));
					}

					MS_WARN_TAG(rtcp, "error parsing %s Packet", packetType.c_str());

					return first;
				}

				data += packetLen;
				len -= packetLen;

				if (!first)
					first = current;
				else
					last->SetNext(current);

				last = current->GetNext() != nullptr ? current->GetNext() : current;
			}

			return first;
		}

```

## 二、 RTCP  有哪些 Type类型 


![在这里插入图片描述](https://img-blog.csdnimg.cn/67d9924e46984f10b742ef09b79d411a.png)

### 1、 RTCP SR

![在这里插入图片描述](https://img-blog.csdnimg.cn/fc3631584a0143d692c30379a0a74890.png)

#### ①、 Sender Information block
![在这里插入图片描述](https://img-blog.csdnimg.cn/bbc4d35c4fca4a90a2a2b6809970cb21.png)

#### ②、SR 数据结构
```cpp
/* Struct for RTCP sender report. */
			struct Header
			{
				uint32_t ssrc;          // 源
				uint32_t ntpSec;        // 网络时间戳， 用于不同源之间的同步 
				uint32_t ntpFrac;
				uint32_t rtpTs;        // 相于时间戳，于RTP包时间戳一致
				uint32_t packetCount;  //总发送包数
				uint32_t octetCount;  // 总发送的数据量
			};

```
#### ③、 接受端的处理

```cpp
void RtpStreamSend::ReceiveRtcpReceiverReport(RTC::RTCP::ReceiverReport* report)
	{
		MS_TRACE();

		/* Calculate RTT. */

		// Get the NTP representation of the current timestamp.
		uint64_t nowMs = DepLibUV::GetTimeMs();
		auto ntp       = Utils::Time::TimeMs2Ntp(nowMs);

		// Get the compact NTP representation of the current timestamp.
		uint32_t compactNtp = (ntp.seconds & 0x0000FFFF) << 16;

		compactNtp |= (ntp.fractions & 0xFFFF0000) >> 16;

		uint32_t lastSr = report->GetLastSenderReport();
		uint32_t dlsr   = report->GetDelaySinceLastSenderReport();

		// RTT in 1/2^16 second fractions.
		uint32_t rtt{ 0 };

		// If no Sender Report was received by the remote endpoint yet, ignore lastSr
		// and dlsr values in the Receiver Report.
		if (lastSr && dlsr && (compactNtp > dlsr + lastSr))
			rtt = compactNtp - dlsr - lastSr;

		// RTT in milliseconds.
		this->rtt = static_cast<float>(rtt >> 16) * 1000;
		this->rtt += (static_cast<float>(rtt & 0x0000FFFF) / 65536) * 1000;

		if (this->rtt > 0.0f)
			this->hasRtt = true;

		this->packetsLost  = report->GetTotalLost();
		this->fractionLost = report->GetFractionLost();

		// Update the score with the received RR.
		UpdateScore(report);
	}
```

### 2、Receiver report block

![在这里插入图片描述](https://img-blog.csdnimg.cn/1c1df124fe134739ae15c2439d25f1fe.png)
### 3、 RTCP SDES 


包含CNAME项的SDES包必须包含在美国组合RTCP包中。SDES包可能包括其他源描述项，这要根据特别的应用需要，并同时考虑带宽限制

![在这里插入图片描述](https://img-blog.csdnimg.cn/4e9247f507d74ec5ac7d93ddced12e78.png)
#### ①、 SDES说明




1、SC： SSRC/CSRC数量
2、Item： 采用TLV存放数据
3、 CNAME： SSRC的规范名

#### ②、 SDES item
![在这里插入图片描述](https://img-blog.csdnimg.cn/43f00f579f3a40caba5ba2290a69e12e.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/1e0a68e9b8724a1ca1d72baac6a87bef.png)
#### ③、 WebRTC中使用SDES
1. 媒体协商时，为每个源（SSRC）设置了一个CNAME


```cpp
....
a=ssrc:3121455836 cname:UrMYhDY/S2YpnPSN
....
```
2. 通过RTCP SDES 确认CNAME与SSRC的关系



### 4、RTCP BYE

不需要ssrc流了 就写ssrc放到ssrc这边  发送方就不会发送哈

![在这里插入图片描述](https://img-blog.csdnimg.cn/de3db691ffd44f8caf261cc12cb92674.png)


### 5、 RTCP APP

应用的信息

![在这里插入图片描述](https://img-blog.csdnimg.cn/5523806fab1347d890e103df81a70952.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2b50683714454fb7a2a897ee4db7313e.png)
### 6、 RTCP FB Type 

![在这里插入图片描述](https://img-blog.csdnimg.cn/a5b3a70fd6b1402f8829e8e903708576.png)
### 7、 RTCP  RTPFB Type

![在这里插入图片描述](https://img-blog.csdnimg.cn/f46f4cfbbdf34d16b1ae99e960f039d1.png)
### 8、 RTCP PSFB Type 

![在这里插入图片描述](https://img-blog.csdnimg.cn/e4b64fb228b94d7685dd2c72d55aee1e.png)
### 9、 RTCP FB Type
![在这里插入图片描述](https://img-blog.csdnimg.cn/1d2d788893934a54b35a2d620252f8f9.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/afe077fc329346798347b04a047f5036.png)


# 总结：

[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)


