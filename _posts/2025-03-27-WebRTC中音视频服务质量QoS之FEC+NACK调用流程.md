---
layout:     post
title:      WebRTC中音视频服务质量QoS之FEC+NACK调用流程
subtitle:   WebRTC、音视频服务质量QoS、JitterBuffer、RTT
date:       2025-03-15
times:      23::54::35
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - JitterBuffer
    - RTT
    - FEC
    - Qos
---




# WebRTC中音视频服务质量QoS之FEC+NACK调用流程


@[TOC](WebRTC中音视频服务质量QoS之FEC+NACK调用流程)

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

[7、WebRTC中音视频服务质量QoS之FEC+NACK调用流程]()

九、 NetEQ

十、 Simulcast与SVC


# 前言



WebRTC中FEC分为两个模块 

     1. FlexfecSender类负责对发送包前向纠错发送 
     2. FecControllerDefault类评估出现FEC的保护比率
     
调用流程图

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/9d25e91a91d34cb4917de537964b6b94.png)


下面是类关系图


![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/d0a7d3a9855040fd8c932e4faefa3a4b.png#pic_center)




# 一、   FlexfecSender类负责对发送包前向纠错发送 


modules\rtp_rtcp\source/flexfec_sender.h

FlexfecSender 类的关系图



RtpTransportControllerSend对象中有成员对象GoogCcNetworkControllerFactory工程的gcc算法类

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/307b1662e0ae4cabbd6b4099a07f785f.png)

 

call/call.h            Call::Create -> 创建 RtpTransportControllerSend 然后保存Call对象中去

call/call.h  call  -> CreateVideoSendStream创建视频发送流 send_stream 插入video_send_streams_ 中

video/video_send_stream.h  VideoSendStream -> 构造函数中任务队列worker_queue_->PostTask中创建VideoSendStreamImpl对象

video/video_send_stream_impl.h  VideoSendStreamImpl    -> 调用 RtpTransportControllerSend  transport_-> CreateRtpVideoSender  ->   构造函数创建RtpVideoSender 把创建RtpVideoSender放到RtpTransportControllerSend类成员队列中去

call/rtp_video_sender.h    RtpVideoSender  ->  构造函数调用MaybeCreateFlexfecSender创建FlexfecSender对象


modules\rtp_rtcp\include/flexfec_sender.h  FlexfecSender -> 构造函数中创建UlpfecGenerator对象ForwardErrorCorrection::CreateFlexfec




## 1、 Call::Create 创建Call对象



创建Call对象  中需要 发送rtp类RtpTransportControllerSend  保存到Call对象去

```javascript

// TODO@chensong 20220803 网络发送信息包线程 PacerThread、ModuleProcessThread
Call* Call::Create(const Call::Config& config) {
  return Create(config, Clock::GetRealTimeClock(),
                ProcessThread::Create("PacerThread"),
                ProcessThread::Create("ModuleProcessThread"));
}

Call* Call::Create(const Call::Config& config,
                   Clock* clock,
                   std::unique_ptr<ProcessThread> call_thread,
                   std::unique_ptr<ProcessThread> pacer_thread) {
  // TODO(bugs.webrtc.org/10284): DCHECK task_queue_factory dependency is
  // always provided in the config.
  TaskQueueFactory* task_queue_factory = config.task_queue_factory
                                             ? config.task_queue_factory
                                             : &GlobalTaskQueueFactory();

  // TODO@chensong 2022-09-29
  //		1.
  //RtpTransportControllerSend中有个线程pacer专门发送网络包反馈网络情况
  //		2. Call :
  return new internal::Call(
      clock, config,
      absl::make_unique<RtpTransportControllerSend>(
          clock, config.event_log, config.network_state_predictor_factory,
          config.network_controller_factory, config.bitrate_config,
          std::move(pacer_thread), task_queue_factory),
      std::move(call_thread), task_queue_factory);
}




// Call构造函数

Call::Call(Clock* clock,
           const Call::Config& config,
           std::unique_ptr<RtpTransportControllerSendInterface> transport_send,
           std::unique_ptr<ProcessThread> module_process_thread,
           TaskQueueFactory* task_queue_factory)
    : clock_(clock),
      task_queue_factory_(task_queue_factory),
      num_cpu_cores_(CpuInfo::DetectNumberOfCores()),
      module_process_thread_(std::move(module_process_thread)),
      call_stats_(new CallStats(clock_, module_process_thread_.get())),
      bitrate_allocator_(new BitrateAllocator(clock_, this)),
      config_(config),
      audio_network_state_(kNetworkDown),
      video_network_state_(kNetworkDown),
      aggregate_network_up_(false),
      receive_crit_(RWLockWrapper::CreateRWLock()),
      send_crit_(RWLockWrapper::CreateRWLock()),
      event_log_(config.event_log),
      received_bytes_per_second_counter_(clock_, nullptr, true),
      received_audio_bytes_per_second_counter_(clock_, nullptr, true),
      received_video_bytes_per_second_counter_(clock_, nullptr, true),
      received_rtcp_bytes_per_second_counter_(clock_, nullptr, true),
      last_bandwidth_bps_(0),
      min_allocated_send_bitrate_bps_(0),
      configured_max_padding_bitrate_bps_(0),
      estimated_send_bitrate_kbps_counter_(clock_, nullptr, true),
      pacer_bitrate_kbps_counter_(clock_, nullptr, true),
      receive_side_cc_(clock_, transport_send->packet_router()),
      receive_time_calculator_(ReceiveTimeCalculator::CreateFromFieldTrial()),
      video_send_delay_stats_(new SendDelayStats(clock_)),
      start_ms_(clock_->TimeInMilliseconds()) {
  RTC_DCHECK(config.event_log != nullptr);
  transport_send_ = std::move(transport_send);
  transport_send_ptr_ = transport_send_.get();
}
```


## 2、CreateVideoSendStream创建视频发送流  触法时机

WebRtcVideoChannel 调用CreateVideoSendStream时机是设置本地sdp


创建视频发送流  触法RecreateWebRtcStream方法时机 有四种：
 

1. 设置编码 SetCodec接口
2. 设置发送参数 SetSendParameters 接口 编码
3. 设置编码器 SetFrameEncryptor 接口
4. 设置本地ssrc的 SetLocalSsrc 接口
5. 设置网络feedback (nack、remb、tcc、rtcp)接口 SetFeedbackParameters 接口


###   ① RecreateWebRtcStream
```javascript


void WebRtcVideoChannel::WebRtcVideoSendStream::RecreateWebRtcStream() {
  RTC_DCHECK_RUN_ON(&thread_checker_);
  if (stream_ != NULL) {
    call_->DestroyVideoSendStream(stream_);
  }

  RTC_CHECK(parameters_.codec_settings);
  RTC_DCHECK_EQ((parameters_.encoder_config.content_type ==
                 webrtc::VideoEncoderConfig::ContentType::kScreen),
                parameters_.options.is_screencast.value_or(false))
      << "encoder content type inconsistent with screencast option";
  parameters_.encoder_config.encoder_specific_settings =
      ConfigureVideoEncoderSettings(parameters_.codec_settings->codec);

  webrtc::VideoSendStream::Config config = parameters_.config.Copy();
  if (!config.rtp.rtx.ssrcs.empty() && config.rtp.rtx.payload_type == -1) {
    RTC_LOG(LS_WARNING) << "RTX SSRCs configured but there's no configured RTX "
                           "payload type the set codec. Ignoring RTX.";
    config.rtp.rtx.ssrcs.clear();
  }
  if (parameters_.encoder_config.number_of_streams == 1) {
    // SVC is used instead of simulcast. Remove unnecessary SSRCs.
    if (config.rtp.ssrcs.size() > 1) {
      config.rtp.ssrcs.resize(1);
      if (config.rtp.rtx.ssrcs.size() > 1) {
        config.rtp.rtx.ssrcs.resize(1);
      }
    }
  }
  // 创建视频发送流程  
  stream_ = call_->CreateVideoSendStream(std::move(config),
                                         parameters_.encoder_config.Copy());

  parameters_.encoder_config.encoder_specific_settings = NULL;

  if (source_) {
    stream_->SetSource(this, GetDegradationPreference());
  }

  // Call stream_->Start() if necessary conditions are met.
  UpdateSendState();
}
```


###   ② CreateVideoSendStream

```javascript 

webrtc::VideoSendStream* Call::CreateVideoSendStream(
    webrtc::VideoSendStream::Config config,
    VideoEncoderConfig encoder_config) {
  if (config_.fec_controller_factory) {
    RTC_LOG(LS_INFO) << "External FEC Controller will be used.";
  }
  // 默认创建fec控制类
  std::unique_ptr<FecController> fec_controller =
      config_.fec_controller_factory
          ? config_.fec_controller_factory->CreateFecController()
          : absl::make_unique<FecControllerDefault>(clock_);
  return CreateVideoSendStream(std::move(config), std::move(encoder_config),
                               std::move(fec_controller));
}



// This method can be used for Call tests with external fec controller factory.
webrtc::VideoSendStream* Call::CreateVideoSendStream(
    webrtc::VideoSendStream::Config config,
    VideoEncoderConfig encoder_config,
    std::unique_ptr<FecController> fec_controller) {
  TRACE_EVENT0("webrtc", "Call::CreateVideoSendStream");
  RTC_DCHECK_RUN_ON(&configuration_sequence_checker_);

  RTC_DCHECK(media_transport() == config.media_transport);

  RegisterRateObserver();

  video_send_delay_stats_->AddSsrcs(config);
  for (size_t ssrc_index = 0; ssrc_index < config.rtp.ssrcs.size();
       ++ssrc_index) {
    event_log_->Log(absl::make_unique<RtcEventVideoSendStreamConfig>(
        CreateRtcLogStreamConfig(config, ssrc_index)));
  }

  // TODO(mflodman): Base the start bitrate on a current bandwidth estimate, if
  // the call has already started.
  // Copy ssrcs from |config| since |config| is moved.
  std::vector<uint32_t> ssrcs = config.rtp.ssrcs;

  VideoSendStream* send_stream = new VideoSendStream(
      clock_, num_cpu_cores_, module_process_thread_.get(), task_queue_factory_,
      call_stats_.get(), transport_send_ptr_, bitrate_allocator_.get(),
      video_send_delay_stats_.get(), event_log_, std::move(config),
      std::move(encoder_config), suspended_video_send_ssrcs_,
      suspended_video_payload_states_, std::move(fec_controller));

  {
    WriteLockScoped write_lock(*send_crit_);
    for (uint32_t ssrc : ssrcs) {
      RTC_DCHECK(video_send_ssrcs_.find(ssrc) == video_send_ssrcs_.end());
      video_send_ssrcs_[ssrc] = send_stream;
    }
    video_send_streams_.insert(send_stream);
  }
  UpdateAggregateNetworkState();

  return send_stream;
}


```


## 2 创建VideoSendStream对象


```JavaScript
 

VideoSendStream::VideoSendStream(
    Clock* clock,
    int num_cpu_cores,
    ProcessThread* module_process_thread,
    TaskQueueFactory* task_queue_factory,
    CallStats* call_stats,
    RtpTransportControllerSendInterface* transport,
    BitrateAllocatorInterface* bitrate_allocator,
    SendDelayStats* send_delay_stats,
    RtcEventLog* event_log,
    VideoSendStream::Config config,
    VideoEncoderConfig encoder_config,
    const std::map<uint32_t, RtpState>& suspended_ssrcs,
    const std::map<uint32_t, RtpPayloadState>& suspended_payload_states,
    std::unique_ptr<FecController> fec_controller)
    : worker_queue_(transport->GetWorkerQueue()),
      stats_proxy_(clock, config, encoder_config.content_type),
      config_(std::move(config)),
      content_type_(encoder_config.content_type) {
  RTC_DCHECK(config_.encoder_settings.encoder_factory);
  RTC_DCHECK(config_.encoder_settings.bitrate_allocator_factory);
  // 编码器的流的初始化
  video_stream_encoder_ = CreateVideoStreamEncoder(clock, task_queue_factory, num_cpu_cores,
                               &stats_proxy_, config_.encoder_settings);
  // TODO(srte): Initialization should not be done posted on a task queue.
  // Note that the posted task must not outlive this scope since the closure
  // references local variables.
  worker_queue_->PostTask(ToQueuedTask(
      [this, clock, call_stats, transport, bitrate_allocator, send_delay_stats,
       event_log, &suspended_ssrcs, &encoder_config, &suspended_payload_states,
       &fec_controller]() {
        send_stream_.reset(new VideoSendStreamImpl(
            clock, &stats_proxy_, worker_queue_, call_stats, transport,
            bitrate_allocator, send_delay_stats, video_stream_encoder_.get(),
            event_log, &config_, encoder_config.max_bitrate_bps,
            encoder_config.bitrate_priority, suspended_ssrcs,
            suspended_payload_states, encoder_config.content_type,
            std::move(fec_controller), config_.media_transport));
      },
      [this]() { thread_sync_event_.Set(); }));

  // Wait for ConstructionTask to complete so that |send_stream_| can be used.
  // |module_process_thread| must be registered and deregistered on the thread
  // it was created on.
  thread_sync_event_.Wait(rtc::Event::kForever);
  send_stream_->RegisterProcessThread(module_process_thread);
  // TODO(sprang): Enable this also for regular video calls by default, if it
  // works well.
  if (encoder_config.content_type == VideoEncoderConfig::ContentType::kScreen ||
      field_trial::IsEnabled(kTargetBitrateRtcpFieldTrial)) {
    video_stream_encoder_->SetBitrateAllocationObserver(send_stream_.get());
  }

  ReconfigureVideoEncoder(std::move(encoder_config));
}
```


## 3 异步创建VideoSendStreamImpl对象

```javascript 


VideoSendStreamImpl::VideoSendStreamImpl(
    Clock* clock,
    SendStatisticsProxy* stats_proxy,
    rtc::TaskQueue* worker_queue,
    CallStats* call_stats,
    RtpTransportControllerSendInterface* transport,
    BitrateAllocatorInterface* bitrate_allocator,
    SendDelayStats* send_delay_stats,
    VideoStreamEncoderInterface* video_stream_encoder,
    RtcEventLog* event_log,
    const VideoSendStream::Config* config,
    int initial_encoder_max_bitrate,
    double initial_encoder_bitrate_priority,
    std::map<uint32_t, RtpState> suspended_ssrcs,
    std::map<uint32_t, RtpPayloadState> suspended_payload_states,
    VideoEncoderConfig::ContentType content_type,
    std::unique_ptr<FecController> fec_controller,
    MediaTransportInterface* media_transport)
    : clock_(clock),
      has_alr_probing_(config->periodic_alr_bandwidth_probing ||
                       GetAlrSettings(content_type)),
      pacing_config_(PacingConfig()),
      stats_proxy_(stats_proxy),
      config_(config),
      worker_queue_(worker_queue),
      timed_out_(false),
      call_stats_(call_stats),
      transport_(transport),
      bitrate_allocator_(bitrate_allocator),
      disable_padding_(true),
      max_padding_bitrate_(0),
      encoder_min_bitrate_bps_(0),
      encoder_target_rate_bps_(0),
      encoder_bitrate_priority_(initial_encoder_bitrate_priority),
      has_packet_feedback_(false),
      video_stream_encoder_(video_stream_encoder),
      encoder_feedback_(clock, config_->rtp.ssrcs, video_stream_encoder),
      bandwidth_observer_(transport->GetBandwidthObserver()),
      rtp_video_sender_(transport_->CreateRtpVideoSender(
          suspended_ssrcs,
          suspended_payload_states,
          config_->rtp,
          config_->rtcp_report_interval_ms,
          config_->send_transport,
          CreateObservers(call_stats,
                          &encoder_feedback_,
                          stats_proxy_,
                          send_delay_stats),
          event_log,
          std::move(fec_controller),
          CreateFrameEncryptionConfig(config_))),
      weak_ptr_factory_(this),
      media_transport_(media_transport) {
  RTC_DCHECK_RUN_ON(worker_queue_);
  RTC_LOG(LS_INFO) << "VideoSendStreamInternal: " << config_->ToString();
  weak_ptr_ = weak_ptr_factory_.GetWeakPtr();

  encoder_feedback_.SetRtpVideoSender(rtp_video_sender_);

  if (media_transport_) {
    // The configured ssrc is interpreted as a channel id, so there must be
    // exactly one.
    RTC_DCHECK_EQ(config_->rtp.ssrcs.size(), 1);
    media_transport_->SetKeyFrameRequestCallback(&encoder_feedback_);
  } else {
    RTC_DCHECK(!config_->rtp.ssrcs.empty());
  }
  RTC_DCHECK(call_stats_);
  RTC_DCHECK(transport_);
  RTC_DCHECK_NE(initial_encoder_max_bitrate, 0);

  if (initial_encoder_max_bitrate > 0) {
    encoder_max_bitrate_bps_ =
        rtc::dchecked_cast<uint32_t>(initial_encoder_max_bitrate);
  } else {
    // TODO(srte): Make sure max bitrate is not set to negative values. We don't
    // have any way to handle unset values in downstream code, such as the
    // bitrate allocator. Previously -1 was implicitly casted to UINT32_MAX, a
    // behaviour that is not safe. Converting to 10 Mbps should be safe for
    // reasonable use cases as it allows adding the max of multiple streams
    // without wrappping around.
    const int kFallbackMaxBitrateBps = 10000000;
    RTC_DLOG(LS_ERROR) << "ERROR: Initial encoder max bitrate = "
                       << initial_encoder_max_bitrate << " which is <= 0!";
    RTC_DLOG(LS_INFO) << "Using default encoder max bitrate = 10 Mbps";
    encoder_max_bitrate_bps_ = kFallbackMaxBitrateBps;
  }

  RTC_CHECK(AlrExperimentSettings::MaxOneFieldTrialEnabled());
  // If send-side BWE is enabled, check if we should apply updated probing and
  // pacing settings.
 /* static FILE * out_stream_ptr = fopen("./test_enable.log", "wb+");
  if (out_stream_ptr)
  {
	  fprintf(out_stream_ptr, "[config = %s]\n", config_-> ToString().c_str());
	  fflush(out_stream_ptr);
  }*/
  if (TransportSeqNumExtensionConfigured(*config_)) {
    has_packet_feedback_ = true;

    absl::optional<AlrExperimentSettings> alr_settings = GetAlrSettings(content_type);
    if (alr_settings) {
		/*if (out_stream_ptr)
		{
			fprintf(out_stream_ptr, "[content_type = %u]\n", content_type);
			fflush(out_stream_ptr);
		}*/
      transport->EnablePeriodicAlrProbing(true);
      transport->SetPacingFactor(alr_settings->pacing_factor);
      configured_pacing_factor_ = alr_settings->pacing_factor;
      transport->SetQueueTimeLimit(alr_settings->max_paced_queue_time);
    } else {
		/*if (out_stream_ptr)
		{
			fprintf(out_stream_ptr, "[alr][content_type = %u]\n", content_type);
			fflush(out_stream_ptr);
		}*/
      RateControlSettings rate_control_settings =
          RateControlSettings::ParseFromFieldTrials();

      transport->EnablePeriodicAlrProbing(
          rate_control_settings.UseAlrProbing());
      const double pacing_factor =
          rate_control_settings.GetPacingFactor().value_or(
              pacing_config_.pacing_factor);
      transport->SetPacingFactor(pacing_factor);
      configured_pacing_factor_ = pacing_factor;
      transport->SetQueueTimeLimit(pacing_config_.max_pacing_delay.Get().ms());
    }
  }

  if (config_->periodic_alr_bandwidth_probing) 
  {
	  /*  if (out_stream_ptr)
		{
			fprintf(out_stream_ptr, "[periodic_alr_bandwidth_probing]EnablePeriodicAlrProbing \n");
			fflush(out_stream_ptr);
		}*/
    transport->EnablePeriodicAlrProbing(true);
  }

  RTC_DCHECK_GE(config_->rtp.payload_type, 0);
  RTC_DCHECK_LE(config_->rtp.payload_type, 127);

  // 注册网络带宽回调函数
  video_stream_encoder_->SetStartBitrate(bitrate_allocator_->GetStartBitrate(this));

  // Only request rotation at the source when we positively know that the remote
  // side doesn't support the rotation extension. This allows us to prepare the
  // encoder in the expectation that rotation is supported - which is the common
  // case.
  bool rotation_applied = absl::c_none_of(
      config_->rtp.extensions, [](const RtpExtension& extension) {
        return extension.uri == RtpExtension::kVideoRotationUri;
      });

  video_stream_encoder_->SetSink(this, rotation_applied);
}
```

## 4、CreateRtpVideoSender 创建 RtpVideoSender 对象保存RtpTransportControllerSend中video_rtp_senders_数组中去

```javascript 


RtpVideoSenderInterface* RtpTransportControllerSend::CreateRtpVideoSender(
    std::map<uint32_t, RtpState> suspended_ssrcs,
    const std::map<uint32_t, RtpPayloadState>& states,
    const RtpConfig& rtp_config,
    int rtcp_report_interval_ms,
    Transport* send_transport,
    const RtpSenderObservers& observers,
    RtcEventLog* event_log,
    std::unique_ptr<FecController> fec_controller,
    const RtpSenderFrameEncryptionConfig& frame_encryption_config) {
  video_rtp_senders_.push_back(absl::make_unique<RtpVideoSender>(
      clock_, suspended_ssrcs, states, rtp_config, rtcp_report_interval_ms,
      send_transport, observers,
      // TODO(holmer): Remove this circular dependency by injecting
      // the parts of RtpTransportControllerSendInterface that are really used.
      this, event_log, &retransmission_rate_limiter_, std::move(fec_controller),
      frame_encryption_config.frame_encryptor,
      frame_encryption_config.crypto_options));
  return video_rtp_senders_.back().get();
}
```


## 5、 创建FlexfecSender对象


### ① RtpVideoSender

```javascript 

RtpVideoSender::RtpVideoSender(
    Clock* clock,
    std::map<uint32_t, RtpState> suspended_ssrcs,
    const std::map<uint32_t, RtpPayloadState>& states,
    const RtpConfig& rtp_config,
    int rtcp_report_interval_ms,
    Transport* send_transport,
    const RtpSenderObservers& observers,
    RtpTransportControllerSendInterface* transport,
    RtcEventLog* event_log,
    RateLimiter* retransmission_limiter,
    std::unique_ptr<FecController> fec_controller,
    FrameEncryptorInterface* frame_encryptor,
    const CryptoOptions& crypto_options)
    : send_side_bwe_with_overhead_(
          webrtc::field_trial::IsEnabled("WebRTC-SendSideBwe-WithOverhead")),
      account_for_packetization_overhead_(!webrtc::field_trial::IsDisabled(
          "WebRTC-SubtractPacketizationOverhead")),
      active_(false),
      module_process_thread_(nullptr),
      suspended_ssrcs_(std::move(suspended_ssrcs)),
      flexfec_sender_(
          MaybeCreateFlexfecSender(clock, rtp_config, suspended_ssrcs_)),
      fec_controller_(std::move(fec_controller)),
      rtp_streams_(
          CreateRtpStreamSenders(clock,
                                 rtp_config,
                                 rtcp_report_interval_ms,
                                 send_transport,
                                 observers.intra_frame_callback,
                                 observers.rtcp_loss_notification_observer,
                                 transport->GetBandwidthObserver(),
                                 transport,
                                 observers.rtcp_rtt_stats,
                                 flexfec_sender_.get(),
                                 observers.bitrate_observer,
                                 observers.rtcp_type_observer,
                                 observers.send_delay_observer,
                                 observers.send_packet_observer,
                                 event_log,
                                 retransmission_limiter,
                                 this,
                                 frame_encryptor,
                                 crypto_options)),
      rtp_config_(rtp_config),
      transport_(transport),
      transport_overhead_bytes_per_packet_(0),
      overhead_bytes_per_packet_(0),
      encoder_target_rate_bps_(0),
      frame_counts_(rtp_config.ssrcs.size()),
      frame_count_observer_(observers.frame_count_observer) {
  RTC_DCHECK_EQ(rtp_config.ssrcs.size(), rtp_streams_.size());
  module_process_thread_checker_.Detach();
  // SSRCs are assumed to be sorted in the same order as |rtp_modules|.
  for (uint32_t ssrc : rtp_config.ssrcs) {
    // Restore state if it previously existed.
    const RtpPayloadState* state = nullptr;
    auto it = states.find(ssrc);
    if (it != states.end()) {
      state = &it->second;
      shared_frame_id_ = std::max(shared_frame_id_, state->shared_frame_id);
    }
    params_.push_back(RtpPayloadParams(ssrc, state));
  }

  // RTP/RTCP initialization.

  // We add the highest spatial layer first to ensure it'll be prioritized
  // when sending padding, with the hope that the packet rate will be smaller,
  // and that it's more important to protect than the lower layers.

  // TODO(nisse): Consider moving registration with PacketRouter last, after the
  // modules are fully configured.
  for (const RtpStreamSender& stream : rtp_streams_) {
    constexpr bool remb_candidate = true;
    transport->packet_router()->AddSendRtpModule(stream.rtp_rtcp.get(),
                                                 remb_candidate);
  }

  for (size_t i = 0; i < rtp_config_.extensions.size(); ++i) {
    const std::string& extension = rtp_config_.extensions[i].uri;
    int id = rtp_config_.extensions[i].id;
    RTC_DCHECK(RtpExtension::IsSupportedForVideo(extension));
    for (const RtpStreamSender& stream : rtp_streams_) {
      RTC_CHECK(stream.rtp_rtcp->RegisterRtpHeaderExtension(extension, id));
    }
  }

  ConfigureProtection(rtp_config);
  ConfigureSsrcs(rtp_config);
  ConfigureRids(rtp_config);

  if (!rtp_config.mid.empty()) 
  {
    for (const RtpStreamSender& stream : rtp_streams_) 
	{
      stream.rtp_rtcp->SetMid(rtp_config.mid);
    }
  }

  for (const RtpStreamSender& stream : rtp_streams_) 
  {
    // Simulcast has one module for each layer. Set the CNAME on all modules.
    stream.rtp_rtcp->SetCNAME(rtp_config.c_name.c_str());
    stream.rtp_rtcp->RegisterRtcpStatisticsCallback(observers.rtcp_stats);
    stream.rtp_rtcp->RegisterSendChannelRtpStatisticsCallback( observers.rtp_stats);
    stream.rtp_rtcp->SetMaxRtpPacketSize(rtp_config.max_packet_size);
    stream.rtp_rtcp->RegisterSendPayloadFrequency(rtp_config.payload_type, kVideoPayloadTypeFrequency);
	// TODO@chensong 注册编码器的网络发送类型 [96, H264]
    stream.sender_video->RegisterPayloadType(rtp_config.payload_type, rtp_config.payload_name);
  }
  // Currently, both ULPFEC and FlexFEC use the same FEC rate calculation logic,
  // so enable that logic if either of those FEC schemes are enabled.
  // 选择抗丢包三种模式 [Fec, NackFec, Nack] 模式
  fec_controller_->SetProtectionMethod(FecEnabled(), NackEnabled());

  fec_controller_->SetProtectionCallback(this);
  // Signal congestion controller this object is ready for OnPacket* callbacks.
   // 信号拥塞控制器此对象已准备好进行OnPacket*回调。
  if (fec_controller_->UseLossVectorMask()) 
  {
	  // 20250325 接收一个就回调OnPacketAdded和OnPacketFeedbackVector 函数
    transport_->RegisterPacketFeedbackObserver(this);
  }
}
```

### ②  创建FlexfecSender 

```javascript 


// TODO(brandtr): Update this function when we support multistream protection.
std::unique_ptr<FlexfecSender> MaybeCreateFlexfecSender(
    Clock* clock,
    const RtpConfig& rtp,
    const std::map<uint32_t, RtpState>& suspended_ssrcs) {
  if (rtp.flexfec.payload_type < 0) {
    return nullptr;
  }
  RTC_DCHECK_GE(rtp.flexfec.payload_type, 0);
  RTC_DCHECK_LE(rtp.flexfec.payload_type, 127);
  if (rtp.flexfec.ssrc == 0) {
    RTC_LOG(LS_WARNING) << "FlexFEC is enabled, but no FlexFEC SSRC given. "
                           "Therefore disabling FlexFEC.";
    return nullptr;
  }
  if (rtp.flexfec.protected_media_ssrcs.empty()) {
    RTC_LOG(LS_WARNING)
        << "FlexFEC is enabled, but no protected media SSRC given. "
           "Therefore disabling FlexFEC.";
    return nullptr;
  }

  if (rtp.flexfec.protected_media_ssrcs.size() > 1) {
    RTC_LOG(LS_WARNING)
        << "The supplied FlexfecConfig contained multiple protected "
           "media streams, but our implementation currently only "
           "supports protecting a single media stream. "
           "To avoid confusion, disabling FlexFEC completely.";
    return nullptr;
  }

  const RtpState* rtp_state = nullptr;
  auto it = suspended_ssrcs.find(rtp.flexfec.ssrc);
  if (it != suspended_ssrcs.end()) {
    rtp_state = &it->second;
  }

  RTC_DCHECK_EQ(1U, rtp.flexfec.protected_media_ssrcs.size());
  return absl::make_unique<FlexfecSender>(
      rtp.flexfec.payload_type, rtp.flexfec.ssrc,
      rtp.flexfec.protected_media_ssrcs[0], rtp.mid, rtp.extensions,
      RTPSender::FecExtensionSizes(), rtp_state, clock);
}



FlexfecSender::FlexfecSender(
    int payload_type,
    uint32_t ssrc,
    uint32_t protected_media_ssrc,
    const std::string& mid,
    const std::vector<RtpExtension>& rtp_header_extensions,
    rtc::ArrayView<const RtpExtensionSize> extension_sizes,
    const RtpState* rtp_state,
    Clock* clock)
    : clock_(clock),
      random_(clock_->TimeInMicroseconds()),
      last_generated_packet_ms_(-1),
      payload_type_(payload_type),
      // Reset RTP state if this is not the first time we are operating.
      // Otherwise, randomize the initial timestamp offset and RTP sequence
      // numbers. (This is not intended to be cryptographically strong.)
      timestamp_offset_(rtp_state ? rtp_state->start_timestamp
                                  : random_.Rand<uint32_t>()),
      ssrc_(ssrc),
      protected_media_ssrc_(protected_media_ssrc),
      mid_(mid),
      seq_num_(rtp_state ? rtp_state->sequence_number
                         : random_.Rand(1, kMaxInitRtpSeqNumber)),
      ulpfec_generator_(
          ForwardErrorCorrection::CreateFlexfec(ssrc, protected_media_ssrc)),
      rtp_header_extension_map_(
          RegisterSupportedExtensions(rtp_header_extensions)),
      header_extensions_size_(
          RtpHeaderExtensionSize(extension_sizes, rtp_header_extension_map_)) {
  // This object should not have been instantiated if FlexFEC is disabled.
  RTC_DCHECK_GE(payload_type, 0);
  RTC_DCHECK_LE(payload_type, 127);
}

```

# 二、 FecControllerDefault类评估出现FEC的保护比率



modules\video_coding/fec_controller_default.h


FEC 输入参数

1. 目标码流 
2. 带宽  
3. 丢包率 
4. rtt  

FEC 输出参数

1. 

根据RtpVideoSender提供的目标码率、帧率、丢包率等实时参数，结合网络带宽估计（BWE）结果，动态计算FEC保护比率

‌运行时更新‌

接收来自RtpVideoSender的码率更新事件
调用FecControllerDefault生成最新FEC保护比率‌  [delta_params key_params] rate_fec = > [0, 255]
通过VideoFecGenerator接口调整冗余包生成速率‌
![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/1189916737144dffbfaa485431eb7e74.png)




RtpTransportControllerSend 网络定时评估后调用PostUpdates函数 目标码流大于0时调用UpdateControlState函数 触发observer_->OnTargetTransferRate函数

调用Call成员函数 Call::OnTargetTransferRate  函数中调用带宽改变函数bitrate_allocator_->OnNetworkChanged触发FEC计算函数



***<font color='red'>UpdateFecRates 方法是在feedback反馈gcc评估带宽后调用的函数</font>***

在BitrateAllocator类两种情况调用

1. 增加一个发送发送流程AddObserver 接口调用
2. 网络改变 OnNetworkChanged 接口调用

###  ① 增加一个发送发送流程AddObserver 接口调用

```javascript 


void BitrateAllocator::AddObserver(BitrateAllocatorObserver* observer,
                                   MediaStreamAllocationConfig config) {
  RTC_DCHECK_RUN_ON(&sequenced_checker_);
  RTC_DCHECK_GT(config.bitrate_priority, 0);
  RTC_DCHECK(std::isnormal(config.bitrate_priority));
  auto it = FindObserverConfig(observer);

  // Update settings if the observer already exists, create a new one otherwise.
  if (it != bitrate_observer_configs_.end()) {
    it->min_bitrate_bps = config.min_bitrate_bps;
    it->max_bitrate_bps = config.max_bitrate_bps;
    it->pad_up_bitrate_bps = config.pad_up_bitrate_bps;
    it->enforce_min_bitrate = config.enforce_min_bitrate;
    it->bitrate_priority = config.bitrate_priority;
  } else {
    bitrate_observer_configs_.push_back(ObserverConfig(
        observer, config.min_bitrate_bps, config.max_bitrate_bps,
        config.pad_up_bitrate_bps, config.priority_bitrate_bps,
        config.enforce_min_bitrate, config.track_id, config.bitrate_priority));
  }

  if (last_target_bps_ > 0) 
  {
    // Calculate a new allocation and update all observers.
	  // 计算新的分配并更新所有观察者   平均分配带宽 observer上去
    ObserverAllocation allocation = AllocateBitrates(last_target_bps_);
    ObserverAllocation bandwidth_allocation = AllocateBitrates(last_link_capacity_bps_);
    for (auto& config : bitrate_observer_configs_) 
	{
      uint32_t allocated_bitrate = allocation[config.observer];
      uint32_t bandwidth = bandwidth_allocation[config.observer];
      BitrateAllocationUpdate update;
      update.target_bitrate = DataRate::bps(allocated_bitrate);
      update.link_capacity = DataRate::bps(bandwidth);
      update.packet_loss_ratio = last_fraction_loss_ / 256.0;
      update.round_trip_time = TimeDelta::ms(last_rtt_);
      // 已弃用，请改用链路容量分配
      update.bwe_period = TimeDelta::ms(last_bwe_period_ms_);
      uint32_t protection_bitrate = config.observer->OnBitrateUpdated(update);
      config.allocated_bitrate_bps = allocated_bitrate;
      if (allocated_bitrate > 0) 
	  {
        config.media_ratio = MediaRatio(allocated_bitrate, protection_bitrate);
	  }
    }
  } else {
    // Currently, an encoder is not allowed to produce frames.
    // But we still have to return the initial config bitrate + let the
    // observer know that it can not produce frames.

    BitrateAllocationUpdate update;
    update.target_bitrate = DataRate::Zero();
    update.link_capacity = DataRate::Zero();
    update.packet_loss_ratio = last_fraction_loss_ / 256.0;
    update.round_trip_time = TimeDelta::ms(last_rtt_);
    update.bwe_period = TimeDelta::ms(last_bwe_period_ms_);
    observer->OnBitrateUpdated(update);
  }
  UpdateAllocationLimits();
}

```




### ② RtpTransportControllerSend 网络评估后调用PostUpdates函数


```javascript 


void RtpTransportControllerSend::PostUpdates(NetworkControlUpdate update) {
  if (update.congestion_window)
  {
	  if (update.congestion_window->IsFinite())
	  {
			pacer_.SetCongestionWindow(update.congestion_window->bytes());
	  }
	  else
	  {
            pacer_.SetCongestionWindow(PacedSender::kNoCongestionWindow);
	  }
  }
  if (update.pacer_config) 
  {
    pacer_.SetPacingRates(update.pacer_config->data_rate().bps(), update.pacer_config->pad_rate().bps());
  }
  for (const auto& probe : update.probe_cluster_configs) 
  {
    int64_t bitrate_bps = probe.target_data_rate.bps();
    pacer_.CreateProbeCluster(bitrate_bps, probe.id);
  }
  // 目标码流大于0 触发 FEC函数 UpdateControlState - > Call  observer_->OnTargetTransferRate(*update);
  if (update.target_rate) 
  {
    control_handler_->SetTargetRate(*update.target_rate);
    UpdateControlState();
  }
}



void RtpTransportControllerSend::UpdateControlState() {
  absl::optional<TargetTransferRate> update = control_handler_->GetUpdate();
  if (!update)
    return;
  retransmission_rate_limiter_.SetMaxRate(
      update->network_estimate.bandwidth.bps());
  // We won't create control_handler_ until we have an observers.
  RTC_DCHECK(observer_ != nullptr);
  observer_->OnTargetTransferRate(*update);
}

void Call::OnTargetTransferRate(TargetTransferRate msg) {
  // TODO(bugs.webrtc.org/9719)
  // Call::OnTargetTransferRate requires that on target transfer rate is invoked
  // from the worker queue (because bitrate_allocator_ requires it). Media
  // transport does not guarantee the callback on the worker queue.
  // When the threading model for MediaTransportInterface is update, reconsider
  // changing this implementation.
  if (!transport_send_ptr_->GetWorkerQueue()->IsCurrent()) {
    transport_send_ptr_->GetWorkerQueue()->PostTask(
        [this, msg] { this->OnTargetTransferRate(msg); });
    return;
  }

  uint32_t target_bitrate_bps = msg.target_rate.bps();
  int loss_ratio_255 = msg.network_estimate.loss_rate_ratio * 255;
  uint8_t fraction_loss =
      rtc::dchecked_cast<uint8_t>(rtc::SafeClamp(loss_ratio_255, 0, 255));
  int64_t rtt_ms = msg.network_estimate.round_trip_time.ms();
  int64_t probing_interval_ms = msg.network_estimate.bwe_period.ms();
  uint32_t bandwidth_bps = msg.network_estimate.bandwidth.bps();
  {
    rtc::CritScope cs(&last_bandwidth_bps_crit_);
    last_bandwidth_bps_ = bandwidth_bps;
  }
  // For controlling the rate of feedback messages.
  receive_side_cc_.OnBitrateChanged(target_bitrate_bps);
  // 触发FEC 控制类 传入 1. 目标码流 2. 带宽  3. 丢包率  4. rtt  5. bwe ms
  bitrate_allocator_->OnNetworkChanged(target_bitrate_bps, bandwidth_bps,
                                       fraction_loss, rtt_ms,
                                       probing_interval_ms);

  // Ignore updates if bitrate is zero (the aggregate network state is down).
  if (target_bitrate_bps == 0) {
    rtc::CritScope lock(&bitrate_crit_);
    estimated_send_bitrate_kbps_counter_.ProcessAndPause();
    pacer_bitrate_kbps_counter_.ProcessAndPause();
    return;
  }

  bool sending_video;
  {
    ReadLockScoped read_lock(*send_crit_);
    sending_video = !video_send_streams_.empty();
  }

  rtc::CritScope lock(&bitrate_crit_);
  if (!sending_video) {
    // Do not update the stats if we are not sending video.
    estimated_send_bitrate_kbps_counter_.ProcessAndPause();
    pacer_bitrate_kbps_counter_.ProcessAndPause();
    return;
  }
  estimated_send_bitrate_kbps_counter_.Add(target_bitrate_bps / 1000);
  // Pacer bitrate may be higher than bitrate estimate if enforcing min bitrate.
  uint32_t pacer_bitrate_bps =
      std::max(target_bitrate_bps, min_allocated_send_bitrate_bps_);
  pacer_bitrate_kbps_counter_.Add(pacer_bitrate_bps / 1000);
}

```




### ③ 网络改变 OnNetworkChanged 接口 

```javascript 


void BitrateAllocator::OnNetworkChanged(uint32_t target_bitrate_bps,
                                        uint32_t link_capacity_bps,
                                        uint8_t fraction_loss,
                                        int64_t rtt,
                                        int64_t bwe_period_ms) {
  RTC_DCHECK_RUN_ON(&sequenced_checker_);
  last_target_bps_ = target_bitrate_bps;
  last_link_capacity_bps_ = link_capacity_bps;
  last_non_zero_bitrate_bps_ =
      target_bitrate_bps > 0 ? target_bitrate_bps : last_non_zero_bitrate_bps_;
  last_fraction_loss_ = fraction_loss;
  last_rtt_ = rtt;
  last_bwe_period_ms_ = bwe_period_ms;

  // Periodically log the incoming BWE.
  int64_t now = clock_->TimeInMilliseconds();
  if (now > last_bwe_log_time_ + kBweLogIntervalMs) {
    RTC_LOG(LS_INFO) << "Current BWE " << target_bitrate_bps;
    last_bwe_log_time_ = now;
  }

  ObserverAllocation allocation = AllocateBitrates(target_bitrate_bps);
  ObserverAllocation bandwidth_allocation = AllocateBitrates(link_capacity_bps);

  for (auto& config : bitrate_observer_configs_) {
    uint32_t allocated_bitrate = allocation[config.observer];
    uint32_t allocated_bandwidth = bandwidth_allocation[config.observer];
    BitrateAllocationUpdate update;
    update.target_bitrate = DataRate::bps(allocated_bitrate);
    update.link_capacity = DataRate::bps(allocated_bandwidth);
    update.packet_loss_ratio = last_fraction_loss_ / 256.0;
    update.round_trip_time = TimeDelta::ms(last_rtt_);
    update.bwe_period = TimeDelta::ms(last_bwe_period_ms_);
    uint32_t protection_bitrate = config.observer->OnBitrateUpdated(update);

    if (allocated_bitrate == 0 && config.allocated_bitrate_bps > 0) {
      if (target_bitrate_bps > 0)
        ++num_pause_events_;
      // The protection bitrate is an estimate based on the ratio between media
      // and protection used before this observer was muted.
      uint32_t predicted_protection_bps =
          (1.0 - config.media_ratio) * config.min_bitrate_bps;
      RTC_LOG(LS_INFO) << "Pausing observer " << config.observer
                       << " with configured min bitrate "
                       << config.min_bitrate_bps << " and current estimate of "
                       << target_bitrate_bps << " and protection bitrate "
                       << predicted_protection_bps;
    } else if (allocated_bitrate > 0 && config.allocated_bitrate_bps == 0) {
      if (target_bitrate_bps > 0)
        ++num_pause_events_;
      RTC_LOG(LS_INFO) << "Resuming observer " << config.observer
                       << ", configured min bitrate " << config.min_bitrate_bps
                       << ", current allocation " << allocated_bitrate
                       << " and protection bitrate " << protection_bitrate;
    }

    // Only update the media ratio if the observer got an allocation.
    if (allocated_bitrate > 0)
      config.media_ratio = MediaRatio(allocated_bitrate, protection_bitrate);
    config.allocated_bitrate_bps = allocated_bitrate;
  }
  UpdateAllocationLimits();
}
```

### ④  调用对应VideoSendStreamImpl类 视频流接口OnBitrateUpdated



```javascript 


uint32_t VideoSendStreamImpl::OnBitrateUpdated(BitrateAllocationUpdate update) {
  RTC_DCHECK_RUN_ON(worker_queue_);
  RTC_DCHECK(rtp_video_sender_->IsActive())
      << "VideoSendStream::Start has not been called.";
  // 向fec控制器中发送当前网络 情况    1. 码流 2. 丢包率  3. rtt  4. 发送视频帧率 
  rtp_video_sender_->OnBitrateUpdated(
      update.target_bitrate.bps(),
      rtc::dchecked_cast<uint8_t>(update.packet_loss_ratio * 256),
      update.round_trip_time.ms(), stats_proxy_->GetSendFrameRate());


  encoder_target_rate_bps_ = rtp_video_sender_->GetPayloadBitrateBps();
  const uint32_t protection_bitrate_bps =
      rtp_video_sender_->GetProtectionBitrateBps();
  DataRate link_allocation = DataRate::Zero();
  if (encoder_target_rate_bps_ > protection_bitrate_bps) {
    link_allocation =
        DataRate::bps(encoder_target_rate_bps_ - protection_bitrate_bps);
  }
  encoder_target_rate_bps_ =
      std::min(encoder_max_bitrate_bps_, encoder_target_rate_bps_);

  DataRate encoder_target_rate = DataRate::bps(encoder_target_rate_bps_);
  link_allocation = std::max(encoder_target_rate, link_allocation);
  video_stream_encoder_->OnBitrateUpdated(
      encoder_target_rate, link_allocation,
      rtc::dchecked_cast<uint8_t>(update.packet_loss_ratio * 256),
      update.round_trip_time.ms());
  stats_proxy_->OnSetEncoderTargetRate(encoder_target_rate_bps_);
  return protection_bitrate_bps;
}

```

### ⑤  FEC控制类中的UpdateFecRates方法计算出 FEC丢包率


 ```javascript

uint32_t FecControllerDefault::UpdateFecRates(
    uint32_t estimated_bitrate_bps,
    int actual_framerate_fps,
    uint8_t fraction_lost,
    std::vector<bool> loss_mask_vector,
    int64_t round_trip_time_ms) {
  float target_bitrate_kbps =
      static_cast<float>(estimated_bitrate_bps) / 1000.0f;
  // Sanity check.
  if (actual_framerate_fps < 1.0) {
    actual_framerate_fps = 1.0;
  }
  FecProtectionParams delta_fec_params;
  FecProtectionParams key_fec_params;
  {
    CritScope lock(&crit_sect_);
    loss_prot_logic_->UpdateBitRate(target_bitrate_kbps);
    loss_prot_logic_->UpdateRtt(round_trip_time_ms);
    // Update frame rate for the loss protection logic class: frame rate should
    // be the actual/sent rate.
    loss_prot_logic_->UpdateFrameRate(actual_framerate_fps);
    // Returns the filtered packet loss, used for the protection setting.
    // The filtered loss may be the received loss (no filter), or some
    // filtered value (average or max window filter).
    // Use max window filter for now.
    media_optimization::FilterPacketLossMode filter_mode =
        media_optimization::kMaxFilter;
	// 取附近10s内最大丢包率 ， 把当然丢包率保存丢包率数组中取
    uint8_t packet_loss_enc = loss_prot_logic_->FilteredLoss(clock_->TimeInMilliseconds(), filter_mode, fraction_lost);
    // For now use the filtered loss for computing the robustness settings.
	// 目前，使用滤波损失来计算鲁棒性设置。
    loss_prot_logic_->UpdateFilteredLossPr(packet_loss_enc);
	// 判断一些当前fec模式  [Fec, FecNack, Nack]
    if (loss_prot_logic_->SelectedType() == media_optimization::kNone)
	{
      return estimated_bitrate_bps;
    }
    // Update method will compute the robustness settings for the given
    // protection method and the overhead cost
    // the protection method is set by the user via SetVideoProtection.
    loss_prot_logic_->UpdateMethod();
    // Get the bit cost of protection method, based on the amount of
    // overhead data actually transmitted (including headers) the last
    // second.
    // Get the FEC code rate for Key frames (set to 0 when NA).
	// 确保FEC码率不超过总带宽的30%：
    key_fec_params.fec_rate = loss_prot_logic_->SelectedMethod()->RequiredProtectionFactorK();
    // Get the FEC code rate for Delta frames (set to 0 when NA).
    delta_fec_params.fec_rate = loss_prot_logic_->SelectedMethod()->RequiredProtectionFactorD();
    // The RTP module currently requires the same |max_fec_frames| for both
    // key and delta frames.
    delta_fec_params.max_fec_frames = loss_prot_logic_->SelectedMethod()->MaxFramesFec();
    key_fec_params.max_fec_frames = loss_prot_logic_->SelectedMethod()->MaxFramesFec();
  }
  // Set the FEC packet mask type. |kFecMaskBursty| is more effective for
  // consecutive losses and little/no packet re-ordering. As we currently
  // do not have feedback data on the degree of correlated losses and packet
  // re-ordering, we keep default setting to |kFecMaskRandom| for now.
  //设置FEC数据包掩码类型|kFecMaskBursty |对以下情况更有效
  //连续丢失和很少/没有数据包重新排序。正如我们目前所做的那样
  //没有关于相关丢失和数据包程度的反馈数据
  //重新排序后，我们暂时将默认设置保留为|kFecMaskRandom|。
  delta_fec_params.fec_mask_type = kFecMaskRandom;
  key_fec_params.fec_mask_type = kFecMaskRandom;
  // Update protection callback with protection settings.
  uint32_t sent_video_rate_bps = 0;
  uint32_t sent_nack_rate_bps = 0;
  uint32_t sent_fec_rate_bps = 0;
  // Rate cost of the protection methods.
  float protection_overhead_rate = 0.0f;
  // TODO(Marco): Pass FEC protection values per layer.
  protection_callback_->ProtectionRequest(
      &delta_fec_params, &key_fec_params, &sent_video_rate_bps,
      &sent_nack_rate_bps, &sent_fec_rate_bps);
  uint32_t sent_total_rate_bps =
      sent_video_rate_bps + sent_nack_rate_bps + sent_fec_rate_bps;
  // Estimate the overhead costs of the next second as staying the same
  // wrt the source bitrate.
  if (sent_total_rate_bps > 0) {
    protection_overhead_rate =
        static_cast<float>(sent_nack_rate_bps + sent_fec_rate_bps) /
        sent_total_rate_bps;
  }
  // Cap the overhead estimate to a threshold, default is 50%.
  protection_overhead_rate =
      std::min(protection_overhead_rate, overhead_threshold_);
  // Source coding rate: total rate - protection overhead.
  return estimated_bitrate_bps * (1.0 - protection_overhead_rate);
}
```



### ⑥   Fec中VCMFecMethod 的方法ProtectionFactor 计算出key_params和delta_params的FEC丢包率


```javascript 


bool VCMFecMethod::ProtectionFactor(const VCMProtectionParameters* parameters) {
  // FEC PROTECTION SETTINGS: varies with packet loss and bitrate

  // No protection if (filtered) packetLoss is 0
  uint8_t packetLoss = rtc::saturated_cast<uint8_t>(255 * parameters->lossPr);
  if (packetLoss == 0) {
    _protectionFactorK = 0;
    _protectionFactorD = 0;
    return true;
  }

  // Parameters for FEC setting:
  // first partition size, thresholds, table pars, spatial resoln fac.
  // 第一分区大小、阈值、表部分、空间分辨率fac。
  // First partition protection: ~ 20%
  // 第一分区保护： ~20%
  uint8_t firstPartitionProt = rtc::saturated_cast<uint8_t>(255 * 0.20);

  // Minimum protection level needed to generate one FEC packet for one
  // source packet/frame (in RTP sender)
  //生成一个FEC数据包所需的最低保护级别
  //源数据包/帧（在RTP发送器中）
  uint8_t minProtLevelFec = 85;

  // Threshold on packetLoss and bitRrate/frameRate (=average #packets),
  // above which we allocate protection to cover at least first partition.
  //分组丢失阈值和比特率/帧率（=平均分组数），
  //超过该阈值，我们分配保护以覆盖至少第一个分区。
  uint8_t lossThr = 0;
  uint8_t packetNumThr = 1;

  // Parameters for range of rate index of table.
  /// 表速率指标范围参数
  const uint8_t ratePar1 = 5;
  const uint8_t ratePar2 = 49;

  // Spatial resolution size, relative to a reference size.
  // 空间分辨率大小，相对于参考大小
  float spatialSizeToRef = rtc::saturated_cast<float>(parameters->codecWidth *
                                                      parameters->codecHeight) /
                           (rtc::saturated_cast<float>(704 * 576));
  // resolnFac: This parameter will generally increase/decrease the FEC rate
  // (for fixed bitRate and packetLoss) based on system size.
  // Use a smaller exponent (< 1) to control/soften system size effect.
  // resolnFac：此参数通常会增加/减少FEC速率
  //（适用于固定比特率和丢包率）基于系统大小。
  //使用较小的指数（<1）来控制/软化系统大小效应
  const float resolnFac = 1.0 / powf(spatialSizeToRef, 0.3f);

  // 根据 目标码率和帧率  计算出一帧的码流
  // 每像素比特数（BPP）模型平衡视频质量 = 目标码率 / (分辨率 * 帧率)
  const int bitRatePerFrame = BitsPerFrame(parameters);

  // Average number of packets per frame (source and fec):
  // 计算每帧的平均数据包数（源和fec）：
  const uint8_t avgTotPackets = rtc::saturated_cast<uint8_t>(
      1.5f + rtc::saturated_cast<float>(bitRatePerFrame) * 1000.0f /rtc::saturated_cast<float>(8.0 * _maxPayloadSize));

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 1. 查表映射公式
  // 根据实时丢包率（Loss Rate）从kFecRateTable中获取基础冗余度
  // BaseFecRate = Interpolate(kFecRateTable,LossRate)
  // 其中Interpolate为线性插值函数，当丢包率介于表项之间时按比例加权计算
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // FEC rate parameters: for P and I frame
  // FEC速率参数：适用于P和I帧
  uint8_t codeRateDelta = 0;
  uint8_t codeRateKey = 0;

  // Get index for table: the FEC protection depends on an effective rate.
  // The range on the rate index corresponds to rates (bps)
  // from ~200k to ~8000k, for 30fps
  //获取表索引：FEC保护取决于有效速率。
  //费率指数上的范围对应于费率（bps）
  //从200k到8000k，帧率为30fps
  //////////////////////////////////////////////////////////////
  //  根据 分辨率 [702 * 576]  计算出当前视频分辨率[width * height] =====>  一帧需要码流的大小
  ///////////////////////////////////////
  const uint16_t effRateFecTable = rtc::saturated_cast<uint16_t>(resolnFac * bitRatePerFrame);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 丢包率下标表
  uint8_t rateIndexTable = rtc::saturated_cast<uint8_t>(VCM_MAX(VCM_MIN((effRateFecTable - ratePar1) / ratePar1, ratePar2), 0));

  // Restrict packet loss range to 50:
  // current tables defined only up to 50%
  //将数据包丢失范围限制为50:
  //当前表仅定义了高达50%
  if (packetLoss >= kPacketLossMax) 
  {
    packetLoss = kPacketLossMax - 1;
  }
  
  uint16_t indexTable = rateIndexTable * kPacketLossMax + packetLoss;

  // Check on table index
  RTC_DCHECK_LT(indexTable, kFecRateTableSize);

  // Protection factor for P frame
  // P帧保护系数
  codeRateDelta = kFecRateTable[indexTable];

  if (packetLoss > lossThr && avgTotPackets > packetNumThr) 
  {
    // Set a minimum based on first partition size.
    //根据第一个分区大小设置最小值。
    if (codeRateDelta < firstPartitionProt) 
	{
      codeRateDelta = firstPartitionProt;
    }
  }

  // Check limit on amount of protection for P frame; 50% is max.
  // 检查P帧的保护量限制；50%为最大值
  if (codeRateDelta >= kPacketLossMax) 
  {
    codeRateDelta = kPacketLossMax - 1;
  }

  // For Key frame:
  // Effectively at a higher rate, so we scale/boost the rate
  // The boost factor may depend on several factors: ratio of packet
  // number of I to P frames, how much protection placed on P frames, etc.
  /// 对于关键帧：
    //有效地以更高的速度进行，因此我们扩大/提高了速度
    //增强因子可能取决于几个因素：数据包的比率I帧到P帧的数量、对P帧的保护程度等。
  const uint8_t packetFrameDelta = rtc::saturated_cast<uint8_t>(0.5 + parameters->packetsPerFrame);
  const uint8_t packetFrameKey = rtc::saturated_cast<uint8_t>(0.5 + parameters->packetsPerFrameKey);
  const uint8_t boostKey = BoostCodeRateKey(packetFrameDelta, packetFrameKey);

  rateIndexTable = rtc::saturated_cast<uint8_t>(VCM_MAX(
      VCM_MIN(1 + (boostKey * effRateFecTable - ratePar1) / ratePar1, ratePar2), 
	  0));
  uint16_t indexTableKey = rateIndexTable * kPacketLossMax + packetLoss;

  indexTableKey = VCM_MIN(indexTableKey, kFecRateTableSize);

  // Check on table index
  assert(indexTableKey < kFecRateTableSize);

  // Protection factor for I frame
  codeRateKey = kFecRateTable[indexTableKey];

  // Boosting for Key frame.
  // 关键帧增强
  int boostKeyProt = _scaleProtKey * codeRateDelta;
  if (boostKeyProt >= kPacketLossMax) {
    boostKeyProt = kPacketLossMax - 1;
  }

  // Make sure I frame protection is at least larger than P frame protection,
  // and at least as high as filtered packet loss.
  codeRateKey = rtc::saturated_cast<uint8_t>(VCM_MAX(packetLoss, VCM_MAX(boostKeyProt, codeRateKey)));

  // Check limit on amount of protection for I frame: 50% is max.
  if (codeRateKey >= kPacketLossMax) {
    codeRateKey = kPacketLossMax - 1;
  }

  _protectionFactorK = codeRateKey;
  _protectionFactorD = codeRateDelta;

  // Generally there is a rate mis-match between the FEC cost estimated
  // in mediaOpt and the actual FEC cost sent out in RTP module.
  // This is more significant at low rates (small # of source packets), where
  // the granularity of the FEC decreases. In this case, non-zero protection
  // in mediaOpt may generate 0 FEC packets in RTP sender (since actual #FEC
  // is based on rounding off protectionFactor on actual source packet number).
  // The correction factor (_corrFecCost) attempts to corrects this, at least
  // for cases of low rates (small #packets) and low protection levels.
  //通常，估计的FEC成本之间存在费率不匹配
  //在mediaOpt中输入FEC成本，在RTP模块中发送实际FEC成本。
  //这在低速率（源数据包数量少）下更为重要，其中
  // FEC的粒度减小。在这种情况下，非零保护
  // in-mediaOpt可能会在RTP发送器中生成0个FEC数据包（因为实际#FEC
  //基于实际源数据包编号的四舍五入protectionFactor）。
  //修正系数（_corrFecCost）试图修正这一点，至少
  //适用于低速率（小数据包）和低保护级别的情况。
  float numPacketsFl =
      1.0f + (rtc::saturated_cast<float>(bitRatePerFrame) * 1000.0 /
                  rtc::saturated_cast<float>(8.0 * _maxPayloadSize) +
              0.5);

  const float estNumFecGen =
      0.5f +
      rtc::saturated_cast<float>(_protectionFactorD * numPacketsFl / 255.0f);

  // We reduce cost factor (which will reduce overhead for FEC and
  // hybrid method) and not the protectionFactor.
  _corrFecCost = 1.0f;
  if (estNumFecGen < 1.1f && _protectionFactorD < minProtLevelFec) {
    _corrFecCost = 0.5f;
  }
  if (estNumFecGen < 0.9f && _protectionFactorD < minProtLevelFec) {
    _corrFecCost = 0.0f;
  }

  // DONE WITH FEC PROTECTION SETTINGS
  return true;
}
```

### ⑦ 调用RtpVideoSender类中ProtectionRequest方法分别RTPSenderVideo类SetFecParameters的转入key_params和delta_params的FEC丢包率

 RTPSenderVideo::SetFecParameters


```javascript
int RtpVideoSender::ProtectionRequest(const FecProtectionParams* delta_params,
                                      const FecProtectionParams* key_params,
                                      uint32_t* sent_video_rate_bps,
                                      uint32_t* sent_nack_rate_bps,
                                      uint32_t* sent_fec_rate_bps) {
  *sent_video_rate_bps = 0;
  *sent_nack_rate_bps = 0;
  *sent_fec_rate_bps = 0;
  for (const RtpStreamSender& stream : rtp_streams_) {
    uint32_t not_used = 0;
    uint32_t module_nack_rate = 0;
    stream.sender_video->SetFecParameters(*delta_params, *key_params);
    *sent_video_rate_bps += stream.sender_video->VideoBitrateSent();
    *sent_fec_rate_bps += stream.sender_video->FecOverheadRate();
    stream.rtp_rtcp->BitrateSent(&not_used, /*video_rate=*/nullptr,
                                 /*fec_rate=*/nullptr, &module_nack_rate);
    *sent_nack_rate_bps += module_nack_rate;
  }
  return 0;
}


void RTPSenderVideo::SetFecParameters(const FecProtectionParams& delta_params,
                                      const FecProtectionParams& key_params) {
  rtc::CritScope cs(&crit_);
  delta_fec_params_ = delta_params;
  key_fec_params_ = key_params;
}
```


# 三、 视频编码后的数据调用RTPSenderVideo类方法SendVideo发送





## 1、发送视频接口 SendVideo  

```javascript

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

    if (i == 0) 
	{
		// 20250324@chensong  设置Jitterbuffer的的大小   解码延迟 PlayeDelay rtt 3 max 
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


## 2、 发送SendVideoPacketWithFlexfec接口

发送总数据 = rtp原数据+fec(rtp原数据)


1. 发送rtp原数据
2. 发送 fec(rtp)的数据

```javascript


void RTPSenderVideo::SendVideoPacketWithFlexfec(
    std::unique_ptr<RtpPacketToSend> media_packet,
    StorageType media_packet_storage,
    bool protect_media_packet) {
  RTC_DCHECK(flexfec_sender_);

  if (protect_media_packet)
  {
    flexfec_sender_->AddRtpPacketAndGenerateFec(*media_packet);
  }

  // 1. 发送rtp原数据
  SendVideoPacket(std::move(media_packet), media_packet_storage);
  // 2. 发送 fec(rtp)的数据
  if (flexfec_sender_->FecAvailable()) {
    std::vector<std::unique_ptr<RtpPacketToSend>> fec_packets =
        flexfec_sender_->GetFecPackets();
    for (auto& fec_packet : fec_packets) {
      size_t packet_length = fec_packet->size();
      uint16_t seq_num = fec_packet->SequenceNumber();
      if (LogAndSendToNetwork(std::move(fec_packet), kDontRetransmit,
                              RtpPacketSender::kHighPriority)) {
        rtc::CritScope cs(&stats_crit_);
        fec_bitrate_.Update(packet_length, clock_->TimeInMilliseconds());
      } else {
        RTC_LOG(LS_WARNING) << "Failed to send FlexFEC packet " << seq_num;
      }
    }
  }
}
```


## 3、AddRtpPacketAndGenerateFec 判断包数量是否达到fec最大rtp包的数量 然后调用fec中EncodeFec方法编码


****<font color='red'>判断包数量是否达到fec最大rtp包的数量 然后调用fec中EncodeFec方法加密</font>***

 ```javascript 

int UlpfecGenerator::AddRtpPacketAndGenerateFec(const uint8_t* data_buffer,
                                                size_t payload_length,
                                                size_t rtp_header_length) {
  RTC_DCHECK(generated_fec_packets_.empty());
  if (media_packets_.empty()) {
    params_ = new_params_;
  }
  // 示例：4 原始块 + 2 冗余块（可容忍 2 个丢包）
  // int k = 4, m = 2;
  // 冗余比例 = m/(k+m) = 33%，需权衡带宽与抗丢包能力
  // 20250324 TODO@chensong 找到M 和 K的值
  bool complete_frame = false;
  // rtp包 mask  seq 开始包  标志位 
  /*
  
bool FrameMarkingExtension::Write(rtc::ArrayView<uint8_t> data,
                                  const FrameMarking& frame_marking) {
  RTC_DCHECK_GE(data.size(), 1);
  RTC_CHECK_LE(frame_marking.temporal_id, 0x07);
  data[0] = frame_marking.start_of_frame ? 0x80 : 0x00;
  data[0] |= frame_marking.end_of_frame ? 0x40 : 0x00;
  data[0] |= frame_marking.independent_frame ? 0x20 : 0x00;
  data[0] |= frame_marking.discardable_frame ? 0x10 : 0x00;

  if (IsScalable(frame_marking.temporal_id, frame_marking.layer_id)) {
    RTC_DCHECK_EQ(data.size(), 3);
    data[0] |= frame_marking.base_layer_sync ? 0x08 : 0x00;
    data[0] |= frame_marking.temporal_id & 0x07;
    data[1] = frame_marking.layer_id;
    data[2] = frame_marking.tl0_pic_idx;
  }
  return true;
}
  */
  const bool marker_bit = (data_buffer[1] & kRtpMarkerBitMask) ? true : false;
  if (media_packets_.size() < kUlpfecMaxMediaPackets) {
    // Our packet masks can only protect up to |kUlpfecMaxMediaPackets| packets.
    std::unique_ptr<ForwardErrorCorrection::Packet> packet(
        new ForwardErrorCorrection::Packet());
    packet->length = payload_length + rtp_header_length;
    memcpy(packet->data, data_buffer, packet->length);
    media_packets_.push_back(std::move(packet));
    // Keep track of the RTP header length, so we can copy the RTP header
    // from |packet| to newly generated ULPFEC+RED packets.
    RTC_DCHECK_GE(rtp_header_length, kRtpHeaderSize);
    last_media_packet_rtp_header_length_ = rtp_header_length;
  }
  if (marker_bit) {
    ++num_protected_frames_;
    complete_frame = true;
  }
  // Produce FEC over at most |params_.max_fec_frames| frames, or as soon as:
  // (1) the excess overhead (actual overhead - requested/target overhead) is
  // less than |kMaxExcessOverhead|, and
  // (2) at least |min_num_media_packets_| media packets is reached.
  // 必须是  包第一个rtp  
  if (complete_frame &&
      (num_protected_frames_ == params_.max_fec_frames ||
       (ExcessOverheadBelowMax() && MinimumMediaPacketsReached()))) {
    // We are not using Unequal Protection feature of the parity erasure code.
    constexpr int kNumImportantPackets = 0;
    constexpr bool kUseUnequalProtection = false;
    int ret = fec_->EncodeFec(media_packets_, params_.fec_rate,
                              kNumImportantPackets, kUseUnequalProtection,
                              params_.fec_mask_type, &generated_fec_packets_);
    if (generated_fec_packets_.empty()) {
      ResetState();
    }
    return ret;
  }
  return 0;
}
```


## 4、 取fec加密后数据 调用FlexfecSender的GetFecPackets方法获取fec编码后的数据包

```javascript


std::vector<std::unique_ptr<RtpPacketToSend>> FlexfecSender::GetFecPackets() {
  std::vector<std::unique_ptr<RtpPacketToSend>> fec_packets_to_send;
  fec_packets_to_send.reserve(ulpfec_generator_.generated_fec_packets_.size());
  for (const auto* fec_packet : ulpfec_generator_.generated_fec_packets_) {
    std::unique_ptr<RtpPacketToSend> fec_packet_to_send(
        new RtpPacketToSend(&rtp_header_extension_map_));
    fec_packet_to_send->set_is_fec(true);

    // RTP header.
    fec_packet_to_send->SetMarker(false);
    fec_packet_to_send->SetPayloadType(payload_type_);
    fec_packet_to_send->SetSequenceNumber(seq_num_++);
    fec_packet_to_send->SetTimestamp(
        timestamp_offset_ +
        static_cast<uint32_t>(kMsToRtpTimestamp *
                              clock_->TimeInMilliseconds()));
    // Set "capture time" so that the TransmissionOffset header extension
    // can be set by the RTPSender.
    fec_packet_to_send->set_capture_time_ms(clock_->TimeInMilliseconds());
    fec_packet_to_send->SetSsrc(ssrc_);
    // Reserve extensions, if registered. These will be set by the RTPSender.
    fec_packet_to_send->ReserveExtension<AbsoluteSendTime>();
    fec_packet_to_send->ReserveExtension<TransmissionOffset>();
    fec_packet_to_send->ReserveExtension<TransportSequenceNumber>();
    // Possibly include the MID header extension.
    if (!mid_.empty()) {
      // This is a no-op if the MID header extension is not registered.
      fec_packet_to_send->SetExtension<RtpMid>(mid_);
    }

    // RTP payload.
    uint8_t* payload = fec_packet_to_send->AllocatePayload(fec_packet->length);
    memcpy(payload, fec_packet->data, fec_packet->length);

    fec_packets_to_send.push_back(std::move(fec_packet_to_send));
  }
  ulpfec_generator_.ResetState();

  int64_t now_ms = clock_->TimeInMilliseconds();
  if (!fec_packets_to_send.empty() &&
      now_ms - last_generated_packet_ms_ > kPacketLogIntervalMs) {
    RTC_LOG(LS_VERBOSE) << "Generated " << fec_packets_to_send.size()
                        << " FlexFEC packets with payload type: "
                        << payload_type_ << " and SSRC: " << ssrc_ << ".";
    last_generated_packet_ms_ = now_ms;
  }

  return fec_packets_to_send;
}

```


# 总结



 
[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)