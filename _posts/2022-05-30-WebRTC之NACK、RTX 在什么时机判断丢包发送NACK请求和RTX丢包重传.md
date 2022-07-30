---
layout:     post
title:      WebRTC之NACK、RTX 在什么时机判断丢包发送NACK请求和RTX丢包重传
subtitle:   视频质量(Qos)、RTCP/RTP、 NACK、RTX
date:       2022-05-30
times:       00::55::33 
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - 网络编程
    - RTC服务质量(Qos)
  
---


# WebRTC之NACK、RTX 在什么时机判断丢包发送NACK请求和RTX丢包重传


@[TOC](WebRTC之NACK、RTX 在什么时机判断丢包发送NACK请求和RTX丢包重传)

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

[2、WebRTC中RTP协议详解](https://blog.csdn.net/Poisx/article/details/125037393)

[3、WebRTC之NACK、RTX 在什么时机判断丢包发送NACK请求和RTX丢包重传](#)

[4、WebRTC源码之视频质量统计数据的数据结构分析](#)


九、 NetEQ

十、 Simulcast与SVC


# 前言
NACK 是判断网络是否丢包重传， 和网络情况

## 一、NACK与RTX的作用
<font color='red'>1、NACK用于通知丢失了哪些包
2、RTX用于重传丢失的包</font>

在发送offer和answer中是否正常Nack和RTX的协议

```
···
a=rtpmap:96 VP8/90000
a=rtcp-fb:96 goog-remb
a=rtcp-fb:96 transport-cc
a=rtcp-fb:96 ccm fir
a=rtcp-fb:96 nack
a=rtcp-fb:96 nack pli
a=rtpmap:97 rtx/90000
a=fmtp:97 apt=96
···
```

### 1、NACK/RTX的工作机制的流程图

![在这里插入图片描述](https://img-blog.csdnimg.cn/6043390451e34ddeb0afcc2ebfd0a983.png)
### 2、NACK/RTX涉及到的几个问题
1、如何判定算法发送了丢包
        	Sequence Number 不连续的时候是丢包了哈
2、NACK什么时候发送
3、NACK的格式是怎样的，当发送端收到NACK时如何处理？
4、RTX格式是怎样的，RTX与NACK如何配合的？

## 二、判断包位置的关键算法
### 1、一个关键的函数：AheadOf
![在这里插入图片描述](https://img-blog.csdnimg.cn/a204fe1606b24feeadad8f5ea1eb857e.png)


1、 AheadOf(a,b), 比较a与b的顺序关系
2、a与b必须是无符号整数
3、如果吧排在a前面，则返回真，否则返回false
4、需要注意的是，他们不是简单的数值大小的比较


```

```

## <font color='red'>三、WebRTC中NACK的处理流程图</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/07e674c021fa473ba4fbf72db0b4a922.png)


1、NackMoudle的创建时机

在RtpVideoStreamReceiver中构造函数中创建NackMoudle类哈

RtpVideoStreamReceiver是在VideoReceiveStream类的构造函数创建哈


### 1、NACK调用栈

![在这里插入图片描述](https://img-blog.csdnimg.cn/e8687a844abb4af3819d360687034c31.png)
##  四、WebRTC如何判断是否丢包的逻辑

![在这里插入图片描述](https://img-blog.csdnimg.cn/8c447a64dd334c94bc9ec845ea409a49.png)


### 1、NackModule::OnReceivedPacket 函数
moudle/video_codeing/nack_module.h

```

int NackModule::OnReceivedPacket(uint16_t seq_num,
                                 bool is_keyframe,
                                 bool is_recovered) {
  rtc::CritScope lock(&crit_);
  // TODO(philipel): When the packet includes information whether it is
  //                 retransmitted or not, use that value instead. For
  //                 now set it to true, which will cause the reordering
  //                 statistics to never be updated.
  bool is_retransmitted = true;
  // 1. 判断是否第一次， 初始化  完成就退出
  if (!initialized_) {
    newest_seq_num_ = seq_num;
    if (is_keyframe)// 这个包是否关键帧===》》 为什么要识别关键帧？？？
      keyframe_list_.insert(seq_num);
    initialized_ = true;
    return 0;
  }

  // Since the |newest_seq_num_| is a packet we have actually received we know
  // that packet has never been Nacked.
  // 2. 如果这次来的seq与上次一样，是重复包， 退出
  if (seq_num == newest_seq_num_)
    return 0;
  // 即不是第一个包和重复包就判断包顺序哈 seq_num在newest_seq_num之前就要删除了哈  
  // 3. 如果是上次处理前面的包， 这个包已经失效了， 如果还在nack列表中， 需要删除的
  // 说明这个包晚到达了 
  if (AheadOf(newest_seq_num_, seq_num)) {
    // An out of order packet has been received.
    auto nack_list_it = nack_list_.find(seq_num);
    int nacks_sent_for_packet = 0;
    if (nack_list_it != nack_list_.end()) {
      nacks_sent_for_packet = nack_list_it->second.retries;
      nack_list_.erase(nack_list_it);
    }
    if (!is_retransmitted)
      UpdateReorderingStatistics(seq_num);
    return nacks_sent_for_packet;
  }

  // Keep track of new keyframes.
  // 4. 如果判断是否是key帧？？？ 哈
  if (is_keyframe)
    keyframe_list_.insert(seq_num); // 如果该报属于key帧， 保持起来

  // And remove old ones so we don't accumulate keyframes.
  // 5. 找到最小边界点，   超出10000个就要删除之前的数据 ， 这个是实时系统
  auto it = keyframe_list_.lower_bound(seq_num - kMaxPacketAge);
  if (it != keyframe_list_.begin())
    keyframe_list_.erase(keyframe_list_.begin(), it);
  // 6. 如何判断是否找回来的包？？？  恢复包
  if (is_recovered) {
    recovered_list_.insert(seq_num); // 如果该包是属于key帧，保持起来

    // Remove old ones so we don't accumulate recovered packets.
	//  是否超出项 超出项也删除了  ， 最大项也是10000哈
    auto it = recovered_list_.lower_bound(seq_num - kMaxPacketAge);
    if (it != recovered_list_.begin())
      recovered_list_.erase(recovered_list_.begin(), it);

    // Do not send nack for packets recovered by FEC or RTX.
    return 0;
  }
  // 7. 什么情况会走到这边呢 
  //     1、不是第一个包
  //     2. 不是一个重复的包
  //     3、 不是在new_seq_num之前的包
  //     4、 不是一个恢复包
  //   有两种情况会走到这边
  //     1、  上一次处理的包的后面的一个包哈   有序的包
  //     2、  上一次处理的包 后面隔好几个包   
  AddPacketsToNack(newest_seq_num_ + 1, seq_num);
  newest_seq_num_ = seq_num;

  // Are there any nacks that are waiting for this seq_num.
  // 8. 哪些包是真真丢包的  就告诉对方从新发送包哈
  std::vector<uint16_t> nack_batch = GetNackBatch(kSeqNumOnly);
  if (!nack_batch.empty()) 
    nack_sender_->SendNack(nack_batch);  //  需要重传哈   放到缓冲区了 ??????

  return 0;
}

```
### 2、 NackModule::AddPacketsToNack初步判断有哪些包丢包了


```
void NackModule::AddPacketsToNack(uint16_t seq_num_start,
                                  uint16_t seq_num_end) {
  // Remove old packets.
  auto it = nack_list_.lower_bound(seq_num_end - kMaxPacketAge);
  nack_list_.erase(nack_list_.begin(), it);

  // If the nack list is too large, remove packets from the nack list until
  // the latest first packet of a keyframe. If the list is still too large,
  // clear it and request a keyframe.
  // 1. 开始到结束之间有多大距离 
  uint16_t num_new_nacks = ForwardDiff(seq_num_start, seq_num_end);
  if (nack_list_.size() + num_new_nacks > kMaxNackPackets) {
    while (RemovePacketsUntilKeyFrame() &&
           nack_list_.size() + num_new_nacks > kMaxNackPackets) {
    }
	// 1.1、 极端情况  没有删除， 就要清除nack， 然后发送请求关键帧给对方  让解码器从新工作哈
    if (nack_list_.size() + num_new_nacks > kMaxNackPackets) {
      nack_list_.clear();
      RTC_LOG(LS_WARNING) << "NACK list full, clearing NACK"
                             " list and requesting keyframe.";
      keyframe_request_sender_->RequestKeyFrame();
      return;
    }
  }
  // 2、 遍历seq_num_start 到seq_num_end 之间 是否有丢包 有的话 就放到nack_list_中哈
  for (uint16_t seq_num = seq_num_start; seq_num != seq_num_end; ++seq_num) {
    // Do not send nack for packets that are already recovered by FEC or RTX
	// 2.1 是否已经通过FEC或者RTX恢复了 该包 恢复了 就不需要放到nack_list_列表中去哈
    if (recovered_list_.find(seq_num) != recovered_list_.end())
      continue;
    NackInfo nack_info(seq_num, seq_num + WaitNumberOfPackets(0.5),
                       clock_->TimeInMilliseconds());
    RTC_DCHECK(nack_list_.find(seq_num) == nack_list_.end());
    nack_list_[seq_num] = nack_info;
  }
}
```

### 3、 GetNackBatch （真真判定丢包的函数）
```
// 遍历所有可疑包 如果包符合条件 就插入nack_batch中
std::vector<uint16_t> NackModule::GetNackBatch(NackFilterOptions options) {
	// 1. 标识以seq_num为判断条件
  bool consider_seq_num = options != kTimeOnly;
  // 2. 标识以timestamp为判断条件 
  bool consider_timestamp = options != kSeqNumOnly;
  int64_t now_ms = clock_->TimeInMilliseconds();
  std::vector<uint16_t> nack_batch;
  auto it = nack_list_.begin();
  while (it != nack_list_.end()) {
	  // 1. send_nack_delay_ms_ 默认为0 ， 可修改
    bool delay_timed_out = now_ms - it->second.created_at_time >= send_nack_delay_ms_;
	// 2. 从一次发送开始到现在， 是否超过了一个RTT的回路的时长 时间  
	// 需要得到一个RTT防止重复传送的情况 
    bool nack_on_rtt_passed = now_ms - it->second.sent_at_time >= rtt_ms_;
	// 3、 第一次发送和最后处理包之前的
    bool nack_on_seq_num_passed = it->second.sent_at_time /*如果是第一次发送*/== -1 &&
        AheadOrAt(newest_seq_num_, it->second.send_at_seq_num)/*该包在最后处理的包之前*/;
	// 符合条件
    if (delay_timed_out && ((consider_seq_num && nack_on_seq_num_passed) ||
                            (consider_timestamp && nack_on_rtt_passed))) {
      nack_batch.emplace_back(it->second.seq_num);
      ++it->second.retries;
      it->second.sent_at_time = now_ms;
	  // 尝试10次 在nack_list列表中没有发现 就要删除了
      if (it->second.retries >= kMaxNackRetries/*kMaxNackRetries= 10*/) {
        RTC_LOG(LS_WARNING) << "Sequence number " << it->second.seq_num
                            << " removed from NACK list due to max retries.";
        it = nack_list_.erase(it);
      } else {
        ++it;
      }
      continue;
    }
    ++it;
  }
  return nack_batch;
}
```

其中要的判断条件

![在这里插入图片描述](https://img-blog.csdnimg.cn/18eebb7eefd74985b0c1051d6368c6c8.png)
### 4、周期性执行的函数NackModule::Process

```

void NackModule::Process() {
  if (nack_sender_) {
    std::vector<uint16_t> nack_batch;
    {
      rtc::CritScope lock(&crit_);
      nack_batch = GetNackBatch(kTimeOnly);
    }

    if (!nack_batch.empty())
      nack_sender_->SendNack(nack_batch);
  }

  // Update the next_process_time_ms_ in intervals to achieve
  // the targeted frequency over time. Also add multiple intervals
  // in case of a skip in time as to not make uneccessary
  // calls to Process in order to catch up.
  int64_t now_ms = clock_->TimeInMilliseconds();
  if (next_process_time_ms_ == -1) {
    next_process_time_ms_ = now_ms + kProcessIntervalMs;
  } else {
    next_process_time_ms_ = next_process_time_ms_ + kProcessIntervalMs +
                            (now_ms - next_process_time_ms_) /
                                kProcessIntervalMs * kProcessIntervalMs;
  }
}
```

### <font color='red'>5、Process函数和OnReceivedPacket函数都有有发送nack函数目的</font>

1. 一个是时间查找丢失的包
2. 一个seq_num的顺序查找丢失包


<font color='red'>这个WebRTC在m74版本中设计怎么样哈 ， 给你会怎么设计这个丢包重传哈</font>

## 五、WebRTC中VP8关键帧的判断

### 1、VP8 RTP结构图

![在这里插入图片描述](https://img-blog.csdnimg.cn/788c19655d6948d7858afbbf50e5ea0f.png)



### 2、 VP8 Payload 结构图

![在这里插入图片描述](https://img-blog.csdnimg.cn/bfd9467ea13a409d832d538efd54be30.png)

### 3、 VP8 Payload描述符两种结构

![在这里插入图片描述](https://img-blog.csdnimg.cn/25d7462352ae474496fcad18696f7372.png)


#### 3.1、VP8 Payload 区别和字段含义
1、 两者的区别
	
	pictureID： 第一个7位第二种15位

2、 字段含义

	X：代表是否下面一行有扩展 [I|L|T|K|RSV]
	R：预留的字段
	N:  1：代表是非参考帧，0：是参考帧
	S：是否属于视频帧分片  1：视频帧的第一个分片 0：其他分片
	R：预留位
	PID：表示分片的序号 ， 最大不超过8

#### 3.2、 描述符中的扩展字节
1、I，该位置1， 表示存在PictureID，且其紧跟在扩展字节之后
2、L， 该位置1， 表示存在TLOPICIDX，它跟在PictureID之后，且T位必须置1
3、T，该位置1或者（T=0其K=1），TID/Y/KEYIDX字节存在，否则不存在
4、K，置1，TID/Y/KEYIDX字节存在；T=1且K=0，KEYIDX域被忽略；否则TID/Y/KEYIDX字节不存在
5、SRV，保留，必须设置为0


<font color='red'>需要注意的点
1、PictureID域由M和PictureID组成。
	M=1 ：PictureID占15位；
          M = 0 : 占7位
2、 TID= 0 ， TLOPICIDX表示的是时间基础层帧的运行索引
	视频的分层
3、TID> 0 ： TLOPICIDX表示的是当前图像所依赖的基础层帧
4、TID/Y/KEYIDX域中，TID占两位，Y占1位，KEYIDX占5位
	TID：代表时间层，基础层为0，层级越高，值越大
	Y： 层同步位，置1，表示当前帧仅依赖基础层帧（TIDO）；置0表示当前帧不依赖基础帧
	KEYIDX：key帧索引值

</font>

### 4、VP8 Payload Header

![在这里插入图片描述](https://img-blog.csdnimg.cn/4e77e376f9204fc3971a8b5eb0ae01bc.png)

#### 4.1、VP8 Payload header 字段含义


一、 Header
1、对于内部帧，该域占3字节；对于key帧，该域占10字节
2、其中前3个字节的结构是通用的
3、该Header只有在上面的S位置位且PID=0时才存在
二、字段含义
1、P，1位，表示帧类型： 0-key, 1-infterframe  关键帧
2、VER，3位，1-3定义了4种不同的解码复杂度的profile
3、SIze， 19位，第一个分片字节的大小


### 5、KeyFrame Header 乘下的7个字节 含义

1、 3字节起始码： 固定值： 0x9D，0x01,0x2A
2、接下来的16位：（2bit Horizontal Scale << 14） | Width (14bits)
3、最后16位： （2bits Vertical Scale << 14）| Height (14bits)


# 总结：

[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)


