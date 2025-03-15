---
layout:     post
title:      WebRTC中音视频服务质量QoS之RTT衡量网络往返时延加权平均RTT计算机制‌的详解
subtitle:   WebRTC、音视频服务质量QoS、RTT
date:       2025-03-15
times:      23::59::33
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
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
### 触发条件‌：接收端仅拉流（不发送媒体数据），通过 ‌RTCP Extended Reports (XR)‌ 扩展协议实现 RTT 探测‌

实现步骤‌：

1. 网关发送 ‌RRTR 报文‌（含 NTP 时间戳 $T_{RRTR}$)
2. 接收端回复 ‌DLRR 报文‌，包含 
     - 原$T_{RRTR}$ (即为LRR)
     - 处理延迟$T_{DLSR}$(接收 RRTR 到发送 DLRR 的时间)
3. 网关计算公式

	  $RTT = {T_{current}}$   - ${T_{TRR}}$ - ${T_{DLSR}}$


# 二、网络质量评估算法之时延加权平均RTT计算机制‌

加权平均RTT计算机制‌
	在实时通信场景（如WebRTC）中，RTT（往返时延）的平滑计算对网络状态感知和拥塞控制至关重要。通过 ‌加权移动平均（Weighted Moving Average）‌ 
	对RTT值进行动态调整，可有效平衡历史数据与实时测量值的影响，抑制短期波动带来的干扰。以下是核心实现逻辑：

	‌1. 公式定义‌
	
	‌计算方式‌：
	
	新平均RTT由 ‌历史平均值（old_avg）‌ 与 ‌最新测量值（new_sample）‌ 按权重合成，公式为：

 
	avg_rtt = 0.7 * old_avg + 0.3 * new_sample  
	
	其中，历史数据权重为70%（0.7），新样本权重为30%（0.3）‌23。

	‌数学意义‌：
	
	‌旧值主导（70%）‌：确保长期趋势稳定，避免偶发延迟突变（如网络抖动）对整体估计的过度影响‌23。
	
	‌新值补充（30%）‌：快速响应网络状态的渐进变化（如带宽增减或路由切换）‌


# 三、 rtp和rtcp发送包列表数据保存时间 （WebRTC根据rtt计算的）

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

```


# 总结


 
[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)

