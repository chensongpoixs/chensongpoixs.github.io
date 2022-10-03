
---
layout:     post
title:      WebRTC媒体协商之CreatePeerConnectionFactory、CreatePeerConnection、CreateOffer
subtitle:   媒体协商
date:       2022-01-09
times:       01::03::36
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - 网络编程
    - SDP
---

# WebRTC媒体协商之CreatePeerConnectionFactory、CreatePeerConnection、CreateOffer


@[TOC](WebRTC媒体协商之CreatePeerConnectionFactory、CreatePeerConnection、CreateOffer)

</font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">


<font color='red'>WebRTC专题开嗨鸭 ！！！  </font>
 

一、 WebRTC 线程模型

[1、WebRTC中线程模型和常见线程模型介绍](https://chensongpoixs.github.io/2021/12/11/WebRTC%E4%B8%AD%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E5%92%8C%E5%B8%B8%E8%A7%81%E7%BA%BF%E7%A8%8B%E6%A8%A1%E5%9E%8B%E4%BB%8B%E7%BB%8D/#/)

[2、WebRTC网络PhysicalSocketServer之WSAEventselect模型使用](https://chensongpoixs.github.io/2022/01/02/WebRTC%E7%BD%91%E7%BB%9CPhysicalSocketServer%E4%B9%8BWSAEventselect%E6%A8%A1%E5%9E%8B%E4%BD%BF%E7%94%A8/)

二、 WebRTC媒体协商

[1、WebRTC媒体协商之SDP中JsepSessionDescription类结构分析](https://chensongpoixs.github.io/2022/04/17/WebRTC%E5%AA%92%E4%BD%93%E5%8D%8F%E5%95%86%E4%B9%8BSDP%E4%B8%ADJsepSessionDescription%E7%B1%BB%E7%BB%93%E6%9E%84%E5%88%86%E6%9E%90/)
[2、WebRTC媒体协商之CreatePeerConnectionFactory、CreatePeerConnection、CreateOffer]()

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

<font color=#999AAA >WebRTC是音视频行业的标杆， 如果要学习音视频， WebRTC是进入音视频行业最好方法， 里面可以有成熟方案， 例如：音频中3A 算法、网络评估、自适应码流、Simulcast、SVC等等 ， 非常适合刚刚进入音视频行业小伙伴哈^_^  我也是哦， 以后再音视频行业长期打算的小伙伴的学习项目。 里面有大量知识点  </font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

<font color=#999AAA >提示：以下是本篇文章正文内容，下面案例可供参考

# 一、CreatePeerConnectionFactory

peerconnectionfactory中调用Initialize方法进行网络和媒体设备初始化

```cpp

rtc::scoped_refptr<PeerConnectionFactoryInterface> CreatePeerConnectionFactory(
    rtc::Thread* network_thread,
    rtc::Thread* worker_thread,
    rtc::Thread* signaling_thread,
    rtc::scoped_refptr<AudioDeviceModule> default_adm,
    rtc::scoped_refptr<AudioEncoderFactory> audio_encoder_factory,
    rtc::scoped_refptr<AudioDecoderFactory> audio_decoder_factory,
    std::unique_ptr<VideoEncoderFactory> video_encoder_factory,
    std::unique_ptr<VideoDecoderFactory> video_decoder_factory,
    rtc::scoped_refptr<AudioMixer> audio_mixer,
    rtc::scoped_refptr<AudioProcessing> audio_processing) {
	if (!audio_processing)
	{
		audio_processing = AudioProcessingBuilder().Create();
	}
	// 媒体引擎创建
  std::unique_ptr<cricket::MediaEngineInterface> media_engine =
      cricket::WebRtcMediaEngineFactory::Create(
          default_adm, audio_encoder_factory, audio_decoder_factory,
          std::move(video_encoder_factory), std::move(video_decoder_factory),
          audio_mixer, audio_processing);

  std::unique_ptr<CallFactoryInterface> call_factory = CreateCallFactory();

  std::unique_ptr<RtcEventLogFactoryInterface> event_log_factory =
      CreateRtcEventLogFactory();
  PeerConnectionFactoryDependencies dependencies;
  dependencies.network_thread = network_thread;
  dependencies.worker_thread = worker_thread;
  dependencies.signaling_thread = signaling_thread;
  dependencies.media_engine = std::move(media_engine);
  dependencies.call_factory = std::move(call_factory);
  dependencies.event_log_factory = std::move(event_log_factory);
  return CreateModularPeerConnectionFactory(std::move(dependencies));
}


// pc/peerconnectionfactory -> create
rtc::scoped_refptr<PeerConnectionFactoryInterface>
CreateModularPeerConnectionFactory(
    PeerConnectionFactoryDependencies dependencies) {
  rtc::scoped_refptr<PeerConnectionFactory> pc_factory(
      new rtc::RefCountedObject<PeerConnectionFactory>(
          std::move(dependencies)));
  // Call Initialize synchronously but make sure it is executed on
  // |signaling_thread|.
  // WebRtc 中SDK中封装 用户接口线程同步技术 -> MethodCallXXX 如果不在同一个线程中这边会卡着这边一直等到 （线程之间的通知的  notify -> wait的玩法 ） 
  MethodCall0<PeerConnectionFactory, bool> call(
      pc_factory.get(), &PeerConnectionFactory::Initialize/*看到吧 使用线程同步初始化 音频和视频的通道 */);
  bool result = call.Marshal(RTC_FROM_HERE, pc_factory->signaling_thread());

  if (!result) {
    return nullptr;
  }
  return PeerConnectionFactoryProxy::Create(pc_factory->signaling_thread(),
                                            pc_factory);
}





bool PeerConnectionFactory::Initialize() {
  RTC_DCHECK(signaling_thread_->IsCurrent());
  rtc::InitRandom(rtc::Time32());
  // 1. 网络管理类初始化
  default_network_manager_.reset(new rtc::BasicNetworkManager());
  if (!default_network_manager_) {
    return false;
  }
  // 2. 设置网络线程
  default_socket_factory_.reset(
      new rtc::BasicPacketSocketFactory(network_thread_));
  if (!default_socket_factory_) {
    return false;
  }
  // 3. 网络通道 和设置网络线程和工作线程
  channel_manager_ = absl::make_unique<cricket::ChannelManager>(
      std::move(media_engine_), absl::make_unique<cricket::RtpDataEngine>(),
      worker_thread_, network_thread_);

  channel_manager_->SetVideoRtxEnabled(true);
  // 4. 通道信息初始化
  if (!channel_manager_->Init()) {
    return false;
  }

  return true;
}


/************************************************************************/
/* 信号线程执行的                                       					*/
/************************************************************************/
bool ChannelManager::Init() {
  RTC_DCHECK(!initialized_);
  if (initialized_) {
    return false;
  }
  RTC_DCHECK(network_thread_);
  RTC_DCHECK(worker_thread_);
  // 正常情况信号线程与网络线程 所以是会设置线程
  if (!network_thread_->IsCurrent()) {
    // Do not allow invoking calls to other threads on the network thread.
	 // 不允许调用网络线程上的其他线程。
    network_thread_->Invoke<void>(
        RTC_FROM_HERE, [&] { network_thread_->DisallowBlockingCalls(); });
  }
  // 媒体信息初始化 非常重要的哈 ^_^
  if (media_engine_) {
    initialized_ = worker_thread_->Invoke<bool>(
        RTC_FROM_HERE, [&] { return media_engine_->Init(); });
    RTC_DCHECK(initialized_);
  } else {
    initialized_ = true;
  }
  return initialized_;
}

```

# 二、CreatePeerConnection

```cpp
// webrtc create peer调用的函数
rtc::scoped_refptr<PeerConnectionInterface>
PeerConnectionFactory::CreatePeerConnection(
    const PeerConnectionInterface::RTCConfiguration& configuration,
    std::unique_ptr<cricket::PortAllocator> allocator,
    std::unique_ptr<rtc::RTCCertificateGeneratorInterface> cert_generator,
    PeerConnectionObserver* observer) {
  // Convert the legacy API into the new depnedency structure.
  PeerConnectionDependencies dependencies(observer);
  dependencies.allocator = std::move(allocator);
  dependencies.cert_generator = std::move(cert_generator);
  // Pass that into the new API.
  return CreatePeerConnection(configuration, std::move(dependencies));
}

rtc::scoped_refptr<PeerConnectionInterface>
PeerConnectionFactory::CreatePeerConnection(
    const PeerConnectionInterface::RTCConfiguration& configuration,
    PeerConnectionDependencies dependencies) {
  RTC_DCHECK(signaling_thread_->IsCurrent());

  // Set internal defaults if optional dependencies are not set.
  if (!dependencies.cert_generator) {
    dependencies.cert_generator =
        absl::make_unique<rtc::RTCCertificateGenerator>(signaling_thread_,
                                                        network_thread_);
  }
  if (!dependencies.allocator) {
    network_thread_->Invoke<void>(RTC_FROM_HERE, [this, &configuration,
                                                  &dependencies]() {
      dependencies.allocator = absl::make_unique<cricket::BasicPortAllocator>(
          default_network_manager_.get(), default_socket_factory_.get(),
          configuration.turn_customizer);
    });
  }

  // TODO(zstein): Once chromium injects its own AsyncResolverFactory, set
  // |dependencies.async_resolver_factory| to a new
  // |rtc::BasicAsyncResolverFactory| if no factory is provided.

  network_thread_->Invoke<void>(
      RTC_FROM_HERE,
      rtc::Bind(&cricket::PortAllocator::SetNetworkIgnoreMask,
                dependencies.allocator.get(), options_.network_ignore_mask));

  std::unique_ptr<RtcEventLog> event_log =
      worker_thread_->Invoke<std::unique_ptr<RtcEventLog>>(
          RTC_FROM_HERE,
          rtc::Bind(&PeerConnectionFactory::CreateRtcEventLog_w, this));

  std::unique_ptr<Call> call = worker_thread_->Invoke<std::unique_ptr<Call>>(
      RTC_FROM_HERE,
      rtc::Bind(&PeerConnectionFactory::CreateCall_w, this, event_log.get()));

  rtc::scoped_refptr<PeerConnection> pc(
      new rtc::RefCountedObject<PeerConnection>(this, std::move(event_log),
                                                std::move(call)));
  ActionsBeforeInitializeForTesting(pc);
  // 这边peerconnection的初始化哈  这边我们需要关注一下
  if (!pc->Initialize(configuration, std::move(dependencies))) {
    return nullptr;
  }
  return PeerConnectionProxy::Create(signaling_thread(), pc);
}


bool PeerConnection::Initialize(
    const PeerConnectionInterface::RTCConfiguration& configuration,
    PeerConnectionDependencies dependencies) {
  RTC_DCHECK_RUN_ON(signaling_thread());
  RTC_DCHECK_RUNS_SERIALIZED(&use_media_transport_race_checker_);
  TRACE_EVENT0("webrtc", "PeerConnection::Initialize");

  RTCError config_error = ValidateConfiguration(configuration);
  if (!config_error.ok()) {
    RTC_LOG(LS_ERROR) << "Invalid configuration: " << config_error.message();
    return false;
  }

  if (!dependencies.allocator) {
    RTC_LOG(LS_ERROR)
        << "PeerConnection initialized without a PortAllocator? "
           "This shouldn't happen if using PeerConnectionFactory.";
    return false;
  }

  if (!dependencies.observer) {
    // TODO(deadbeef): Why do we do this?
    RTC_LOG(LS_ERROR) << "PeerConnection initialized without a "
                         "PeerConnectionObserver";
    return false;
  }

  observer_ = dependencies.observer;
  async_resolver_factory_ = std::move(dependencies.async_resolver_factory);
  port_allocator_ = std::move(dependencies.allocator);
  tls_cert_verifier_ = std::move(dependencies.tls_cert_verifier);

  // 找到你 stun and turn server -> 怎么玩呢  我很好奇  webrtc怎么搞这些服务的鸭
  cricket::ServerAddresses stun_servers;
  std::vector<cricket::RelayServerConfig> turn_servers;

  RTCErrorType parse_error =
      ParseIceServers(configuration.servers, &stun_servers, &turn_servers);
  if (parse_error != RTCErrorType::NONE) {
    return false;
  }

  // The port allocator lives on the network thread and should be initialized
  // there.
  const auto pa_result =
      network_thread()->Invoke<InitializePortAllocatorResult>(
          RTC_FROM_HERE,
          rtc::Bind(&PeerConnection::InitializePortAllocator_n, this,
                    stun_servers, turn_servers, configuration));

  // If initialization was successful, note if STUN or TURN servers
  // were supplied.
  if (!stun_servers.empty()) 
  {
    NoteUsageEvent(UsageEvent::STUN_SERVER_ADDED);
  }
  if (!turn_servers.empty()) {
    NoteUsageEvent(UsageEvent::TURN_SERVER_ADDED);
  }

  // Send information about IPv4/IPv6 status.
  PeerConnectionAddressFamilyCounter address_family;
  if (pa_result.enable_ipv6) {
    address_family = kPeerConnection_IPv6;
  } else {
    address_family = kPeerConnection_IPv4;
  }
  RTC_HISTOGRAM_ENUMERATION("WebRTC.PeerConnection.IPMetrics", address_family,
                            kPeerConnectionAddressFamilyCounter_Max);

  const PeerConnectionFactoryInterface::Options& options = factory_->options();

  // RFC 3264: The numeric value of the session id and version in the
  // o line MUST be representable with a "64 bit signed integer".
  // Due to this constraint session id |session_id_| is max limited to
  // LLONG_MAX.
  session_id_ = rtc::ToString(rtc::CreateRandomId64() & LLONG_MAX);
  JsepTransportController::Config config;
  config.redetermine_role_on_ice_restart = configuration.redetermine_role_on_ice_restart;
  config.ssl_max_version = factory_->options().ssl_max_version;
  config.disable_encryption = options.disable_encryption;
  config.bundle_policy = configuration.bundle_policy;
  config.rtcp_mux_policy = configuration.rtcp_mux_policy; // rtcp-mux 将 RTP 和 RTCP 复用到单一的端口进行传输，这简化了 NAT traversal，而 BUNDLE 又将多路媒体流复用到同一端口进行传输，这不仅使 candidate harvesting 等 ICE 相关的 SDP 属性变得简单，而且又进一步简化了 NAT traversal。
  // TODO(bugs.webrtc.org/9891) - Remove options.crypto_options then remove this
  // stub.
  config.crypto_options = configuration.crypto_options.has_value()
                              ? *configuration.crypto_options
                              : options.crypto_options;
  config.transport_observer = this;
  config.event_log = event_log_ptr_;
#if defined(ENABLE_EXTERNAL_AUTH)
  config.enable_external_auth = true;
#endif
  config.active_reset_srtp_params = configuration.active_reset_srtp_params;

  if (configuration.use_media_transport ||
      configuration.use_media_transport_for_data_channels) {
    if (!factory_->media_transport_factory()) {
      RTC_DCHECK(false)
          << "PeerConnecton is initialized with use_media_transport = true or "
          << "use_media_transport_for_data_channels = true "
          << "but media transport factory is not set in PeerConnectionFactory";
      return false;
    }

    if (configuration.use_media_transport ||
        configuration.use_media_transport_for_data_channels) {
      // TODO(bugs.webrtc.org/9719): This check will eventually go away, when
      // RTP media transport is introduced. But until then, we require SDES to
      // be enabled.
      if (configuration.enable_dtls_srtp.has_value() &&
          configuration.enable_dtls_srtp.value()) {
        RTC_LOG(LS_WARNING)
            << "When media transport is used, SDES must be enabled. Set "
               "configuration.enable_dtls_srtp to false. use_media_transport="
            << configuration.use_media_transport
            << ", use_media_transport_for_data_channels="
            << configuration.use_media_transport_for_data_channels;
        return false;
      }
    }

    config.use_media_transport_for_media = configuration.use_media_transport;
    config.use_media_transport_for_data_channels = configuration.use_media_transport_for_data_channels;
    config.media_transport_factory = factory_->media_transport_factory();
  }

  transport_controller_.reset(new JsepTransportController(
      signaling_thread(), network_thread(), port_allocator_.get(),
      async_resolver_factory_.get(), config));
  transport_controller_->SignalIceConnectionState.connect(
      this, &PeerConnection::OnTransportControllerConnectionState);
  transport_controller_->SignalStandardizedIceConnectionState.connect(
      this, &PeerConnection::SetStandardizedIceConnectionState);
  transport_controller_->SignalConnectionState.connect(
      this, &PeerConnection::SetConnectionState);
  transport_controller_->SignalIceGatheringState.connect(
      this, &PeerConnection::OnTransportControllerGatheringState);
  transport_controller_->SignalIceCandidatesGathered.connect(
      this, &PeerConnection::OnTransportControllerCandidatesGathered);
  transport_controller_->SignalIceCandidatesRemoved.connect(
      this, &PeerConnection::OnTransportControllerCandidatesRemoved);
  transport_controller_->SignalDtlsHandshakeError.connect(
      this, &PeerConnection::OnTransportControllerDtlsHandshakeError);

  sctp_factory_ = factory_->CreateSctpTransportInternalFactory();

  stats_.reset(new StatsCollector(this));
  stats_collector_ = RTCStatsCollector::Create(this);

  configuration_ = configuration;
  use_media_transport_ = configuration.use_media_transport;

  // Obtain a certificate from RTCConfiguration if any were provided (optional).
  rtc::scoped_refptr<rtc::RTCCertificate> certificate;
  if (!configuration.certificates.empty()) {
    // TODO(hbos,torbjorng): Decide on certificate-selection strategy instead of
    // just picking the first one. The decision should be made based on the DTLS
    // handshake. The DTLS negotiations need to know about all certificates.
    certificate = configuration.certificates[0];
  }

  transport_controller_->SetIceConfig(ParseIceConfig(configuration));

  if (options.disable_encryption) {
    dtls_enabled_ = false;
  } else {
    // Enable DTLS by default if we have an identity store or a certificate.
    dtls_enabled_ = (dependencies.cert_generator || certificate);
    // |configuration| can override the default |dtls_enabled_| value.
    if (configuration.enable_dtls_srtp) {
      dtls_enabled_ = *(configuration.enable_dtls_srtp);
    }
  }

  if (configuration.use_media_transport_for_data_channels) {
    if (configuration.enable_rtp_data_channel) {
      RTC_LOG(LS_ERROR) << "enable_rtp_data_channel and "
                           "use_media_transport_for_data_channels are "
                           "incompatible and cannot both be set to true";
      return false;
    }
    data_channel_type_ = cricket::DCT_MEDIA_TRANSPORT;
  } else if (configuration.enable_rtp_data_channel) {
    // Enable creation of RTP data channels if the kEnableRtpDataChannels is
    // set. It takes precendence over the disable_sctp_data_channels
    // PeerConnectionFactoryInterface::Options.
    data_channel_type_ = cricket::DCT_RTP;
  } else {
    // DTLS has to be enabled to use SCTP.
    if (!options.disable_sctp_data_channels && dtls_enabled_) {
      data_channel_type_ = cricket::DCT_SCTP;
    }
  }

  video_options_.screencast_min_bitrate_kbps = configuration.screencast_min_bitrate;
  audio_options_.combined_audio_video_bwe = configuration.combined_audio_video_bwe;

  audio_options_.audio_jitter_buffer_max_packets = configuration.audio_jitter_buffer_max_packets;

  audio_options_.audio_jitter_buffer_fast_accelerate = configuration.audio_jitter_buffer_fast_accelerate;

  audio_options_.audio_jitter_buffer_min_delay_ms = configuration.audio_jitter_buffer_min_delay_ms;

  audio_options_.audio_jitter_buffer_enable_rtx_handling = configuration.audio_jitter_buffer_enable_rtx_handling;

  // Whether the certificate generator/certificate is null or not determines
  // what PeerConnectionDescriptionFactory will do, so make sure that we give it
  // the right instructions by clearing the variables if needed.
  if (!dtls_enabled_) {
    dependencies.cert_generator.reset();
    certificate = nullptr;
  } else if (certificate) {
    // Favor generated certificate over the certificate generator.
    dependencies.cert_generator.reset();
  }
  // 这边 需要关注一下 SDP 的信息哈 ^_^ webrtc_sesssion_desc_factory -> createoffer -> create
  webrtc_session_desc_factory_.reset(new WebRtcSessionDescriptionFactory(
      signaling_thread(), channel_manager(), this, session_id(),
      std::move(dependencies.cert_generator), certificate, &ssrc_generator_));
  //createoffer 会用这个回调函数哈 ^_^ 真正等待ready回调函数触发 ->> 回复应用层SDP信息哈 的回调函数
  webrtc_session_desc_factory_->SignalCertificateReady.connect( this, &PeerConnection::OnCertificateReady);

  if (options.disable_encryption) {
    webrtc_session_desc_factory_->SetSdesPolicy(cricket::SEC_DISABLED);
  }

  webrtc_session_desc_factory_->set_enable_encrypted_rtp_header_extensions(
      GetCryptoOptions().srtp.enable_encrypted_rtp_header_extensions);
  webrtc_session_desc_factory_->set_is_unified_plan(IsUnifiedPlan());

  // Add default audio/video transceivers for Plan B SDP.
  if (!IsUnifiedPlan()) {
    transceivers_.push_back(
        RtpTransceiverProxyWithInternal<RtpTransceiver>::Create(signaling_thread(), new RtpTransceiver(cricket::MEDIA_TYPE_AUDIO)));
    transceivers_.push_back(
        RtpTransceiverProxyWithInternal<RtpTransceiver>::Create(signaling_thread(), new RtpTransceiver(cricket::MEDIA_TYPE_VIDEO)));
  }
  int delay_ms = return_histogram_very_quickly_ ? 0 : REPORT_USAGE_PATTERN_DELAY_MS;
  signaling_thread()->PostDelayed(RTC_FROM_HERE, delay_ms, this,
                                  MSG_REPORT_USAGE_PATTERN, nullptr);
  return true;
}


```

# 三、CreateOffer


![在这里插入图片描述](https://img-blog.csdnimg.cn/dbb90f2fe14648bc887036d8d8c8004c.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAY2hlbl9zb25nXw==,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)











```cpp

void PeerConnection::CreateOffer(CreateSessionDescriptionObserver* observer,
                                 const RTCOfferAnswerOptions& options) {
  RTC_DCHECK_RUN_ON(signaling_thread());
  TRACE_EVENT0("webrtc", "PeerConnection::CreateOffer");
  // 找到你鸭   ^_^   ^_^   真正实现媒体流数据回调的注册 哈哈 看到了 
  if (!observer) {
    RTC_LOG(LS_ERROR) << "CreateOffer - observer is NULL.";
    return;
  }

  if (IsClosed()) {
    std::string error = "CreateOffer called when PeerConnection is closed.";
    RTC_LOG(LS_ERROR) << error;
    PostCreateSessionDescriptionFailure(
        observer, RTCError(RTCErrorType::INVALID_STATE, std::move(error)));
    return;
  }

  if (!ValidateOfferAnswerOptions(options)) {
    std::string error = "CreateOffer called with invalid options.";
    RTC_LOG(LS_ERROR) << error;
    PostCreateSessionDescriptionFailure(
        observer, RTCError(RTCErrorType::INVALID_PARAMETER, std::move(error)));
    return;
  }

  // Legacy handling for offer_to_receive_audio and offer_to_receive_video.
  // Specified in WebRTC section 4.4.3.2 "Legacy configuration extensions".
  if (IsUnifiedPlan()) {
    RTCError error = HandleLegacyOfferOptions(options);
    if (!error.ok()) {
      PostCreateSessionDescriptionFailure(observer, std::move(error));
      return;
    }
  }

  cricket::MediaSessionOptions session_options;
  GetOptionsForOffer(options, &session_options);
  webrtc_session_desc_factory_->CreateOffer(observer, options, session_options);
}


void WebRtcSessionDescriptionFactory::CreateOffer(
    CreateSessionDescriptionObserver* observer,
    const PeerConnectionInterface::RTCOfferAnswerOptions& options,
    const cricket::MediaSessionOptions& session_options) {
  std::string error = "CreateOffer";
  if (certificate_request_state_ == CERTIFICATE_FAILED) {
    error += kFailedDueToIdentityFailed;
    RTC_LOG(LS_ERROR) << error;
    PostCreateSessionDescriptionFailed(observer, error);
    return;
  }

  if (!ValidMediaSessionOptions(session_options)) {
    error += " called with invalid session options";
    RTC_LOG(LS_ERROR) << error;
    PostCreateSessionDescriptionFailed(observer, error);
    return;
  }

  CreateSessionDescriptionRequest request(
      CreateSessionDescriptionRequest::kOffer, observer, session_options);
  if (certificate_request_state_ == CERTIFICATE_WAITING) {
    create_session_description_requests_.push(request);
  } else {
    RTC_DCHECK(certificate_request_state_ == CERTIFICATE_SUCCEEDED ||
               certificate_request_state_ == CERTIFICATE_NOT_NEEDED);
    InternalCreateOffer(request);
  }
}



// 这边是构造函数中注册回调函数哈 ^_^ 
void WebRtcSessionDescriptionFactory::SetCertificate(
    const rtc::scoped_refptr<rtc::RTCCertificate>& certificate) {
  RTC_DCHECK(certificate);
  RTC_LOG(LS_VERBOSE) << "Setting new certificate.";

  certificate_request_state_ = CERTIFICATE_SUCCEEDED;
  SignalCertificateReady(certificate);

  transport_desc_factory_.set_certificate(certificate);
  transport_desc_factory_.set_secure(cricket::SEC_ENABLED);

  while (!create_session_description_requests_.empty()) {
    if (create_session_description_requests_.front().type ==
        CreateSessionDescriptionRequest::kOffer) {
      InternalCreateOffer(create_session_description_requests_.front());
    } else {
      InternalCreateAnswer(create_session_description_requests_.front());
    }
    create_session_description_requests_.pop();
  }
}



WebRtcSessionDescriptionFactory::WebRtcSessionDescriptionFactory(
    rtc::Thread* signaling_thread,
    cricket::ChannelManager* channel_manager,
    PeerConnectionInternal* pc,
    const std::string& session_id,
    std::unique_ptr<rtc::RTCCertificateGeneratorInterface> cert_generator,
    const rtc::scoped_refptr<rtc::RTCCertificate>& certificate,
    UniqueRandomIdGenerator* ssrc_generator)
    : signaling_thread_(signaling_thread),
      session_desc_factory_(channel_manager,
                            &transport_desc_factory_,
                            ssrc_generator),
      // RFC 4566 suggested a Network Time Protocol (NTP) format timestamp
      // as the session id and session version. To simplify, it should be fine
      // to just use a random number as session id and start version from
      // |kInitSessionVersion|.
      session_version_(kInitSessionVersion),
      cert_generator_(std::move(cert_generator)),
      pc_(pc),
      session_id_(session_id),
      certificate_request_state_(CERTIFICATE_NOT_NEEDED) {
  RTC_DCHECK(signaling_thread_);
  RTC_DCHECK(!(cert_generator_ && certificate));
  bool dtls_enabled = cert_generator_ || certificate;
  // SRTP-SDES is disabled if DTLS is on.
  SetSdesPolicy(dtls_enabled ? cricket::SEC_DISABLED : cricket::SEC_REQUIRED);
  if (!dtls_enabled) {
    RTC_LOG(LS_VERBOSE) << "DTLS-SRTP disabled.";
    return;
  }

  if (certificate) {
    // Use |certificate|.
    certificate_request_state_ = CERTIFICATE_WAITING;

    RTC_LOG(LS_VERBOSE) << "DTLS-SRTP enabled; has certificate parameter.";
    // We already have a certificate but we wait to do |SetIdentity|; if we do
    // it in the constructor then the caller has not had a chance to connect to
    // |SignalCertificateReady|.
    signaling_thread_->Post(
        RTC_FROM_HERE, this, MSG_USE_CONSTRUCTOR_CERTIFICATE,
        new rtc::ScopedRefMessageData<rtc::RTCCertificate>(certificate));
  } else {
    // Generate certificate.
    certificate_request_state_ = CERTIFICATE_WAITING;

    rtc::scoped_refptr<WebRtcCertificateGeneratorCallback> callback(
        new rtc::RefCountedObject<WebRtcCertificateGeneratorCallback>());
    callback->SignalRequestFailed.connect(
        this, &WebRtcSessionDescriptionFactory::OnCertificateRequestFailed);
	// 这边注册回调函数哈
    callback->SignalCertificateReady.connect(
        this, &WebRtcSessionDescriptionFactory::SetCertificate);

    rtc::KeyParams key_params = rtc::KeyParams();
    RTC_LOG(LS_VERBOSE)
        << "DTLS-SRTP enabled; sending DTLS identity request (key type: "
        << key_params.type() << ").";

    // Request certificate. This happens asynchronously, so that the caller gets
    // a chance to connect to |SignalCertificateReady|.
    cert_generator_->GenerateCertificateAsync(key_params, absl::nullopt,
                                              callback);
  }
}






void WebRtcSessionDescriptionFactory::InternalCreateOffer(
    CreateSessionDescriptionRequest request) {
  if (pc_->local_description()) {
    // If the needs-ice-restart flag is set as described by JSEP, we should
    // generate an offer with a new ufrag/password to trigger an ICE restart.
    for (cricket::MediaDescriptionOptions& options :
         request.options.media_description_options) {
      if (pc_->NeedsIceRestart(options.mid)) {
        options.transport_options.ice_restart = true;
      }
    }
  }

  std::unique_ptr<cricket::SessionDescription> desc =
      session_desc_factory_.CreateOffer(
          request.options, pc_->local_description()
                               ? pc_->local_description()->description()
                               : nullptr);
  if (!desc) {
    PostCreateSessionDescriptionFailed(request.observer,
                                       "Failed to initialize the offer.");
    return;
  }

  // RFC 3264
  // When issuing an offer that modifies the session,
  // the "o=" line of the new SDP MUST be identical to that in the
  // previous SDP, except that the version in the origin field MUST
  // increment by one from the previous SDP.

  // Just increase the version number by one each time when a new offer
  // is created regardless if it's identical to the previous one or not.
  // The |session_version_| is a uint64_t, the wrap around should not happen.
  RTC_DCHECK(session_version_ + 1 > session_version_);
  auto offer = absl::make_unique<JsepSessionDescription>(
      SdpType::kOffer, std::move(desc), session_id_,
      rtc::ToString(session_version_++));
  if (pc_->local_description()) {
    for (const cricket::MediaDescriptionOptions& options :
         request.options.media_description_options) {
      if (!options.transport_options.ice_restart) {
        CopyCandidatesFromSessionDescription(pc_->local_description(),
                                             options.mid, offer.get());
      }
    }
  }
  // 这边回调 中间层后在回调应用层SDP回调哈 ^_^
  PostCreateSessionDescriptionSucceeded(request.observer, std::move(offer));
}


std::unique_ptr<SessionDescription> MediaSessionDescriptionFactory::CreateOffer(
    const MediaSessionOptions& session_options,
    const SessionDescription* current_description) const {
  // Must have options for each existing section.
  if (current_description) {
    RTC_DCHECK_LE(current_description->contents().size(),
                  session_options.media_description_options.size());
  }

  IceCredentialsIterator ice_credentials(
      session_options.pooled_ice_credentials);

  std::vector<const ContentInfo*> current_active_contents;
  if (current_description) {
    current_active_contents =
        GetActiveContents(*current_description, session_options);
  }

  StreamParamsVec current_streams =
      GetCurrentStreamParams(current_active_contents);

  AudioCodecs offer_audio_codecs;
  VideoCodecs offer_video_codecs;
  DataCodecs offer_data_codecs;
  GetCodecsForOffer(current_active_contents, &offer_audio_codecs,
                    &offer_video_codecs, &offer_data_codecs);

  if (!session_options.vad_enabled) {
    // If application doesn't want CN codecs in offer.
    StripCNCodecs(&offer_audio_codecs);
  }
  FilterDataCodecs(&offer_data_codecs,
                   session_options.data_channel_type == DCT_SCTP);

  RtpHeaderExtensions audio_rtp_extensions;
  RtpHeaderExtensions video_rtp_extensions;
  GetRtpHdrExtsToOffer(current_active_contents, &audio_rtp_extensions,
                       &video_rtp_extensions);

  auto offer = absl::make_unique<SessionDescription>();

  // Iterate through the media description options, matching with existing media
  // descriptions in |current_description|.
  size_t msection_index = 0;
  for (const MediaDescriptionOptions& media_description_options :
       session_options.media_description_options) {
    const ContentInfo* current_content = nullptr;
    if (current_description &&
        msection_index < current_description->contents().size()) {
      current_content = &current_description->contents()[msection_index];
      // Media type must match unless this media section is being recycled.
      RTC_DCHECK(current_content->name != media_description_options.mid ||
                 IsMediaContentOfType(current_content,
                                      media_description_options.type));
    }
    switch (media_description_options.type) {
      case MEDIA_TYPE_AUDIO:
        if (!AddAudioContentForOffer(
                media_description_options, session_options, current_content,
                current_description, audio_rtp_extensions, offer_audio_codecs,
                &current_streams, offer.get(), &ice_credentials)) {
          return nullptr;
        }
        break;
      case MEDIA_TYPE_VIDEO:
        if (!AddVideoContentForOffer(
                media_description_options, session_options, current_content,
                current_description, video_rtp_extensions, offer_video_codecs,
                &current_streams, offer.get(), &ice_credentials)) {
          return nullptr;
        }
        break;
      case MEDIA_TYPE_DATA:
        if (!AddDataContentForOffer(media_description_options, session_options,
                                    current_content, current_description,
                                    offer_data_codecs, &current_streams,
                                    offer.get(), &ice_credentials)) {
          return nullptr;
        }
        break;
      default:
        RTC_NOTREACHED();
    }
    ++msection_index;
  }

  // Bundle the contents together, if we've been asked to do so, and update any
  // parameters that need to be tweaked for BUNDLE.
  if (session_options.bundle_enabled) {
    ContentGroup offer_bundle(GROUP_TYPE_BUNDLE);
    for (const ContentInfo& content : offer->contents()) {
      if (content.rejected) {
        continue;
      }
      // TODO(deadbeef): There are conditions that make bundling two media
      // descriptions together illegal. For example, they use the same payload
      // type to represent different codecs, or same IDs for different header
      // extensions. We need to detect this and not try to bundle those media
      // descriptions together.
      offer_bundle.AddContentName(content.name);
    }
    if (!offer_bundle.content_names().empty()) {
      offer->AddGroup(offer_bundle);
      if (!UpdateTransportInfoForBundle(offer_bundle, offer.get())) {
        RTC_LOG(LS_ERROR)
            << "CreateOffer failed to UpdateTransportInfoForBundle.";
        return nullptr;
      }
      if (!UpdateCryptoParamsForBundle(offer_bundle, offer.get())) {
        RTC_LOG(LS_ERROR)
            << "CreateOffer failed to UpdateCryptoParamsForBundle.";
        return nullptr;
      }
    }
  }

  // The following determines how to signal MSIDs to ensure compatibility with
  // older endpoints (in particular, older Plan B endpoints).
  if (is_unified_plan_) {
    // Be conservative and signal using both a=msid and a=ssrc lines. Unified
    // Plan answerers will look at a=msid and Plan B answerers will look at the
    // a=ssrc MSID line.
    offer->set_msid_signaling(cricket::kMsidSignalingMediaSection |
                              cricket::kMsidSignalingSsrcAttribute);
  } else {
    // Plan B always signals MSID using a=ssrc lines.
    offer->set_msid_signaling(cricket::kMsidSignalingSsrcAttribute);
  }

  offer->set_extmap_allow_mixed(session_options.offer_extmap_allow_mixed);

  if (session_options.media_transport_settings.has_value()) {
    offer->AddMediaTransportSetting(
        session_options.media_transport_settings->transport_name,
        session_options.media_transport_settings->transport_setting);
  }

  return offer;
}

```



# 总结
<font color=#999AAA > 
