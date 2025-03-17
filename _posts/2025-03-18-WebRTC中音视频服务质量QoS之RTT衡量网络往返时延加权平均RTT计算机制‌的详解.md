---
layout:     post
title:      WebRTC中音视频服务质量QoS之RTT衡量网络往返时延加权平均RTT计算机制‌的详解
subtitle:   WebRTC、音视频服务质量QoS、RTT
date:       2025-03-18
times:      01::54::35
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - JitterBuffer
    - RTT
---




# WebRTC中音视频服务质量QoS之RTT衡量网络往返时延加权平均RTT计算机制‌的详解


@[TOC](WebRTC中音视频服务质量QoS之RTT衡量网络往返时延加权平均RTT计算机制‌的详解)

</font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">


<font color='red'>WebRTC专题开嗨鸭 ！！！  </font>
 

一、 WebRTC 线程模型

[1、WebRTC中线程模型和常见线程模型介绍](https://chensongpoixs.github.io/2021/12/11/WebRTC%E4%B8%AD%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E5%92%8C%E5%B8%B8%E8%A7%81%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E4%BB%8B%E7%BB%8D/#/)

[2、WebRTC网络PhysicalSocketServer之WSAEventselect模型使用](https://chensongpoixs.github.io/2022/01/02/WebRTC%E7%BD%91%E7%BB%9CPhysicalSocketServer%E4%B9%8BWSAEventselect%E6%A8%A1%E5%9E%8B%E4%BD%BF%E7%94%A8/)



二、 WebRTC媒体协商

[1、WebRTC媒体协商之SDP中JsepSessionDescription类结构分析](https://chensongpoixs.github.io/2022/04/17/WebRTC%E5%AA%92%E4%BD%93%E5%8D%8F%E5%95%86%E4%B9%8BSDP%E4%B8%ADJsepSessionDescription%E7%B1%BB%E7%BB%93%E6%9E%84%E5%88%86%E6%9E%90/)

[2、WebRTC媒体协商之CreatePeerConnectionFactory、CreatePeerConnection、CreateOffer](https://chensongpoixs.github.io/2022/01/09/WebRTC%E5%AA%92%E4%BD%93%E5%8D%8F%E5%95%86%E4%B9%8BCreatePeerConnectionFactory-CreatePeerConnection-CreateOffer/)

[3、WebRTC之证书(certificate)生成的时机分析](https://chensongpoixs.github.io/2022/10/04/WebRTC%E4%B9%8B%E8%AF%81%E4%B9%A6certificate%E7%94%9F%E6%88%90%E7%9A%84%E6%97%B6%E6%9C%BA%E5%88%86%E6%9E%90/#/)

[4、WebRTC源码之RtpTransceiver添加视频轨道的AddTrack函数中桥接模式的流程分析]()

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

[5、WebRTC源码之RTCPReceiver源码分析]()

[6、WebRTC中音视频服务质量QoS之RTT衡量网络往返时延加权平均RTT计算机制‌的详解]()


九、 NetEQ

十、 Simulcast与SVC


# 前言

# 一、   RTT 网络往返时延的原理‌

WebRTC 提供 ‌两种 RTT 计算模式‌，适应不同传输场景

## 1、基于发送端（SR/RR 模式）
   
  ***  <font color='red'>触发条件‌： 发送端周期性发送 ‌Sender Report (SR)‌，接收端回应 ‌Receiver Report (RR)‌‌ </font>***
   

### ①. ‌基本定义‌
		
		‌DLSR‌ 表示自接收端最后一次收到发送端 Sender Report (SR) 到生成当前 Receiver Report (RR) 的时间间隔，单位为 ‌1/65536 秒‌‌1。
		若接收端未收到过 SR 报文，则 DLSR 值为零‌1。
		
### ②. ‌计算 RTT 网络往返时延的原理‌
		
		在端到端通信中（以端点 A 和 B 为例）：

		‌A 发送 SR‌：记录发送时间 t1（即 LSR，Last SR Timestamp）‌2。
		‌B 接收 SR‌：记录接收时间 last_recv_time‌2。
		‌B 发送 RR‌：计算从 last_recv_time 到当前时间的延迟（即 DLSR），并附加到 RR 报文‌2。
		‌A 接收 RR‌：根据公式 RTT = 当前时间 - LSR - DLSR 计算往返时间。

公式： ${RTT=T_{current} − T_ {LSR} −  \frac{T_{DLSR}}{65536}}$ (单位：秒)
    
  参数说明‌：
 
​
$T_ {LSR}$ ：发送端最后一次 SR 的 NTP 时间戳（中间 32 位）‌3。
‌$T_{DLSR‌}$：接收端处理 SR 到生成 RR 的延迟（单位：1/65536 秒）‌

### ③ 发送 Sender Report  (SR) 协议

#### SenderReport 协议的格式 

```javascript

//    Sender report (SR) (RFC 3550).
//     0                   1                   2                   3
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |V=2|P|    RC   |   PT=SR=200   |             length            |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  0 |                         SSRC of sender                        |
//    +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//  4 |              NTP timestamp, most significant word             |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  8 |             NTP timestamp, least significant word             |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 12 |                         RTP timestamp                         |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 16 |                     sender's packet count                     |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 20 |                      sender's octet count                     |
// 24 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
```
 
 #### 组织SR协议

```javascript

std::unique_ptr<rtcp::RtcpPacket> RTCPSender::BuildSR(const RtcpContext& ctx) {
  // Timestamp shouldn't be estimated before first media frame.
  RTC_DCHECK_GE(last_frame_capture_time_ms_, 0);
  // The timestamp of this RTCP packet should be estimated as the timestamp of
  // the frame being captured at this moment. We are calculating that
  // timestamp as the last frame's timestamp + the time since the last frame
  // was captured.
  int rtp_rate = rtp_clock_rates_khz_[last_payload_type_];
  if (rtp_rate <= 0) {
    rtp_rate =
        (audio_ ? kBogusRtpRateForAudioRtcp : kVideoPayloadTypeFrequency) /
        1000;
  }
  // Round now_us_ to the closest millisecond, because Ntp time is rounded
  // when converted to milliseconds,
  uint32_t rtp_timestamp =
      timestamp_offset_ + last_rtp_timestamp_ +
      ((ctx.now_us_ + 500) / 1000 - last_frame_capture_time_ms_) * rtp_rate;

  rtcp::SenderReport* report = new rtcp::SenderReport();
  report->SetSenderSsrc(ssrc_);
  report->SetNtp(TimeMicrosToNtp(ctx.now_us_));
  report->SetRtpTimestamp(rtp_timestamp);
  report->SetPacketCount(ctx.feedback_state_.packets_sent);
  report->SetOctetCount(ctx.feedback_state_.media_bytes_sent);
  // TODO@chensong  2025-03-15  获取当前发送 
  report->SetReportBlocks(CreateReportBlocks(ctx.feedback_state_));

  return std::unique_ptr<rtcp::RtcpPacket>(report);
}

```


####  SR和RR中都有ReportBlock数据块保存 LSR和DLSR的信息

```javascript

// From RFC 3550, RTP: A Transport Protocol for Real-Time Applications.
//
// RTCP report block (RFC 3550).
//
//     0                   1                   2                   3
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//  0 |                 SSRC_1 (SSRC of first source)                 |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  4 | fraction lost |       cumulative number of packets lost       |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  8 |           extended highest sequence number received           |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 12 |                      interarrival jitter                      |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 16 |                         last SR (LSR)                         |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 20 |                   delay since last SR (DLSR)                  |
// 24 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
```



####  SR和RR中都有ReportBlock协议解析


last_sr_ ：发送端发送时间

delay_since_last_sr_  : 是远端最后接受SR或者RR包的时间

```javascript


bool ReportBlock::Parse(const uint8_t* buffer, size_t length) 
{
  RTC_DCHECK(buffer != nullptr);
  if (length < ReportBlock::kLength)
  {
    RTC_LOG(LS_ERROR) << "Report Block should be 24 bytes long";
    return false;
  }
  // 接收到的媒体源ssrc
  source_ssrc_ = ByteReader<uint32_t>::ReadBigEndian(&buffer[0]);
  // TODO@chensong 2022-10-19  丢包率 fraction_lost
  /**
		TODO@chensong 2023-03-07  
		某时刻收到的有序包的数量Count = transmitted-retransmitte,当前时刻为Count2,上一时刻为Count1;

        接收端以一定的频率发送RTCP包（RR、REMB、NACK等）时，会统计两次发送间隔之间(fraction)的接收包信息。

        接收端发送的RR包中包含两个丢包:

        一个是fraction_lost，是两次统计间隔间的丢包率(以256为基数换算成8bit)。

        一个是cumulative number of packets lost，是总的累积丢包。 
  **/
  fraction_lost_ = buffer[4];
  // 接收开始丢包总数， 迟到包不算丢包，重传有可以导致负数
  cumulative_lost_ = ByteReader<int32_t, 3>::ReadBigEndian(&buffer[5]);
  // 低16位表示收到的最大seq，高16位表示seq循环次数
  extended_high_seq_num_ = ByteReader<uint32_t>::ReadBigEndian(&buffer[8]);
  // rtp包到达时间间隔的统计方差
  jitter_ = ByteReader<uint32_t>::ReadBigEndian(&buffer[12]);
  // ntp时间戳的中间32位
  last_sr_ = ByteReader<uint32_t>::ReadBigEndian(&buffer[16]);
  // 记录上一个接收SR的时间与上一个发送SR的时间差
  delay_since_last_sr_ = ByteReader<uint32_t>::ReadBigEndian(&buffer[20]);

  return true;
}

```

### ④  发送ReceiverReport（RR）协议

#### ReceiverReport协议格式

```javascript

// RTCP receiver report (RFC 3550).
//
//   0                   1                   2                   3
//   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |V=2|P|    RC   |   PT=RR=201   |             length            |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                     SSRC of packet sender                     |
//  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//  |                         report block(s)                       |
//  |                            ....                               |

```

#### 组织 ReceiverReport(RR)数据

在RTCPSender类中BuildRR方法中调用 GetFeedbackState方法获取 ReportBlock数据


调用流程

 
RTCPSender类BuildRR  --->  ModuleRtpRtcpImpl::GetFeedbackState获取  remote_sender_rtp_time_(远端发送时间)和 last_received_sr_ntp_ （最后一次接受时间）
   --->LastReceivedNTP 方法调用NTP方法
   -->RTCPReceiver类NTP 获取 remote_sender_rtp_time_(远端发送时间)和 last_received_sr_ntp_ （最后一次接受时间）
 

```javascript

std::unique_ptr<rtcp::RtcpPacket> RTCPSender::BuildRR(const RtcpContext& ctx) {
  rtcp::ReceiverReport* report = new rtcp::ReceiverReport();
  report->SetSenderSsrc(ssrc_);
   // TODO@chensong 2025-03-15  rtp_rtcp_impl.cc ->  ModuleRtpRtcpImpl::GetFeedbackState
  report->SetReportBlocks(CreateReportBlocks(ctx.feedback_state_));

  return std::unique_ptr<rtcp::RtcpPacket>(report);
}





// TODO(pbos): Handle media and RTX streams separately (separate RTCP
// feedbacks).
RTCPSender::FeedbackState ModuleRtpRtcpImpl::GetFeedbackState() {
  RTCPSender::FeedbackState state;
  // This is called also when receiver_only is true. Hence below
  // checks that rtp_sender_ exists.
  if (rtp_sender_) {
    StreamDataCounters rtp_stats;
    StreamDataCounters rtx_stats;
    rtp_sender_->GetDataCounters(&rtp_stats, &rtx_stats);
    state.packets_sent =
        rtp_stats.transmitted.packets + rtx_stats.transmitted.packets;
    state.media_bytes_sent = rtp_stats.transmitted.payload_bytes +
                             rtx_stats.transmitted.payload_bytes;
    state.send_bitrate = rtp_sender_->BitrateSent();
  }
  state.module = this;
  // TODO@chensong 2025-03-15 获取远端发送信息包时间 和当前最后接收一包记录时间
  LastReceivedNTP(&state.last_rr_ntp_secs, &state.last_rr_ntp_frac,
                  &state.remote_sr);

  state.last_xr_rtis = rtcp_receiver_.ConsumeReceivedXrReferenceTimeInfo();

  return state;
}

bool RTCPReceiver::NTP(uint32_t* received_ntp_secs,
                       uint32_t* received_ntp_frac,
                       uint32_t* rtcp_arrival_time_secs,
                       uint32_t* rtcp_arrival_time_frac,
                       uint32_t* rtcp_timestamp) const {
  rtc::CritScope lock(&rtcp_receiver_lock_);
  if (!last_received_sr_ntp_.Valid()) {
    return false;
  }
  //   TODO@chensong 2025-03-15  last_rr_ntp_frac 发送时间戳
  // NTP from incoming SenderReport.
  if (received_ntp_secs) {
    *received_ntp_secs = remote_sender_ntp_time_.seconds();
  }
  if (received_ntp_frac) {
    *received_ntp_frac = remote_sender_ntp_time_.fractions();
  }

  // Rtp time from incoming SenderReport.
  // TODO@chensong 2025-03-15 远端接受最后一个rtp包的时间
  if (rtcp_timestamp) 
  {
    *rtcp_timestamp = remote_sender_rtp_time_;
  }

  // Local NTP time when we received a RTCP packet with a send block.
  // TODO@chensong 2025-03-15 本地接受最后一个rtcp包的时间
  if (rtcp_arrival_time_secs) {
    *rtcp_arrival_time_secs = last_received_sr_ntp_.seconds();
  }
  if (rtcp_arrival_time_frac) {
    *rtcp_arrival_time_frac = last_received_sr_ntp_.fractions();
  }

  return true;
}
// 接收SenderReport包信息
void RTCPReceiver::HandleSenderReport(const CommonHeader& rtcp_block,
                                      PacketInformation* packet_information) {
  rtcp::SenderReport sender_report;
  if (!sender_report.Parse(rtcp_block)) {
    ++num_skipped_packets_;
    return;
  }

  const uint32_t remote_ssrc = sender_report.sender_ssrc();

  packet_information->remote_ssrc = remote_ssrc;

  UpdateTmmbrRemoteIsAlive(remote_ssrc);

  // Have I received RTP packets from this party?
  if (remote_ssrc_ == remote_ssrc) {
    // Only signal that we have received a SR when we accept one.
    packet_information->packet_type_flags |= kRtcpSr;
	// TODO@chensong 2025-03-15   SR => RR 
    remote_sender_ntp_time_ = sender_report.ntp();
    remote_sender_rtp_time_ = sender_report.rtp_timestamp();
    last_received_sr_ntp_ = TimeMicrosToNtp(clock_->TimeInMicroseconds());
  } else {
    // We will only store the send report from one source, but
    // we will store all the receive blocks.
    packet_information->packet_type_flags |= kRtcpRr;
  }

  for (const rtcp::ReportBlock& report_block : sender_report.report_blocks()) {
    HandleReportBlock(report_block, packet_information, remote_ssrc);
  }
}
```

#### 终止计算rtt往返时延  加权平均RTT计算机制‌

##### 定时计算 WebRTC中默认1秒  

在ModuleRtpRtcpImpl类中Process方法中统计   加权平均RTT计算机制‌



```javascript

// Process any pending tasks such as timeouts (non time critical events).
void ModuleRtpRtcpImpl::Process() {
  const int64_t now = clock_->TimeInMilliseconds();
  next_process_time_ = now + kRtpRtcpMaxIdleTimeProcessMs;

  if (rtp_sender_) {
    if (now >= last_bitrate_process_time_ + kRtpRtcpBitrateProcessTimeMs) {
      rtp_sender_->ProcessBitrate();
      last_bitrate_process_time_ = now;
      next_process_time_ =
          std::min(next_process_time_, now + kRtpRtcpBitrateProcessTimeMs);
    }
  }

  bool process_rtt = now >= last_rtt_process_time_ + kRtpRtcpRttProcessTimeMs;
  if (rtcp_sender_.Sending()) {
    // Process RTT if we have received a report block and we haven't
    // processed RTT for at least |kRtpRtcpRttProcessTimeMs| milliseconds.
    if (rtcp_receiver_.LastReceivedReportBlockMs() > last_rtt_process_time_ &&
        process_rtt) {
      std::vector<RTCPReportBlock> receive_blocks;
      rtcp_receiver_.StatisticsReceived(&receive_blocks);
      int64_t max_rtt = 0;
      for (std::vector<RTCPReportBlock>::iterator it = receive_blocks.begin();
           it != receive_blocks.end(); ++it) {
        int64_t rtt = 0;
        rtcp_receiver_.RTT(it->sender_ssrc, &rtt, NULL, NULL, NULL);
        max_rtt = (rtt > max_rtt) ? rtt : max_rtt;
      }
      // Report the rtt.
      if (rtt_stats_ && max_rtt != 0)
        rtt_stats_->OnRttUpdate(max_rtt);
    }

    // Verify receiver reports are delivered and the reported sequence number
    // is increasing.
    if (rtcp_receiver_.RtcpRrTimeout()) {
      RTC_LOG_F(LS_WARNING) << "Timeout: No RTCP RR received.";
    } else if (rtcp_receiver_.RtcpRrSequenceNumberTimeout()) {
      RTC_LOG_F(LS_WARNING) << "Timeout: No increase in RTCP RR extended "
                               "highest sequence number.";
    }

    if (remote_bitrate_ && rtcp_sender_.TMMBR()) {
      unsigned int target_bitrate = 0;
      std::vector<unsigned int> ssrcs;
      if (remote_bitrate_->LatestEstimate(&ssrcs, &target_bitrate)) {
        if (!ssrcs.empty()) {
          target_bitrate = target_bitrate / ssrcs.size();
        }
        rtcp_sender_.SetTargetBitrate(target_bitrate);
      }
    }
  } else {
    // Report rtt from receiver.
    if (process_rtt) {
      int64_t rtt_ms;
      if (rtt_stats_ && rtcp_receiver_.GetAndResetXrRrRtt(&rtt_ms)) {
        rtt_stats_->OnRttUpdate(rtt_ms);
      }
    }
  }

  // Get processed rtt.
  if (process_rtt) {
    last_rtt_process_time_ = now;
    next_process_time_ = std::min(
        next_process_time_, last_rtt_process_time_ + kRtpRtcpRttProcessTimeMs);
    if (rtt_stats_) 
	{
		// TODO@chensong 2025-03-15  1秒更新一次 rtt    公式
    
		  /*
  TODO@chensong 2025-03-15 
  加权平均RTT计算机制‌
	在实时通信场景（如WebRTC）中，RTT（往返时延）的平滑计算对网络状态感知和拥塞控制至关重要。通过 ‌加权移动平均（Weighted Moving Average）‌ 
	对RTT值进行动态调整，可有效平衡历史数据与实时测量值的影响，抑制短期波动带来的干扰。以下是核心实现逻辑：

	‌1. 公式定义‌
	‌计算方式‌：
	新平均RTT由 ‌历史平均值（old_avg）‌ 与 ‌最新测量值（new_sample）‌ 按权重合成，公式为：

	text
	Copy Code
	avg_rtt = 0.7 * old_avg + 0.3 * new_sample  
	其中，历史数据权重为70%（0.7），新样本权重为30%（0.3）‌23。

	‌数学意义‌：

	‌旧值主导（70%）‌：确保长期趋势稳定，避免偶发延迟突变（如网络抖动）对整体估计的过度影响‌23。
	‌新值补充（30%）‌：快速响应网络状态的渐进变化（如带宽增减或路由切换）‌
  */
      // Make sure we have a valid RTT before setting.
      int64_t last_rtt = rtt_stats_->LastProcessedRtt();
      if (last_rtt >= 0)
        set_rtt_ms(last_rtt);
    }
  }

  if (rtcp_sender_.TimeToSendRTCPReport())
    rtcp_sender_.SendRTCP(GetFeedbackState(), kRtcpReport);

  if (TMMBR() && rtcp_receiver_.UpdateTmmbrTimers()) {
    rtcp_receiver_.NotifyTmmbrUpdated();
  }
}
```




## 2、基于接收端（RTCP XR 模式）

 ***  <font color='red'>触发条件‌：接收端仅拉流（不发送媒体数据），通过 ‌RTCP Extended Reports (XR)‌ 扩展协议实现 RTT 探测‌ </font>

实现步骤‌：

1. 网关发送 ‌RRTR 报文‌（含 NTP 时间戳 $T_{RRTR}$)
2. 接收端回复 ‌DLRR 报文‌，包含 
     - 原$T_{RRTR}$ (即为LRR)
     - 处理延迟$T_{DLSR}$(接收 RRTR 到发送 DLRR 的时间)
3. 网关计算公式

	  $RTT = {T_{current}}$   - ${T_{TRR}}$ - ${T_{DLSR}}$


# 二、WebRTC中网络服务质量Qos之网络波动时延加权平均RTT计算机制‌

 




时延加权平均RTT计算机制‌在实时通信场景（如WebRTC）中，RTT（往返时延）的平滑计算对网络状态感知和拥塞控制至关重要。通过 ‌加权移动平均（Weighted Moving Average）对RTT值进行动态调整，可有效平衡历史数据与实时测量值的影响，抑制短期波动带来的干扰。以下是核心实现逻辑：

## 1. 时延加权平均RTT计算公式
	
计算方式‌： 新平均RTT由 ‌历史平均值（old_avg）‌ 与 ‌最新测量值（new_sample）‌ 按权重合成，公式为：

 
${avgrtt} = 0.7 * {oldavg}+ 0.3 * {newsample}$ 
	
其中，历史数据权重为70%（0.7），新样本权重为30%（0.3）‌23。

‌数学意义‌：
	
旧值主导（70%）‌：确保长期趋势稳定，避免偶发延迟突变（如网络抖动）对整体估计的过度影响‌。
	
‌新值补充（30%）‌：快速响应网络状态的渐进变化（如带宽增减或路由切换）‌


## 2、 WebRTC中时延加权平均RTT的代码实现 


### ① ModuleRtpRtcpImpl 类中定时任务每各1秒钟从所有通道根据RR中rtt值 取最大值  调用统计类（rtt_stats）中OnRttUpdate方法 把rtt保存到reports_成员中这样定义保存网络rtt值 


ModuleRtpRtcpImpl 类的定时任务的实现


```javascript

// Process any pending tasks such as timeouts (non time critical events).
void ModuleRtpRtcpImpl::Process() {
  const int64_t now = clock_->TimeInMilliseconds();
  next_process_time_ = now + kRtpRtcpMaxIdleTimeProcessMs;

  if (rtp_sender_) {
    if (now >= last_bitrate_process_time_ + kRtpRtcpBitrateProcessTimeMs) {
      rtp_sender_->ProcessBitrate();
      last_bitrate_process_time_ = now;
      next_process_time_ =
          std::min(next_process_time_, now + kRtpRtcpBitrateProcessTimeMs);
    }
  }

  bool process_rtt = now >= last_rtt_process_time_ + kRtpRtcpRttProcessTimeMs;
  if (rtcp_sender_.Sending()) {
    // Process RTT if we have received a report block and we haven't
    // processed RTT for at least |kRtpRtcpRttProcessTimeMs| milliseconds.
    if (rtcp_receiver_.LastReceivedReportBlockMs() > last_rtt_process_time_ &&
        process_rtt) 
	{
      std::vector<RTCPReportBlock> receive_blocks;
	  // TODO@chensong 20250317 取所有ssrc通道的根据 RR 中rtt值最大max_rtt信息
      rtcp_receiver_.StatisticsReceived(&receive_blocks);
      int64_t max_rtt = 0;
      for (std::vector<RTCPReportBlock>::iterator it = receive_blocks.begin();
           it != receive_blocks.end(); ++it) 
	  {
        int64_t rtt = 0;
        rtcp_receiver_.RTT(it->sender_ssrc, &rtt, NULL, NULL, NULL);
        max_rtt = (rtt > max_rtt) ? rtt : max_rtt;
      }
      // Report the rtt.
      if (rtt_stats_ && max_rtt != 0)
      {
        rtt_stats_->OnRttUpdate(max_rtt);
	  }
    }

    // Verify receiver reports are delivered and the reported sequence number
    // is increasing.
    if (rtcp_receiver_.RtcpRrTimeout()) {
      RTC_LOG_F(LS_WARNING) << "Timeout: No RTCP RR received.";
    } else if (rtcp_receiver_.RtcpRrSequenceNumberTimeout()) {
      RTC_LOG_F(LS_WARNING) << "Timeout: No increase in RTCP RR extended "
                               "highest sequence number.";
    }

    if (remote_bitrate_ && rtcp_sender_.TMMBR()) {
      unsigned int target_bitrate = 0;
      std::vector<unsigned int> ssrcs;
      if (remote_bitrate_->LatestEstimate(&ssrcs, &target_bitrate)) {
        if (!ssrcs.empty()) {
          target_bitrate = target_bitrate / ssrcs.size();
        }
        rtcp_sender_.SetTargetBitrate(target_bitrate);
      }
    }
  } else {
    // Report rtt from receiver.
    if (process_rtt) {
      int64_t rtt_ms;
      if (rtt_stats_ && rtcp_receiver_.GetAndResetXrRrRtt(&rtt_ms)) {
        rtt_stats_->OnRttUpdate(rtt_ms);
      }
    }
  }

  // Get processed rtt.
  if (process_rtt) {
    last_rtt_process_time_ = now;
    next_process_time_ = std::min(
        next_process_time_, last_rtt_process_time_ + kRtpRtcpRttProcessTimeMs);
    if (rtt_stats_) 
	{
		// TODO@chensong 2025-03-15  1秒更新一次 rtt    公式
    
		  /*
  TODO@chensong 2025-03-15 
  加权平均RTT计算机制‌
	在实时通信场景（如WebRTC）中，RTT（往返时延）的平滑计算对网络状态感知和拥塞控制至关重要。通过 ‌加权移动平均（Weighted Moving Average）‌ 
	对RTT值进行动态调整，可有效平衡历史数据与实时测量值的影响，抑制短期波动带来的干扰。以下是核心实现逻辑：

	‌1. 公式定义‌
	‌计算方式‌：
	新平均RTT由 ‌历史平均值（old_avg）‌ 与 ‌最新测量值（new_sample）‌ 按权重合成，公式为：

	text
	Copy Code
	avg_rtt = 0.7 * old_avg + 0.3 * new_sample  
	其中，历史数据权重为70%（0.7），新样本权重为30%（0.3）‌23。

	‌数学意义‌：

	‌旧值主导（70%）‌：确保长期趋势稳定，避免偶发延迟突变（如网络抖动）对整体估计的过度影响‌23。
	‌新值补充（30%）‌：快速响应网络状态的渐进变化（如带宽增减或路由切换）‌
  */
      // Make sure we have a valid RTT before setting.
      int64_t last_rtt = rtt_stats_->LastProcessedRtt();
      if (last_rtt >= 0)
        set_rtt_ms(last_rtt);
    }
  }

  if (rtcp_sender_.TimeToSendRTCPReport())
    rtcp_sender_.SendRTCP(GetFeedbackState(), kRtcpReport);

  if (TMMBR() && rtcp_receiver_.UpdateTmmbrTimers()) {
    rtcp_receiver_.NotifyTmmbrUpdated();
  }
}
```

### ② CallStats类中RTT实现

CallStats类是在Call类中创建的然后注册到定时任务中的 该类会注册音频发送流（AudioSendStream）、视频发送流（VideoReceiveStream）、视频接收流（VideoReceiveStream）和 前向纠错接收流（FlexfecReceiveStreamImpl） 


在CallStats类每一秒中计算一次rtt的值步骤：

1. 取出最近1500毫秒中rtt统计的数据
2. 取出第一步获取rtt数据中最大rtt值即max_rtt
3. 调用GetNewAvgRttMs方法计算加权平均RTT值
4. 计算的加权平均RTT值（avg_rtt）和最大rtt(max_rtt)反馈到接受网络带宽控制类中OnRttUpdate方法 进行网络拥塞控制

```javascrpit


void CallStats::Process() 
{
  RTC_DCHECK_RUN_ON(&process_thread_checker_);
  int64_t now = clock_->TimeInMilliseconds();
  last_process_time_ = now;

  int64_t avg_rtt_ms = avg_rtt_ms_;
  // TODO@chensong 2025-03-17 1. 清除大约kRttTimeoutMs （1500ms）的数据
  RemoveOldReports(now, &reports_);
  // TODO@chensong 2025-03-17 2. kRttTimeoutMs 最大  rtt
  max_rtt_ms_ = GetMaxRttMs(reports_);
  // TODO@chensong 20250317  3. 得到加权平均RTT值
  avg_rtt_ms = GetNewAvgRttMs(reports_, avg_rtt_ms);
  {
    rtc::CritScope lock(&avg_rtt_ms_lock_);
    avg_rtt_ms_ = avg_rtt_ms;
  }

  // If there is a valid rtt, update all observers with the max rtt.
  if (max_rtt_ms_ >= 0) 
  {
    RTC_DCHECK_GE(avg_rtt_ms, 0);
	// TODO@chensong 2022-12-20 observers_是什么时候的创建的 需要跟一下  track
	// TODO@chensong 20250317  4.  call类中ReceiveSideCongestionController 成员类中方法OnRttUpdate方法 进行网络拥塞控制
	for (CallStatsObserver* observer : observers_)
	{
      observer->OnRttUpdate(avg_rtt_ms, max_rtt_ms_);
	}
    // Sum for Histogram of average RTT reported over the entire call.
    sum_avg_rtt_ms_ += avg_rtt_ms;
    ++num_avg_rtt_;
  }
}


int64_t GetNewAvgRttMs(const std::list<CallStats::RttTime>& reports,
                       int64_t prev_avg_rtt) {
  if (reports.empty())
    return -1;  // Reset (invalid average).

  int64_t cur_rtt_ms = GetAvgRttMs(reports);
  if (prev_avg_rtt == -1)
    return cur_rtt_ms;  // New initial average value.

  // Weight factor to apply to the average rtt.
  // We weigh the old average at 70% against the new average (30%).
  /*
  TODO@chensong 2025-03-15 
  加权平均RTT计算机制‌
	在实时通信场景（如WebRTC）中，RTT（往返时延）的平滑计算对网络状态感知和拥塞控制至关重要。通过 ‌加权移动平均（Weighted Moving Average）‌ 
	对RTT值进行动态调整，可有效平衡历史数据与实时测量值的影响，抑制短期波动带来的干扰。以下是核心实现逻辑：

	‌1. 公式定义‌
	‌计算方式‌：
	新平均RTT由 ‌历史平均值（old_avg）‌ 与 ‌最新测量值（new_sample）‌ 按权重合成，公式为：

	text
	Copy Code
	avg_rtt = 0.7 * old_avg + 0.3 * new_sample  
	其中，历史数据权重为70%（0.7），新样本权重为30%（0.3）‌23。

	‌数学意义‌：

	‌旧值主导（70%）‌：确保长期趋势稳定，避免偶发延迟突变（如网络抖动）对整体估计的过度影响‌23。
	‌新值补充（30%）‌：快速响应网络状态的渐进变化（如带宽增减或路由切换）‌
  */
  constexpr const float kWeightFactor = 0.3f;
  return prev_avg_rtt * (1.0f - kWeightFactor) + cur_rtt_ms * kWeightFactor;
}

```



# 三、WebRTC中加权平均RTT的应用

目前有三个地方使用到加权平均RTT

1. NACK（否定确认）或ARQ（自动重传请求）中的缓冲区管理策略有关
2. RTP 协议中的 ‌Playout Delay‌ 通过头部扩展字段实现 网络状态动态调整缓冲区，降低卡顿率‌
3. 网络评估模型参数输入(gcc、BBR)
 


## 1、 NACK（否定确认）或ARQ（自动重传请求）中的缓冲区管理策略有关


### ① 发送端缓冲区管理‌
 
发送端需维护历史数据包队列，用于响应接收端的重传请求。主要策略包括：

1. 时间窗口限制‌：仅保留最近一段时间（如 1 秒）内发送的数据包‌。
    - 超出窗口的旧包自动淘汰，避免内存无限增长。
2.  容量限制‌：队列最大包数通常设为 1000（如 kMaxNackPackets=1000），防止缓冲区溢出‌
3.  淘汰机制‌：
    - 按时间顺序淘汰最旧数据包‌。
    - 对已确认接收或达到最大重传次数的包主动移除‌
  

根据 rtt 放弃 rtp包 

	 公式 ： 淘汰时间 = 3 × max(基准时间, 3 × 当前RTT)

    基准时间通常为 1000ms（兜底值，防止 RTT 过小导致缓存不足）

发送端缓冲区队列保存时间计算代码



```javascript 


void RtpPacketHistory::CullOldPackets(int64_t now_ms) 
{
    //TODO@chensong 2025-03-15 比如NACK（否定确认）或ARQ（自动重传请求）中的缓冲区管理策略有关。
	//  根据 rtt 放弃 rtp包 
	// 公式 ： 淘汰时间 = 3 × max(基准时间, 3 × 当前RTT)
    // 基准时间通常为 1000ms（兜底值，防止 RTT 过小导致缓存不足）
  int64_t packet_duration_ms = std::max(kMinPacketDurationRtt * rtt_ms_, kMinPacketDurationMs);
  while (!packet_history_.empty())
  {
    auto stored_packet_it = packet_history_.find(*start_seqno_);
    RTC_DCHECK(stored_packet_it != packet_history_.end());

    if (packet_history_.size() >= kMaxCapacity /* 9600*/) 
	{
      // We have reached the absolute max capacity, remove one packet
      // unconditionally.
      RemovePacket(stored_packet_it);
      continue;
    }

    const StoredPacket& stored_packet = stored_packet_it->second;
    if (!stored_packet.send_time_ms) 
	{
      // Don't remove packets that have not been sent.
      return;
    }

    if (*stored_packet.send_time_ms + packet_duration_ms > now_ms) 
	{
      // Don't cull packets too early to avoid failed retransmission requests.
      return;
    }

    if (packet_history_.size() >= number_to_store_ ||
        (mode_ == StorageMode::kStoreAndCull && *stored_packet.send_time_ms + (packet_duration_ms * kPacketCullingDelayFactor) <= now_ms)) 
	{
      // Too many packets in history, or this packet has timed out. Remove it
      // and continue.
      RemovePacket(stored_packet_it);
    }
	else 
	{
      // No more packets can be removed right now.
      return;
    }
  }
}

void RemoveOldReports(int64_t now, std::list<CallStats::RttTime>* reports) 
{
  static constexpr const int64_t kRttTimeoutMs = 1500;
  reports->remove_if([&now](CallStats::RttTime& r) { return now - r.time > kRttTimeoutMs; });
}

int64_t GetMaxRttMs(const std::list<CallStats::RttTime>& reports) 
{
  int64_t max_rtt_ms = -1;
  for (const CallStats::RttTime& rtt_time : reports)
  {
    max_rtt_ms = std::max(rtt_time.rtt, max_rtt_ms);
  }
  return max_rtt_ms;
}
```

### ② 接收端缓冲区管理‌

接收端通过缓冲区检测丢包并触发 NACK 请求：

‌1. 丢包检测‌：基于 RTP 序列号连续性判断丢包，维护 nack_list 记录待重传包‌ 
2‌. 乱序容忍‌：允许一定程度的乱序（如 kMaxPacketAge=10000 序列号跨度），避免误判‌ 
3‌. 动态触发‌：
   - 立即触发：首次检测到丢包时立即发送 NACK 请求‌
   - 周期性处理：每隔 20ms（kProcessIntervalMs=20）批量处理 nack_list，合并多次请求‌  、 重传超时时间设为 1.5×RTT，避免过早或过晚重传

```javascript


void NackModule::Process() {
  if (nack_sender_) 
  {
    std::vector<uint16_t> nack_batch;
    {
      rtc::CritScope lock(&crit_);
	  // TODO@chensong2023-03-29 以时间判断是否nack重新发送包
      nack_batch = GetNackBatch(kTimeOnly);
    }

    if (!nack_batch.empty())
    {
      nack_sender_->SendNack(nack_batch);
    }
  }

  // Update the next_process_time_ms_ in intervals to achieve
  // the targeted frequency over time. Also add multiple intervals
  // in case of a skip in time as to not make uneccessary
  // calls to Process in order to catch up.
  int64_t now_ms = clock_->TimeInMilliseconds();
  if (next_process_time_ms_ == -1) 
  {
    next_process_time_ms_ = now_ms + kProcessIntervalMs /*20*/;
  }
  else 
  {
    /* RTC_LOG(LS_INFO) << "start[ nack module -->next_process_time_ms_ = "
                      << next_process_time_ms_ << "][now_ms = " << now_ms <<
       "]";*/
    next_process_time_ms_ = next_process_time_ms_ + kProcessIntervalMs/*20*/ +
                            (now_ms - next_process_time_ms_) /
                                kProcessIntervalMs /*20*/ * kProcessIntervalMs /*20*/;
   /* RTC_LOG(LS_INFO) << "end [ nack module -->next_process_time_ms_ = "
                     << next_process_time_ms_ << "]";*/


  }
}



// TODO@chensong 2022-05-30
// 遍历所有可疑包 如果包符合条件 就插入nack_batch中
std::vector<uint16_t> NackModule::GetNackBatch(NackFilterOptions options) 
{
  // TODO@chensong 2022-05-30
	// 1. 标识以seq_num为判断条件
  bool consider_seq_num = options != kTimeOnly;
  // TODO@chensong 2022-05-30
  // 2. 标识以timestamp为判断条件 
  bool consider_timestamp = options != kSeqNumOnly;
  int64_t now_ms = clock_->TimeInMilliseconds();
  std::vector<uint16_t> nack_batch;
  auto it = nack_list_.begin();
  while (it != nack_list_.end()) 
  {
    // TODO@chensong 2022-05-30
	  // 1. send_nack_delay_ms_ 默认为0 ， 可修改
	  // TODO@chensong 2025-03-15   发送  nack 包 延迟发送   是否延迟nack包的修改
    bool delay_timed_out = now_ms - it->second.created_at_time >= send_nack_delay_ms_;
    // TODO@chensong 2022-05-30
	// 2. 从一次发送开始到现在， 是否超过了一个RTT的回路的时长 时间  
	// 需要得到一个RTT防止重复传送的情况 
    bool nack_on_rtt_passed = now_ms - it->second.sent_at_time >= rtt_ms_;
    // TODO@chensong 2022-05-30
	// 3、 第一次发送和最后处理包之前的
    bool nack_on_seq_num_passed = it->second.sent_at_time /*TODO@chensong 2022-05-30 如果是第一次发送*/== -1 &&
        AheadOrAt(newest_seq_num_, it->second.send_at_seq_num)/*TODO@chensong 2022-05-30 该包在最后处理的包之前*/;
    // TODO@chensong 2022-05-30
	// 符合条件
	//   1. seq 为判断条件
	//   2. rtt即时间为判断条件
    if (delay_timed_out && ((consider_seq_num && nack_on_seq_num_passed) || (consider_timestamp && nack_on_rtt_passed))) 
	{
      nack_batch.emplace_back(it->second.seq_num);
      ++it->second.retries;
      it->second.sent_at_time = now_ms;
      // TODO@chensong 2022-05-30
	  // 尝试10次 在nack_list列表中没有发现 就要删除了
      if (it->second.retries >= kMaxNackRetries/*kMaxNackRetries= 10*/) 
	  {
        RTC_LOG(LS_WARNING) << "Sequence number " << it->second.seq_num << " removed from NACK list due to max retries.";
        it = nack_list_.erase(it);
      } 
	  else 
	  {
        ++it;
      }
      continue;
    }
    ++it;
  }
  return nack_batch;
}

void NackModule::AddPacketsToNack(uint16_t seq_num_start, uint16_t seq_num_end) 
{
  // Remove old packets.
  auto it = nack_list_.lower_bound(seq_num_end - kMaxPacketAge);
  nack_list_.erase(nack_list_.begin(), it);

  // If the nack list is too large, remove packets from the nack list until
  // the latest first packet of a keyframe. If the list is still too large,
  // clear it and request a keyframe.
  // TODO@chensong 2022-05-30
  // 1. 开始到结束之间有多大距离 
  uint16_t num_new_nacks = ForwardDiff(seq_num_start, seq_num_end);
  if (nack_list_.size() + num_new_nacks > kMaxNackPackets) 
  {
    while (RemovePacketsUntilKeyFrame() && nack_list_.size() + num_new_nacks > kMaxNackPackets)
	{ }
    // TODO@chensong 2022-05-30
	// 1.1、 极端情况  没有删除， 就要清除nack， 然后发送请求关键帧给对方  让解码器从新工作哈  
	// TODO@chensong 2022-12-20  mediasoup 在业务层做了请求关键帧  ？？？ ====> 需要清除缓存的 和下面一样的步骤 这样的设计挺好的哈 ^_^ 
    if (nack_list_.size() + num_new_nacks > kMaxNackPackets) 
	{
      nack_list_.clear();
      RTC_LOG(LS_WARNING) << "NACK list full, clearing NACK"
                             " list and requesting keyframe.";
      keyframe_request_sender_->RequestKeyFrame();
      return;
    }
  }
  // TODO@chensong 2022-05-30
  // 2、 遍历seq_num_start 到seq_num_end 之间 是否有丢包 有的话 就放到nack_list_中哈
  for (uint16_t seq_num = seq_num_start; seq_num != seq_num_end; ++seq_num) 
  {
    // Do not send nack for packets that are already recovered by FEC or RTX
    // TODO@chensong 2022-05-30
	// 2.1 是否已经通过FEC或者RTX恢复了 该包 恢复了 就不需要放到nack_list_列表中去哈
	if (recovered_list_.find(seq_num) != recovered_list_.end())
	{
      continue;
	}
    NackInfo nack_info(seq_num, seq_num + WaitNumberOfPackets(0.5), clock_->TimeInMilliseconds());
    RTC_DCHECK(nack_list_.find(seq_num) == nack_list_.end());
    nack_list_[seq_num] = nack_info;
  }
}

// Send a Negative acknowledgment packet.
int32_t ModuleRtpRtcpImpl::SendNACK(const uint16_t* nack_list,
                                    const uint16_t size) {
  for (int i = 0; i < size; ++i) {
    receive_loss_stats_.AddLostPacket(nack_list[i]);
  }
  uint16_t nack_length = size;
  uint16_t start_id = 0;
  int64_t now_ms = clock_->TimeInMilliseconds();
  // TODO@chensong  20250318 
  //   ‌RTT 敏感重传‌：
  //    1. 重传超时时间设为 1.5×RTT，避免过早或过晚重传‌  
  //    2. 高RTT 时延长缓冲区窗口（如 2 秒），提高弱网下的恢复概率‌
  if (TimeToSendFullNackList(now_ms)) {
    nack_last_time_sent_full_ms_ = now_ms;
  } else {
    // Only send extended list.
    if (nack_last_seq_number_sent_ == nack_list[size - 1]) {
      // Last sequence number is the same, do not send list.
      return 0;
    }
    // Send new sequence numbers.
    for (int i = 0; i < size; ++i) {
      if (nack_last_seq_number_sent_ == nack_list[i]) {
        start_id = i + 1;
        break;
      }
    }
    nack_length = size - start_id;
  }

  // Our RTCP NACK implementation is limited to kRtcpMaxNackFields sequence
  // numbers per RTCP packet.
  if (nack_length > kRtcpMaxNackFields) {
    nack_length = kRtcpMaxNackFields;
  }
  nack_last_seq_number_sent_ = nack_list[start_id + nack_length - 1];

  return rtcp_sender_.SendRTCP(GetFeedbackState(), kRtcpNack, nack_length,
                               &nack_list[start_id]);
}
```

## 2、 RTP 协议中的 ‌Playout Delay‌ 通过头部扩展字段实现 网络状态动态调整JitterBuffer缓冲区，降低卡顿率‌

RTP 协议中的 ‌Playout Delay‌ 通过头部扩展字段实现，用于控制接收端播放缓冲区的延迟范围，优化实时流媒体的平滑性与交互性

1. 功能与作用
    - 动态缓冲控制‌：发送端通过 playout-delay 扩展字段向接收端传递 ‌最小（min）‌ 和 ‌最大（max）‌ 延迟值，指导接收端调整抖动缓冲区（Jitter Buffer）的延迟窗口‌ 
 ‌   
    - min 延迟‌：保证基本抗抖动能力，避免因网络波动导致频繁卡顿（如设为 100-200ms）‌  
 
     - max 延迟‌：限制最大容忍延迟，防止缓冲区过长影响实时性（如交互式通信场景设为 400ms）‌ 
  
 2‌. 场景适配‌：
      
- 交互式通信‌（如视频会议）：设置较小的延迟窗口（min=50ms, max=200ms），优先降低端到端延迟‌ 
‌    
- 流媒体播放‌（如直播）：增大 max 延迟（如 500ms），增强抗连续丢包能力‌


渲染延迟公式 =   Render time = Capture time in receiver time + playout  delay

接受端 TriggerCallbacksFromRtcpPacket  获取SR和RR获取rtt延迟设置 在SendVideo发送视频数据rtp头上增加播放延迟设置  jitterbuffer设置

```javascript

// Holding no Critical section.
void RTCPReceiver::TriggerCallbacksFromRtcpPacket(
    const PacketInformation& packet_information) {
  // Process TMMBR and REMB first to avoid multiple callbacks
  // to OnNetworkChanged.
  if (packet_information.packet_type_flags & kRtcpTmmbr) {
    // Might trigger a OnReceivedBandwidthEstimateUpdate.
    // TODO@chensong 2022-12-20 根据接收端反馈网络带宽 更新带宽模块 bandwidth
    // ？？？ [现在抛弃？]
    RTC_LOG(LS_INFO) << " RTCP RMMBR --> bitrate ";
    NotifyTmmbrUpdated();
  }
  uint32_t local_ssrc;
  std::set<uint32_t> registered_ssrcs;
  {
    // We don't want to hold this critsect when triggering the callbacks below.
    rtc::CritScope lock(&rtcp_receiver_lock_);
    local_ssrc = main_ssrc_;
    registered_ssrcs = registered_ssrcs_;
  }
  // TODO@chensong 2022-12-20 receiver_only_ default false
  if (!receiver_only_ && (packet_information.packet_type_flags & kRtcpSrReq)) {
    rtp_rtcp_->OnRequestSendReport();
  }
  // TODO@chensong 发送RTX丢包信息
  // TODO@chensong 2025-03-15 nack
  if (!receiver_only_ && (packet_information.packet_type_flags & kRtcpNack)) {
    if (!packet_information.nack_sequence_numbers.empty()) {
      RTC_LOG(LS_VERBOSE) << "Incoming NACK length: "
                          << packet_information.nack_sequence_numbers.size();
      // 请求重新发送seq的包   ModuleRtpRtcpImpl->OnReceivedNack
      rtp_rtcp_->OnReceivedNack(packet_information.nack_sequence_numbers);
    }
  }

  // We need feedback that we have received a report block(s) so that we
  // can generate a new packet in a conference relay scenario, one received
  // report can generate several RTCP packets, based on number relayed/mixed
  // a send report block should go out to all receivers.
  // TODO@chensong 2022-12-20 接受端请求立即刷新帧 (sps、pps信息)
  if (rtcp_intra_frame_observer_) {
    RTC_DCHECK(!receiver_only_);
    if ((packet_information.packet_type_flags & kRtcpPli) ||
        (packet_information.packet_type_flags & kRtcpFir)) {
      if (packet_information.packet_type_flags & kRtcpPli) {
        RTC_LOG(LS_VERBOSE)
            << "Incoming PLI from SSRC " << packet_information.remote_ssrc;
      } else {
        RTC_LOG(LS_VERBOSE)
            << "Incoming FIR from SSRC " << packet_information.remote_ssrc;
      }
      // TODO@chensong 2022-12-20
      // EncoderRtcpFeedback::OnReceivedIntraFrameRequest
      rtcp_intra_frame_observer_->OnReceivedIntraFrameRequest(local_ssrc);
    }
  }
  if (rtcp_loss_notification_observer_ &&
      (packet_information.packet_type_flags & kRtcpLossNotification)) {
    rtcp::LossNotification* loss_notification =
        packet_information.loss_notification.get();
    RTC_DCHECK(loss_notification);
    if (loss_notification->media_ssrc() == local_ssrc) {
      rtcp_loss_notification_observer_->OnReceivedLossNotification(
          loss_notification->media_ssrc(), loss_notification->last_decoded(),
          loss_notification->last_received(),
          loss_notification->decodability_flag());
    }
  }
  if (rtcp_bandwidth_observer_) {
    RTC_DCHECK(!receiver_only_);  // goog-remb 宽带评估
    if (packet_information.packet_type_flags & kRtcpRemb) {
      RTC_LOG(LS_VERBOSE)
          << "Incoming REMB: "
          << packet_information.receiver_estimated_max_bitrate_bps;
      rtcp_bandwidth_observer_->OnReceivedEstimatedBitrate(
          packet_information.receiver_estimated_max_bitrate_bps);
    }
    if ((packet_information.packet_type_flags & kRtcpSr) ||
        (packet_information.packet_type_flags & kRtcpRr)) {
      int64_t now_ms = clock_->TimeInMilliseconds();
      // TODO@chensong 2023-04-29 网络带宽评估输入参数
      // 这边只是进行计算并没有计算出目标码流
      rtcp_bandwidth_observer_->OnReceivedRtcpReceiverReport(
          packet_information.report_blocks, packet_information.rtt_ms, now_ms);
    }
  }
  // TODO@chensong 2022-12-20 接受sr或者rr信息做ack确认 没有看懂啥意思？？？
  // 感觉啥好像都没有干是的    
  // TODO@chensong 2025-03-16  设置rtp 扩展 播放端延迟播放设置参数 PlayDelay （）
  // ‌渲染延迟公式 =   Render time = Capture time in receiver time + playout  delay
  if ((packet_information.packet_type_flags & kRtcpSr) ||
      (packet_information.packet_type_flags & kRtcpRr)) {
    rtp_rtcp_->OnReceivedRtcpReportBlocks(packet_information.report_blocks);
  }

  if (transport_feedback_observer_ &&
      (packet_information.packet_type_flags & kRtcpTransportFeedback)) {
    uint32_t media_source_ssrc =
        packet_information.transport_feedback->media_ssrc();
    if (media_source_ssrc == local_ssrc ||
        registered_ssrcs.find(media_source_ssrc) != registered_ssrcs.end()) {
      // TODO@chensong 2022-12-05    接受端反馈过来的接受包seq和时间戳统计数据
      // remb
      // RtpTransportControllerSend::OnTransportFeedback
      // 这个代码带宽评估的非常重要一步是根据对端反馈网络带宽 带宽评估条件之一
      transport_feedback_observer_->OnTransportFeedback(
          *packet_information.transport_feedback);
    }
  }
  // TODO@chensong 2022-12-20  bitrate 对象没有
  // RtpRtcp::Configuration配置中默认是没有该bitrate_allocation_observer_对象的
  // 所以一般下面的逻辑不走了
  if (bitrate_allocation_observer_ &&
      packet_information.target_bitrate_allocation) {
    bitrate_allocation_observer_->OnBitrateAllocationUpdated(
        *packet_information.target_bitrate_allocation);
  }

  // TODO@chensong 2022-12-20 数据统计模块
  if (!receiver_only_) {
    rtc::CritScope cs(&feedbacks_lock_);
    if (stats_callback_) {
      for (const auto& report_block : packet_information.report_blocks) {
        RtcpStatistics stats;
        stats.packets_lost = report_block.packets_lost;
        stats.extended_highest_sequence_number =
            report_block.extended_highest_sequence_number;
        stats.fraction_lost = report_block.fraction_lost;
        stats.jitter = report_block.jitter;

        stats_callback_->StatisticsUpdated(stats, report_block.source_ssrc);
      }
    }
  }
}
bool RTPSenderVideo::SendVideo(VideoFrameType frame_type,
                               int8_t payload_type,
                               uint32_t rtp_timestamp,
                               int64_t capture_time_ms,
                               const uint8_t* payload_data,
                               size_t payload_size,
                               const RTPFragmentationHeader* fragmentation,
                               const RTPVideoHeader* video_header,
                               int64_t expected_retransmission_time_ms) {
  TRACE_EVENT_ASYNC_STEP1("webrtc", "Video", capture_time_ms, "Send", "type",
                          FrameTypeToString(frame_type));
  // TODO@chensong 2022-12-07 rtp拼接H264 数据包发送拼接
  if (frame_type == VideoFrameType::kEmptyFrame) {
    return true;
  }

  if (payload_size == 0) {
    return false;
  }
  RTC_CHECK(video_header);

  size_t fec_packet_overhead;
  bool red_enabled;
  int32_t retransmission_settings;
  bool set_video_rotation;
  bool set_color_space = false;
  // TODO@chensong 为什么会对H264单独处理呢  ？？？
  bool set_frame_marking =
      video_header->codec == kVideoCodecH264 &&
      video_header->frame_marking.temporal_id != kNoTemporalIdx;

  // 根据video_header信息，更新播放延迟(current_playout_delay_)
   //  TODO@chensong 2025-03-15 播放端 播放延迟设置   rtp包中扩展中设置延迟播放毫秒数
   // ‌渲染延迟公式 =   Render time = Capture time in receiver time + playout delay
  const absl::optional<PlayoutDelay> playout_delay = playout_delay_oracle_->PlayoutDelayToSend(video_header->playout_delay);
  {
    rtc::CritScope cs(&crit_);
    // According to
    // http://www.etsi.org/deliver/etsi_ts/126100_126199/126114/12.07.00_60/
    // ts_126114v120700p.pdf Section 7.4.5:
    // The MTSI client shall add the payload bytes as defined in this clause
    // onto the last RTP packet in each group of packets which make up a key
    // frame (I-frame or IDR frame in H.264 (AVC), or an IRAP picture in H.265
    // (HEVC)). The MTSI client may also add the payload bytes onto the last RTP
    // packet in each group of packets which make up another type of frame
    // (e.g. a P-Frame) only if the current value is different from the previous
    // value sent.
    // Set rotation when key frame or when changed (to follow standard).
    // Or when different from 0 (to follow current receiver implementation).
    set_video_rotation = frame_type == VideoFrameType::kVideoFrameKey ||
                         video_header->rotation != last_rotation_ ||
                         video_header->rotation != kVideoRotation_0;
    last_rotation_ = video_header->rotation;

    // Send color space when changed or if the frame is a key frame. Keep
    // sending color space information until the first base layer frame to
    // guarantee that the information is retrieved by the receiver.
    if (video_header->color_space != last_color_space_) 
	{
      last_color_space_ = video_header->color_space;
      set_color_space = true;
      transmit_color_space_next_frame_ = !IsBaseLayer(*video_header);
    }
	else 
	{
      set_color_space = frame_type == VideoFrameType::kVideoFrameKey ||
                        transmit_color_space_next_frame_;
      transmit_color_space_next_frame_ = transmit_color_space_next_frame_
                                             ? !IsBaseLayer(*video_header)
                                             : false;
    }

    // FEC settings.
    const FecProtectionParams& fec_params = frame_type == VideoFrameType::kVideoFrameKey ? key_fec_params_ : delta_fec_params_;
    if (flexfec_enabled()) 
	{
      flexfec_sender_->SetFecParameters(fec_params);
    }
    if (ulpfec_enabled()) 
	{
      ulpfec_generator_.SetFecParameters(fec_params);
    }

    fec_packet_overhead = CalculateFecPacketOverhead();
    red_enabled = this->red_enabled();
    retransmission_settings = retransmission_settings_;
  }

  // Maximum size of packet including rtp headers.
  // Extra space left in case packet will be resent using fec or rtx.
  // 其实封包的过程，就是计算一帧数据需要封多少个包、每个包放多少载荷，为此我们需要知道各种封包模式下，每个包的最大载荷（包大小减去头部大小）。

  //   首先计算一个包的最大容量，这个容量是指可以用来容纳 RTP
  //     头部和载荷的容量，FEC、重传的开销排除在外：
  int packet_capacity = rtp_sender_->MaxRtpPacketSize() - fec_packet_overhead -
                        (rtp_sender_->RtxStatus() ? kRtxHeaderSize : 0);

  std::unique_ptr<RtpPacketToSend> single_packet = rtp_sender_->AllocatePacket();
  RTC_DCHECK_LE(packet_capacity, single_packet->capacity());
  single_packet->SetPayloadType(payload_type);
  single_packet->SetTimestamp(rtp_timestamp);
  single_packet->set_capture_time_ms(capture_time_ms);

  // 接着准备四种包的模板：
  // single_packet: 对应 NAL unit 和 STAP-A 的包；
  // first_packet: 对应 FU-A 的首个包；
  // middle_packet: 对应 FU-A 的中间包；
  // last_packet: 对应 FU-A 的最后一个包；
  auto first_packet = absl::make_unique<RtpPacketToSend>(*single_packet);
  auto middle_packet = absl::make_unique<RtpPacketToSend>(*single_packet);
  auto last_packet = absl::make_unique<RtpPacketToSend>(*single_packet);
  // Simplest way to estimate how much extensions would occupy is to set them.
  // 根据video_header 给packet添加extension
  AddRtpHeaderExtensions(*video_header, playout_delay, frame_type,
                         set_video_rotation, set_color_space, set_frame_marking,
                         /*first=*/true, /*last=*/true, single_packet.get());
  AddRtpHeaderExtensions(*video_header, playout_delay, frame_type,
                         set_video_rotation, set_color_space, set_frame_marking,
                         /*first=*/true, /*last=*/false, first_packet.get());
  AddRtpHeaderExtensions(*video_header, playout_delay, frame_type,
                         set_video_rotation, set_color_space, set_frame_marking,
                         /*first=*/false, /*last=*/false, middle_packet.get());
  AddRtpHeaderExtensions(*video_header, playout_delay, frame_type,
                         set_video_rotation, set_color_space, set_frame_marking,
                         /*first=*/false, /*last=*/true, last_packet.get());

  RTC_DCHECK_GT(packet_capacity, single_packet->headers_size());
  RTC_DCHECK_GT(packet_capacity, first_packet->headers_size());
  RTC_DCHECK_GT(packet_capacity, middle_packet->headers_size());
  RTC_DCHECK_GT(packet_capacity, last_packet->headers_size());
  RtpPacketizer::PayloadSizeLimits limits;
  limits.max_payload_len = packet_capacity - middle_packet->headers_size();

  RTC_DCHECK_GE(single_packet->headers_size(), middle_packet->headers_size());
  limits.single_packet_reduction_len = single_packet->headers_size() - middle_packet->headers_size();

  RTC_DCHECK_GE(first_packet->headers_size(), middle_packet->headers_size());
  limits.first_packet_reduction_len = first_packet->headers_size() - middle_packet->headers_size();

  RTC_DCHECK_GE(last_packet->headers_size(), middle_packet->headers_size());
  limits.last_packet_reduction_len = last_packet->headers_size() - middle_packet->headers_size();

  RTPVideoHeader minimized_video_header;
  const RTPVideoHeader* packetize_video_header = video_header;

  rtc::ArrayView<const uint8_t> generic_descriptor_raw_00 = first_packet->GetRawExtension<RtpGenericFrameDescriptorExtension00>();
  rtc::ArrayView<const uint8_t> generic_descriptor_raw_01 = first_packet->GetRawExtension<RtpGenericFrameDescriptorExtension01>();

  if (!generic_descriptor_raw_00.empty() && !generic_descriptor_raw_01.empty()) 
  {
    RTC_LOG(LS_WARNING) << "Two versions of GFD extension used.";
    return false;
  }

  rtc::ArrayView<const uint8_t> generic_descriptor_raw = !generic_descriptor_raw_01.empty() ? generic_descriptor_raw_01
                                         : generic_descriptor_raw_00;
  if (!generic_descriptor_raw.empty()) 
  {
    if (MinimizeDescriptor(*video_header, &minimized_video_header)) 
	{
      packetize_video_header = &minimized_video_header;
    }
  }

  // 如果帧加密了，对payload和header进行加密
  // TODO(benwright@webrtc.org) - Allocate enough to always encrypt inline.
  rtc::Buffer encrypted_video_payload;
  if (frame_encryptor_ != nullptr) {
    if (generic_descriptor_raw.empty()) {
      return false;
    }
    // 获取帧加密后最大的长度
    const size_t max_ciphertext_size =
        frame_encryptor_->GetMaxCiphertextByteSize(cricket::MEDIA_TYPE_VIDEO,
                                                   payload_size);
    encrypted_video_payload.SetSize(max_ciphertext_size);

    size_t bytes_written = 0;

    // Only enable header authentication if the field trial is enabled.
    rtc::ArrayView<const uint8_t> additional_data;
    if (generic_descriptor_auth_experiment_) {
      additional_data = generic_descriptor_raw;
    }
    // 媒体数据进行加密哈  -->>>
    if (frame_encryptor_->Encrypt(
            cricket::MEDIA_TYPE_VIDEO, first_packet->Ssrc(), additional_data,
            rtc::MakeArrayView(payload_data, payload_size),
            encrypted_video_payload, &bytes_written) != 0) {
      return false;
    }

    encrypted_video_payload.SetSize(bytes_written);
    payload_data = encrypted_video_payload.data();
    payload_size = encrypted_video_payload.size();
  } else if (require_frame_encryption_) {
    RTC_LOG(LS_WARNING)
        << "No FrameEncryptor is attached to this video sending stream but "
        << "one is required since require_frame_encryptor is set";
  }

  VideoCodecType video_type;
  {
    rtc::CritScope cs(&payload_type_crit_);
    // payload_type_map_中payload_type编码器id什么时候注册呢 ？？？
    // TODO@chensong 2022-04-04   在rtp_video_sender
    // 中构造函数中进行注册编码器哈
    const auto it = payload_type_map_.find(payload_type);
    if (it == payload_type_map_.end()) {
      RTC_LOG(LS_ERROR) << "Payload type " << static_cast<int>(payload_type)
                        << " not registered.";
      return false;
    }
    video_type = it->second;
  }
  // TODO@chensong 2022-04-04 h264 and NALU 组包
  std::unique_ptr<RtpPacketizer> packetizer = RtpPacketizer::Create(
      video_type, rtc::MakeArrayView(payload_data, payload_size), limits,
      *packetize_video_header, frame_type, fragmentation);

  const uint8_t temporal_id = GetTemporalId(*video_header);
  StorageType storage = GetStorageType(temporal_id, retransmission_settings,
                                       expected_retransmission_time_ms);
  const size_t num_packets = packetizer->NumPackets();

  size_t unpacketized_payload_size;
  if (fragmentation && fragmentation->fragmentationVectorSize > 0) {
    unpacketized_payload_size = 0;
    for (uint16_t i = 0; i < fragmentation->fragmentationVectorSize; ++i) {
      unpacketized_payload_size += fragmentation->fragmentationLength[i];
    }
  } else {
    unpacketized_payload_size = payload_size;
  }
  size_t packetized_payload_size = 0;

  if (num_packets == 0) {
    return false;
  }

  uint16_t first_sequence_number;
  bool first_frame = first_frame_sent_();
  for (size_t i = 0; i < num_packets; ++i) {
    std::unique_ptr<RtpPacketToSend> packet;
    int expected_payload_capacity;
    // Choose right packet template:
    if (num_packets == 1) {
      // TODO@chensong 2022-12-19 nal rtp 不需要分包啦
      packet = std::move(single_packet);
      expected_payload_capacity =
          limits.max_payload_len - limits.single_packet_reduction_len;
    } else if (i == 0) {
      // TODO@chensong 2022-12-19 nal rtp 开始发送位置
      packet = std::move(first_packet);
      expected_payload_capacity =
          limits.max_payload_len - limits.first_packet_reduction_len;
    } else if (i == num_packets - 1) {
      // TODO@chensong 2022-12-19 nal 分包 rtp 结束标记 记录
      packet = std::move(last_packet);
      expected_payload_capacity =
          limits.max_payload_len - limits.last_packet_reduction_len;
    } else {
      packet = absl::make_unique<RtpPacketToSend>(*middle_packet);
      expected_payload_capacity = limits.max_payload_len;
    }

    if (!packetizer->NextPacket(packet.get())) {
      return false;
    }
    RTC_DCHECK_LE(packet->payload_size(), expected_payload_capacity);
    if (!rtp_sender_->AssignSequenceNumber(packet.get())) {
      return false;
    }
    packetized_payload_size += packet->payload_size();

    if (rtp_sequence_number_map_ && i == 0) {
      first_sequence_number = packet->SequenceNumber();
    }

    if (i == 0) {
      playout_delay_oracle_->OnSentPacket(packet->SequenceNumber(),
                                          playout_delay);
    }
    // No FEC protection for upper temporal layers, if used.
    bool protect_packet = temporal_id == 0 || temporal_id == kNoTemporalIdx;

    // Put packetization finish timestamp into extension.
    if (packet->HasExtension<VideoTimingExtension>()) {
      packet->set_packetization_finish_time_ms(clock_->TimeInMilliseconds());
      // TODO(ilnik): Due to webrtc:7859, packets with timing extensions are not
      // protected by FEC. It reduces FEC efficiency a bit. When FEC is moved
      // below the pacer, it can be re-enabled for these packets.
      // NOTE: Any RTP stream processor in the network, modifying 'network'
      // timestamps in the timing frames extension have to be an end-point for
      // FEC, otherwise recovered by FEC packets will be corrupted.
      protect_packet = false;
    }
    //////////////////////////////////////////////////////////////////////////
    //////TODO@chensong ulpfec  2022-09-13  RED %%%
    if (flexfec_enabled()) {
      // TODO(brandtr): Remove the FlexFEC code path when FlexfecSender
      // is wired up to PacedSender instead.
      SendVideoPacketWithFlexfec(std::move(packet), storage, protect_packet);
    } else if (red_enabled) {
      SendVideoPacketAsRedMaybeWithUlpfec(std::move(packet), storage,
                                          protect_packet);
    } else {
      // TODO@chensong
      SendVideoPacket(std::move(packet), storage);
    }

    if (first_frame) {
      if (i == 0) {
        RTC_LOG(LS_INFO)
            << "Sent first RTP packet of the first video frame (pre-pacer)";
      }
      if (i == num_packets - 1) {
        RTC_LOG(LS_INFO)
            << "Sent last RTP packet of the first video frame (pre-pacer)";
      }
    }
  }

  if (rtp_sequence_number_map_) {
    const uint32_t timestamp = rtp_timestamp - rtp_sender_->TimestampOffset();
    rtc::CritScope cs(&crit_);
    rtp_sequence_number_map_->InsertFrame(first_sequence_number, num_packets,
                                          timestamp);
  }

  rtc::CritScope cs(&stats_crit_);
  RTC_DCHECK_GE(packetized_payload_size, unpacketized_payload_size);
  packetization_overhead_bitrate_.Update(
      packetized_payload_size - unpacketized_payload_size,
      clock_->TimeInMilliseconds());

  TRACE_EVENT_ASYNC_END1("webrtc", "Video", capture_time_ms, "timestamp",
                         rtp_timestamp);
  return true;
}

```

## 3、 网络评估模型参数输入RTT(GCC、BBR)




# 总结


 
[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)

