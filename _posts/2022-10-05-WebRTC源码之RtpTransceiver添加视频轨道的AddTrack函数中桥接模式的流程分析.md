---
layout:     post
title:      WebRTC源码之RtpTransceiver添加视频轨道的AddTrack函数中桥接模式的流程分析
subtitle:   WebRTC、RtpTransceiver
date:       2022-10-05
times:      03::24::33
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - 媒体协商
---

# WebRTC源码之RtpTransceiver添加视频轨道的AddTrack函数中桥接模式的流程分析


@[TOC](WebRTC源码之RtpTransceiver添加视频轨道的AddTrack函数中桥接模式的流程分析)

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

九、 NetEQ

十、 Simulcast与SVC


# 前言

<font color=#999AAA >WebRTC是音视频行业的标杆， 如果要学习音视频， WebRTC是进入音视频行业最好方法， 里面可以有成熟方案， 例如：音频中3A 算法、网络评估、自适应码流、Simulcast、SVC等等 ， 非常适合刚刚进入音视频行业小伙伴哈^_^  我也是哦， 以后再音视频行业长期打算的小伙伴的学习项目。 里面有大量知识点  </font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

<font color=#999AAA >提示：以下是本篇文章正文内容，下面案例可供参考

# 一、WebRTC中peerConnection的AddTrack调用的流程图

![在这里插入图片描述](https://img-blog.csdnimg.cn/e20615ad241248d4933f42318c1c7ddf.jpeg#pic_center)

# 二、桥接模式介绍

桥接（Bridge）是用于把抽象化与实现化解耦，使得二者可以独立变化。这种类型的设计模式属于结构型模式，它通过提供抽象化和实现化之间的桥接结构，来实现二者的解耦。

这种模式涉及到一个作为桥接的接口，使得实体类的功能独立于接口实现类。这两种类型的类可被结构化改变而互不影响。

我们通过下面的实例来演示桥接模式（Bridge Pattern）的用法。其中，可以使用相同的抽象类方法但是不同的桥接实现类，来画出不同颜色的圆。

介绍

意图：将抽象部分与实现部分分离，使它们都可以独立的变化。

主要解决：在有多种可能会变化的情况下，用继承会造成类爆炸问题，扩展起来不灵活。

何时使用：实现系统可能有多个角度分类，每一种角度都可能变化。

如何解决：把这种多角度分类分离出来，让它们独立变化，减少它们之间耦合。

关键代码：抽象类依赖实现类。

应用实例： 1、猪八戒从天蓬元帅转世投胎到猪，转世投胎的机制将尘世划分为两个等级，即：灵魂和肉体，前者相当于抽象化，后者相当于实现化。生灵通过功能的委派，调用肉体对象的功能，使得生灵可以动态地选择。 2、墙上的开关，可以看到的开关是抽象的，不用管里面具体怎么实现的。

优点： 1、抽象和实现的分离。 2、优秀的扩展能力。 3、实现细节对客户透明。

缺点：桥接模式的引入会增加系统的理解与设计难度，由于聚合关联关系建立在抽象层，要求开发者针对抽象进行设计与编程。

使用场景： 1、如果一个系统需要在构件的抽象化角色和具体化角色之间增加更多的灵活性，避免在两个层次之间建立静态的继承联系，通过桥接模式可以使它们在抽象层建立一个关联关系。 2、对于那些不希望使用继承或因为多层次继承导致系统类的个数急剧增加的系统，桥接模式尤为适用。 3、一个类存在两个独立变化的维度，且这两个维度都需要进行扩展。

注意事项：对于两个独立变化的维度，使用桥接模式再适合不过了。

实现

我们有一个作为桥接实现的 DrawAPI 接口和实现了 DrawAPI 接口的实体类 RedCircle、GreenCircle。Shape 是一个抽象类，将使用 DrawAPI 的对象。BridgePatternDemo 类使用 Shape 类来画出不同颜色的圆。

![在这里插入图片描述](https://img-blog.csdnimg.cn/d36cb7227a864ef481f57274de9d2000.jpeg#pic_center)

```cpp
class DrawAPI {
   public:
  virtual  void drawCircle(int radius, int x, int y) = 0;
}

  class RedCircle public: DrawAPI
  {
   
   public:
    void drawCircle(int radius, int x, int y) 
    {
      printf("%s\n", std::string("Drawing Circle[ color: red, radius: " + std::to_string(radius )+", x: " +std::to_string(x)+", "+ std::to_string(y) +"]").c_str());
   }
}
class GreenCircle public: DrawAPI
  {
   
   public:
    void drawCircle(int radius, int x, int y) 
    {
      printf("%s\n", std::string("Drawing Circle[ color: green, radius: " + std::to_string(radius )+", x: " +std::to_string(x)+", "+ std::to_string(y) +"]").c_str());
   }
}



///////////////////
  class Shape {
   protected:
   DrawAPI drawAPI;
   protected:
   Shape(DrawAPI drawAPI)
   {
     drawAPI = drawAPI;
   }
   public: 
   virtual void draw() = 0;  
}


  class Circle :public Shape {
   private:
    int x, y, radius;
 
   public Circle(int x, int y, int radius, DrawAPI drawAPI) 
   : Shape (drawAPI)
   {
      
      this.x = x;  
      this.y = y;  
      this.radius = radius;
   }
 
   public:
    void draw() 
    {
      drawAPI.drawCircle(radius,x,y);
   }
}

//////////////////////////
 
  int main(int argc, char *argv[])
  {
      Shape redCircle  (100,100, 10, new RedCircle());
      Shape greenCircle  (100,100, 10, new GreenCircle());
 
      redCircle.draw();
      greenCircle.draw();
   }
 

```


# 三、WebRTC中桥接模式使用
## 1、介绍 VideoTrackSource 、VideoTrack和Notifier桥接模式
在模板Notifier中RegisterObserver注册对象

```cpp

// VideoTrackSource is a convenience base class for implementations of
// VideoTrackSourceInterface.
class RTC_EXPORT VideoTrackSource : public Notifier<VideoTrackSourceInterface> {
 public:
  explicit VideoTrackSource(bool remote);
  void SetState(SourceState new_state);

  SourceState state() const override { return state_; }
  bool remote() const override { return remote_; }

  bool is_screencast() const override { return false; }
  absl::optional<bool> needs_denoising() const override {
    return absl::nullopt;
  }

  bool GetStats(Stats* stats) override { return false; }

  void AddOrUpdateSink(rtc::VideoSinkInterface<VideoFrame>* sink,
                       const rtc::VideoSinkWants& wants) override;
  void RemoveSink(rtc::VideoSinkInterface<VideoFrame>* sink) override;

 protected:
  virtual rtc::VideoSourceInterface<VideoFrame>* source() = 0;

 private:
  rtc::ThreadChecker worker_thread_checker_;
  SourceState state_;
  const bool remote_;
};

// Implements a template version of a notifier.
// TODO(deadbeef): This is an implementation detail; move out of api/.
// TODO@chensong 2022-10-05 桥接模式
template <class T>
class Notifier : public T 
{
 public:
  Notifier() {}

  virtual void RegisterObserver(ObserverInterface* observer) 
  {
    RTC_DCHECK(observer != nullptr);
    observers_.push_back(observer);
  }

  virtual void UnregisterObserver(ObserverInterface* observer)
  {
    for (std::list<ObserverInterface*>::iterator it = observers_.begin(); it != observers_.end(); it++) 
	{
      if (*it == observer)
	  {
        observers_.erase(it);
        break;
      }
    }
  }

  void FireOnChanged() 
  {
    // Copy the list of observers to avoid a crash if the observer object
    // unregisters as a result of the OnChanged() call. If the same list is used
    // UnregisterObserver will affect the list make the iterator invalid.
    std::list<ObserverInterface*> observers = observers_;
    for (std::list<ObserverInterface*>::iterator it = observers.begin(); it != observers.end(); ++it) 
	{
      (*it)->OnChanged();
    }
  }

 protected:
  std::list<ObserverInterface*> observers_;
};
 
 class VideoTrack : public MediaStreamTrack<VideoTrackInterface>, public rtc::VideoSourceBase, public ObserverInterface 
{
 public:
  static rtc::scoped_refptr<VideoTrack> Create(
      const std::string& label,
      VideoTrackSourceInterface* source,
      rtc::Thread* worker_thread);

  void AddOrUpdateSink(rtc::VideoSinkInterface<VideoFrame>* sink,
                       const rtc::VideoSinkWants& wants) override;
  void RemoveSink(rtc::VideoSinkInterface<VideoFrame>* sink) override;

  VideoTrackSourceInterface* GetSource() const override {
    return video_source_.get();
  }
  ContentHint content_hint() const override;
  void set_content_hint(ContentHint hint) override;
  bool set_enabled(bool enable) override;
  std::string kind() const override;

 protected:
  VideoTrack(const std::string& id,
             VideoTrackSourceInterface* video_source,
             rtc::Thread* worker_thread);
  ~VideoTrack();

 private:
  // Implements ObserverInterface. Observes |video_source_| state.
  void OnChanged() override;

  rtc::Thread* const worker_thread_;
  rtc::ThreadChecker signaling_thread_checker_;
  rtc::scoped_refptr<VideoTrackSourceInterface> video_source_;
  ContentHint content_hint_ RTC_GUARDED_BY(signaling_thread_checker_);
};
// VideoTrackInterface.
template <typename T>
class MediaStreamTrack : public Notifier<T> {
 public:
  typedef typename T::TrackState TypedTrackState;

  std::string id() const override { return id_; }
  MediaStreamTrackInterface::TrackState state() const override 
  {
    return state_;
  }
  bool enabled() const override { return enabled_; }
  bool set_enabled(bool enable) override 
  {
    bool fire_on_change = (enable != enabled_);
    enabled_ = enable;
    if (fire_on_change) {
      Notifier<T>::FireOnChanged();
    }
    return fire_on_change;
  }

 protected:
  explicit MediaStreamTrack(const std::string& id)
      : enabled_(true), id_(id), state_(MediaStreamTrackInterface::kLive) {}

  bool set_state(MediaStreamTrackInterface::TrackState new_state) {
    bool fire_on_change = (state_ != new_state);
    state_ = new_state;
    if (fire_on_change)
      Notifier<T>::FireOnChanged();
    return true;
  }

 private:
  bool enabled_;
  std::string id_;
  MediaStreamTrackInterface::TrackState state_;
};

```

<font color='red'>对应的关系是VideoTrack对象是要注册到VideoTrackSource中的</font>

<font color='red'>VideoTrackSource管理所有VideoTrack对象</font>

### 1.1  在VideoTrack中还有桥接模式MediaStreamTrack

是MediaStreamTrack这个类桥接模式是对应RtpSender对象进行注册的




## 2. VideoTrack、MediaStreamTrack和VideoRtpSender一个组合

```cpp
// VideoTrackInterface.
template <typename T>
class MediaStreamTrack : public Notifier<T> {
 public:
  typedef typename T::TrackState TypedTrackState;

  std::string id() const override { return id_; }
  MediaStreamTrackInterface::TrackState state() const override 
  {
    return state_;
  }
  bool enabled() const override { return enabled_; }
  bool set_enabled(bool enable) override 
  {
    bool fire_on_change = (enable != enabled_);
    enabled_ = enable;
    if (fire_on_change) {
      Notifier<T>::FireOnChanged();
    }
    return fire_on_change;
  }

 protected:
  explicit MediaStreamTrack(const std::string& id)
      : enabled_(true), id_(id), state_(MediaStreamTrackInterface::kLive) {}

  bool set_state(MediaStreamTrackInterface::TrackState new_state) {
    bool fire_on_change = (state_ != new_state);
    state_ = new_state;
    if (fire_on_change)
      Notifier<T>::FireOnChanged();
    return true;
  }

 private:
  bool enabled_;
  std::string id_;
  MediaStreamTrackInterface::TrackState state_;
};
```


**在RtpSenderBase类中SetTrack方法**

```cpp

bool RtpSenderBase::SetTrack(MediaStreamTrackInterface* track) 
{
  TRACE_EVENT0("webrtc", "RtpSenderBase::SetTrack");
  if (stopped_)
  {
    RTC_LOG(LS_ERROR) << "SetTrack can't be called on a stopped RtpSender.";
    return false;
  }
  if (track && track->kind() != track_kind())
  {
    RTC_LOG(LS_ERROR) << "SetTrack with " << track->kind()
                      << " called on RtpSender with " << track_kind()
                      << " track.";
    return false;
  }

  // Detach from old track.
  if (track_)
  {
    DetachTrack();
    track_->UnregisterObserver(this);
    RemoveTrackFromStats();
  }

  // Attach to new track.
  // TODO@chensong 2022-10-04 是否有的老的信息
  bool prev_can_send_track = can_send_track();
  // Keep a reference to the old track to keep it alive until we call SetSend.
  rtc::scoped_refptr<MediaStreamTrackInterface> old_track = track_;
  track_ = track;
  if (track_) 
  {
	// TODO@chensong 2022-10-04 举例子: 以VideoTrack为例子
	  //  把当前的rtp_sender对象注册到VideoTrack中MediaStreamTrack对象(Notifier<>)中去
    track_->RegisterObserver(this);

	// TODO@chensong 2022-10-04 调用VideoRtp_Sender对象中方法
    AttachTrack();
  }

  // Update channel.
  if (can_send_track()) 
  {
    SetSend();
    AddTrackToStats();
  } 
  else if (prev_can_send_track) 
  {
    ClearSend();
  }
  attachment_id_ = (track_ ? GenerateUniqueId() : 0);
  return true;
}
```


相当于VideoTrack管理所有VideoRTPSender对象

# 四、RtpSenderBase关系图

![在这里插入图片描述](https://img-blog.csdnimg.cn/5f3ac46004494444859202646e921ba3.jpeg#pic_center)



VideoTrack在RtpSenderBase成员变量MediaStreamTrackInterface

在RtpSenderBase中有MediaChannel（编码器）、dtlsTransport（网络传输）

```cpp


```




# 五、AddTrack添加视频源分析

CreateSender 创建发送RtpSender对象

CreateReceiver 创建接受RtpReceiver对象


把RtpSender和RtpReceiver对象合并RtpTransceiver对象中管理

RtpTransceiver对象放到transceivers_列表中去


 ![在这里插入图片描述](https://img-blog.csdnimg.cn/1d7dfa7ca05d48979552d462dc65885b.jpeg#pic_center)


```cpp

RTCErrorOr<rtc::scoped_refptr<RtpSenderInterface>> PeerConnection::AddTrack(rtc::scoped_refptr<MediaStreamTrackInterface> track,
    const std::vector<std::string>& stream_ids)
{
  RTC_DCHECK_RUN_ON(signaling_thread());
  TRACE_EVENT0("webrtc", "PeerConnection::AddTrack");
  if (!track) 
  {
    LOG_AND_RETURN_ERROR(RTCErrorType::INVALID_PARAMETER, "Track is null.");
  }
  if (!(track->kind() == MediaStreamTrackInterface::kAudioKind ||
        track->kind() == MediaStreamTrackInterface::kVideoKind)) 
  {
    LOG_AND_RETURN_ERROR(RTCErrorType::INVALID_PARAMETER,
                         "Track has invalid kind: " + track->kind());
  }
  if (IsClosed()) 
  {
    LOG_AND_RETURN_ERROR(RTCErrorType::INVALID_STATE,
                         "PeerConnection is closed.");
  }
  if (FindSenderForTrack(track)) 
  {
    LOG_AND_RETURN_ERROR(
        RTCErrorType::INVALID_PARAMETER,
        "Sender already exists for track " + track->id() + ".");
  }
  auto sender_or_error = (IsUnifiedPlan() ? AddTrackUnifiedPlan(track, stream_ids) : AddTrackPlanB(track, stream_ids));
  if (sender_or_error.ok())
  {
    // TODO@chensong 20220905 回调 用户名态类的方法通知
    Observer()->OnRenegotiationNeeded();
    // TODO@chensong 2022-09-29 放到数据统计中去
    stats_->AddTrack(track);
  }
  return sender_or_error;
}



RTCErrorOr<rtc::scoped_refptr<RtpSenderInterface>>
PeerConnection::AddTrackUnifiedPlan(rtc::scoped_refptr<MediaStreamTrackInterface> track, const std::vector<std::string>& stream_ids) 
{
	// TODO@chensong 2022-10-04  transceivers_中查找是否有track对象是停止或者不使用的
  rtc::scoped_refptr<RtpTransceiverProxyWithInternal<RtpTransceiver>> transceiver = FindFirstTransceiverForAddedTrack(track);
  if (transceiver) 
  {
    RTC_LOG(LS_INFO) << "Reusing an existing "
                     << cricket::MediaTypeToString(transceiver->media_type())
                     << " transceiver for AddTrack.";
    if (transceiver->direction() == RtpTransceiverDirection::kRecvOnly) 
	{
      transceiver->internal()->set_direction(RtpTransceiverDirection::kSendRecv);
    }
	else if (transceiver->direction() == RtpTransceiverDirection::kInactive) 
	{
      transceiver->internal()->set_direction(RtpTransceiverDirection::kSendOnly);
    }
    //这边注册了
    transceiver->sender()->SetTrack(track);
    transceiver->internal()->sender_internal()->set_stream_ids(stream_ids /*svc vec id */);
  } 
  else 
  {
    cricket::MediaType media_type =
        (track->kind() == MediaStreamTrackInterface::kAudioKind
             ? cricket::MEDIA_TYPE_AUDIO
             : cricket::MEDIA_TYPE_VIDEO);
    RTC_LOG(LS_INFO) << "Adding " << cricket::MediaTypeToString(media_type)
                     << " transceiver in response to a call to AddTrack.";
    std::string sender_id = track->id();
    // Avoid creating a sender with an existing ID by generating a random ID.
    // This can happen if this is the second time AddTrack has created a sender
    // for this track.
    // TODO@chensong 20220905
    // 查看是否存在有冲突sender_id 如果冲突就重新生成一个哈
    if (FindSenderById(sender_id)) 
	{
      sender_id = rtc::CreateRandomUuid();
    }
    // 看到吧  这边  发送和接受数据的通道都设置好了
    rtc::scoped_refptr<RtpSenderProxyWithInternal<RtpSenderInternal>> sender = CreateSender(media_type, sender_id, track, stream_ids, {});
    rtc::scoped_refptr<RtpReceiverProxyWithInternal<RtpReceiverInternal>> receiver = CreateReceiver(media_type, rtc::CreateRandomUuid());
    transceiver = CreateAndAddTransceiver(sender, receiver);
    transceiver->internal()->set_created_by_addtrack(true);
    transceiver->internal()->set_direction(RtpTransceiverDirection::kSendRecv);
  }
  return transceiver->sender();
}


rtc::scoped_refptr<RtpSenderProxyWithInternal<RtpSenderInternal>>
PeerConnection::CreateSender(
    cricket::MediaType media_type,
    const std::string& id,
    rtc::scoped_refptr<MediaStreamTrackInterface> track,
    const std::vector<std::string>& stream_ids,
    const std::vector<RtpEncodingParameters>& send_encodings)
{
  RTC_DCHECK_RUN_ON(signaling_thread());
  rtc::scoped_refptr<RtpSenderProxyWithInternal<RtpSenderInternal>> sender;
  if (media_type == cricket::MEDIA_TYPE_AUDIO) 
  {
    RTC_DCHECK(!track ||
               (track->kind() == MediaStreamTrackInterface::kAudioKind));
    sender = RtpSenderProxyWithInternal<RtpSenderInternal>::Create(
        signaling_thread(),
        AudioRtpSender::Create(worker_thread(), id, stats_.get()));
    NoteUsageEvent(UsageEvent::AUDIO_ADDED);
  } 
  else 
  {
    RTC_DCHECK_EQ(media_type, cricket::MEDIA_TYPE_VIDEO);
    RTC_DCHECK(!track || (track->kind() == MediaStreamTrackInterface::kVideoKind));
    sender = RtpSenderProxyWithInternal<RtpSenderInternal>::Create( signaling_thread(), VideoRtpSender::Create(worker_thread(), id));
    NoteUsageEvent(UsageEvent::VIDEO_ADDED);
  }
  bool set_track_succeeded = sender->SetTrack(track);
  RTC_DCHECK(set_track_succeeded);
  sender->internal()->set_stream_ids(stream_ids);
  sender->internal()->set_init_send_encodings(send_encodings);
  return sender;
}
```

# 总结


VideoSourceTrack管理所有VideoTrack对象

VideoTrack对象管理所有的VideoRtpSender对象


[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)


