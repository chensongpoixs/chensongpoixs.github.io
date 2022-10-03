---
layout:     post
title:      WebRTC之证书certificate生成的时机分析
subtitle:   WebRTC、Certificate
date:       2022-10-04
times:      03::24::33
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - 媒体协商
---


# WebRTC之证书(certificate)生成的时机分析


@[TOC](WebRTC之证书生成的时机分析)

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

# 一、WebRTC中证书生成的时机图

 
![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2022-10-04/webrtc_certificate_callback.jpg?raw=true)


WebRTC中证书是在工作线程中生成的 

分为两种情况

1. 使用默认(config)证书的流程
2. 创建证书的流程


# 二、使用默认的证书

在调用PeerconnectionFactroy中的方法CreateConnection方法第一个参数转入config中certificates字段有没有附上值

 ```cpp
 struct RTC_EXPORT RTCConfiguration {
 ...
   // TODO(pthatcher): Rename this ice_transport_type, but update
    // Chromium at the same time.
    IceTransportsType type = kAll;
    BundlePolicy bundle_policy = kBundlePolicyBalanced;
    RtcpMuxPolicy rtcp_mux_policy = kRtcpMuxPolicyRequire;
    std::vector<rtc::scoped_refptr<rtc::RTCCertificate>> certificates; // 证书
    int ice_candidate_pool_size = 0;
 ...
 };
```


```cpp


void WebRtcSessionDescriptionFactory::OnMessage(rtc::Message* msg) {
  switch (msg->message_id) 
  {
    case MSG_CREATE_SESSIONDESCRIPTION_SUCCESS: {
	//TODO@chensong 2022-10-04  给用户态的的certificate的信息哈  
      CreateSessionDescriptionMsg* param = static_cast<CreateSessionDescriptionMsg*>(msg->pdata);
      param->observer->OnSuccess(param->description.release());
      delete param;
      break;
    }
    case MSG_CREATE_SESSIONDESCRIPTION_FAILED: {
      CreateSessionDescriptionMsg* param =
          static_cast<CreateSessionDescriptionMsg*>(msg->pdata);
      param->observer->OnFailure(std::move(param->error));
      delete param;
      break;
    }
    case MSG_USE_CONSTRUCTOR_CERTIFICATE: 
	{
		// 默认证书就之间回调函数哈
      rtc::ScopedRefMessageData<rtc::RTCCertificate>* param =
          static_cast<rtc::ScopedRefMessageData<rtc::RTCCertificate>*>(
              msg->pdata);
      RTC_LOG(LS_INFO) << "Using certificate supplied to the constructor.";
      SetCertificate(param->data());
      delete param;
      break;
    }
    default:
      RTC_NOTREACHED();
      break;
  }
}
```

# 三、创建证书的流程


## 1、创建WebRtcCertificateGeneratorCallback类

创建一个中间WebRtcCertificateGeneratorCallback类作为 证书任务创建成功回调用

在WebRtcCertificateGeneratorCallback类中其实啥都没有呢 就有两个信号槽分别是证书创建成功的回调函数和证书创建失败回调函数

```cpp


// DTLS certificate request callback class.
class WebRtcCertificateGeneratorCallback
    : public rtc::RTCCertificateGeneratorCallback,
      public sigslot::has_slots<> {
 public:
  // |rtc::RTCCertificateGeneratorCallback| overrides.
  void OnSuccess(
      const rtc::scoped_refptr<rtc::RTCCertificate>& certificate) override;
  void OnFailure() override;

  sigslot::signal0<> SignalRequestFailed; // 证书创建失败回调函数
  sigslot::signal1<const rtc::scoped_refptr<rtc::RTCCertificate>&>
      SignalCertificateReady;//证书创建成功回调函数
};

void WebRtcCertificateGeneratorCallback::OnFailure() {
  SignalRequestFailed();
}

void WebRtcCertificateGeneratorCallback::OnSuccess(
    const rtc::scoped_refptr<rtc::RTCCertificate>& certificate) {
  SignalCertificateReady(certificate);
}

```

##  2、 创建生成证书任务RTCCertificateGenerationTask类
 
生成证书是RTCCertificateGenerationTask类中 在工作线程中操作

在工作线程中创建证书， 在信号线程转发证书给上次应用



在WebRtcCertificateGeneratorCallback有一个证书生成的成功信号槽最终调用WebRtcSessionDescriptionFactory::SetCertificate方法

```cpp

// static
scoped_refptr<RTCCertificate> RTCCertificateGenerator::GenerateCertificate(
    const KeyParams& key_params,
    const absl::optional<uint64_t>& expires_ms) {
  if (!key_params.IsValid()) {
    return nullptr;
  }

  SSLIdentity* identity = nullptr;
  //TODO@chensong 2022-20-04 WebRTC中证书默认是没有时间限制的
  if (!expires_ms) 
  {
    identity = SSLIdentity::Generate(kIdentityName, key_params);
  } else {
    uint64_t expires_s = *expires_ms / 1000;
    // Limit the expiration time to something reasonable (a year). This was
    // somewhat arbitrarily chosen. It also ensures that the value is not too
    // large for the unspecified |time_t|.
	// TODO@chensong 2022-10-04 最小的时间
    expires_s = std::min(expires_s, kYearInSeconds);
    // TODO(torbjorng): Stop using |time_t|, its type is unspecified. It it safe
    // to assume it can hold up to a year's worth of seconds (and more), but
    // |SSLIdentity::Generate| should stop relying on |time_t|.
    // See bugs.webrtc.org/5720.
    time_t cert_lifetime_s = static_cast<time_t>(expires_s);
    identity = SSLIdentity::GenerateWithExpiration(kIdentityName, key_params,
                                                   cert_lifetime_s);
  }
  if (!identity) {
    return nullptr;
  }
  std::unique_ptr<SSLIdentity> identity_sptr(identity);
  return RTCCertificate::Create(std::move(identity_sptr));
}
// Helper class for generating certificates asynchronously; a single task
// instance is responsible for a single asynchronous certificate generation
// request. We are using a separate helper class so that a generation request
// can outlive the |RTCCertificateGenerator| that spawned it.
class RTCCertificateGenerationTask : public RefCountInterface,
                                     public MessageHandler {
 public:
  RTCCertificateGenerationTask(
      Thread* signaling_thread,
      Thread* worker_thread,
      const KeyParams& key_params,
      const absl::optional<uint64_t>& expires_ms,
      const scoped_refptr<RTCCertificateGeneratorCallback>& callback)
      : signaling_thread_(signaling_thread),
        worker_thread_(worker_thread),
        key_params_(key_params),
        expires_ms_(expires_ms),
        callback_(callback) {
    RTC_DCHECK(signaling_thread_);
    RTC_DCHECK(worker_thread_);
    RTC_DCHECK(callback_);
  }
  ~RTCCertificateGenerationTask() override {}

  // Handles |MSG_GENERATE| and its follow-up |MSG_GENERATE_DONE|.
  void OnMessage(Message* msg) override {
    switch (msg->message_id) {
      case MSG_GENERATE: {
        RTC_DCHECK(worker_thread_->IsCurrent());
        // Perform the certificate generation work here on the worker thread.
        certificate_ = RTCCertificateGenerator::GenerateCertificate(
            key_params_, expires_ms_);
        // Handle callbacks on signaling thread. Pass on the |msg->pdata|
        // (which references |this| with ref counting) to that thread.
		// TODO@chensong 2022-10-04 证书创建好了 切换到信号线程转发给上层应用
        signaling_thread_->Post(RTC_FROM_HERE, this, MSG_GENERATE_DONE,
                                msg->pdata);
        break;
      }
      case MSG_GENERATE_DONE: 
	  {
        RTC_DCHECK(signaling_thread_->IsCurrent());
        // Perform callback with result here on the signaling thread.
        // TODO@chensong 2022-03-25 这边开始创建好本地WebRTC的证书的信息
        // 进行回调webrtc_session_destion_factory中信息哈 ^_^ 
		// TODO@chensong 2022-10-04 信号线程证书转发上层应用
        if (certificate_) 
		{
		// TODO @chensong 2022-10-04 这边其实调用代理类WebRtcCertificateGeneratorCallback中方法OnSuccess 
			// 在WebRtcCertificateGeneratorCallback有一个证书生成的成功信号槽最终调用WebRtcSessionDescriptionFactory::SetCertificate方法
          callback_->OnSuccess(certificate_);
        }
		else 
		{
          callback_->OnFailure();
        }
        // Destroy |msg->pdata| which references |this| with ref counting. This
        // may result in |this| being deleted - do not touch member variables
        // after this line.
        delete msg->pdata;
        return;
      }
      default:
        RTC_NOTREACHED();
    }
  }

 private:
  Thread* const signaling_thread_; // 信号线程
  Thread* const worker_thread_; // 工作线程
  const KeyParams key_params_;
  const absl::optional<uint64_t> expires_ms_; // 证书的过期时间
  const scoped_refptr<RTCCertificateGeneratorCallback> callback_; //工作线程生成证书后， 信号线程调用该回调传给上层应用的证书
  scoped_refptr<RTCCertificate> certificate_; // 证书
};
```


## 3、WebRtcSessionDescriptionFactory::SetCertificate类中保存证书


```cpp

void WebRtcSessionDescriptionFactory::SetCertificate(const rtc::scoped_refptr<rtc::RTCCertificate>& certificate) 
{
  RTC_DCHECK(certificate);
  RTC_LOG(LS_VERBOSE) << "Setting new certificate.";
  // TODO@chensong 2022-03-25  创建本地的WebRTC的证书 的信息
  // 改变 certificate的状态 的变化
  certificate_request_state_ = CERTIFICATE_SUCCEEDED;
  // TODO@chensong 2022-03-25 这个代码是啥意思呢  ->>>>>>>   为啥 设置jsep_trsport_controller 的本地 证书呢 
  SignalCertificateReady(certificate);

  transport_desc_factory_.set_certificate(certificate);
  transport_desc_factory_.set_secure(cricket::SEC_ENABLED);
  // TODO@chensong 202209-29 一般情况下是没有数据的， 原因是只有的在应用层在生成证书之前调用创建Offer或者创建Answer的接口，
  //  create_session_description_requests_队列中才有数据，
  while (!create_session_description_requests_.empty()) 
  {
    if (create_session_description_requests_.front().type == CreateSessionDescriptionRequest::kOffer) 
	{
      InternalCreateOffer(create_session_description_requests_.front());
    }
	else 
	{
      InternalCreateAnswer(create_session_description_requests_.front());
    }
    create_session_description_requests_.pop();
  }
}
```



# 总结

[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)


