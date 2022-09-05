---
layout:     post
title:      WebRTC网络PhysicalSocketServer之WSAEventselect模型使用
subtitle:   Win网络WSAEventSelect、WSAWaitForMultipleEvents、 WSAEnumNetworkEvents
date:       2022-01-02
times:       13::44::01
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - 网络编程
  
---


# WebRTC网络PhysicalSocketServer之WSAEventselect模型使用


@[TOC](WebRTC网络PhysicalSocketServer之WSAEventselect模型使用)

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



# 一、WebRTC网络模型




## 1、WebRTC网络PhysicalSocketServer

WebRTC网络使用PhysicalSocketServer类封装对不同平台的网络io事件封装


### ①， win平台网络io
win平台使用主要使用 WSAEventSelect,WSAWaitForMultipleEvents, WSAEnumNetworkEvents这三个函数实现网络io事件监听


代码分析

在WebRTC中使用到和Qt一样的机制叫信号槽 sigslot 非常好玩吧  这里我们先不分析信号槽


在WebRTC中提供 异步连接， 异步读取事件分别都是以信号槽实现的

```cpp
// Provides the ability to perform socket I/O asynchronously.
class AsyncSocket : public Socket {
 public:
  AsyncSocket();
  ~AsyncSocket() override;

  AsyncSocket* Accept(SocketAddress* paddr) override = 0;

  // SignalReadEvent and SignalWriteEvent use multi_threaded_local to allow
  // access concurrently from different thread.
  // For example SignalReadEvent::connect will be called in AsyncUDPSocket ctor
  // but at the same time the SocketDispatcher maybe signaling the read event.
  // ready to read
  sigslot::signal1<AsyncSocket*, sigslot::multi_threaded_local> SignalReadEvent;
  // ready to write
  sigslot::signal1<AsyncSocket*, sigslot::multi_threaded_local>
      SignalWriteEvent;
  sigslot::signal1<AsyncSocket*> SignalConnectEvent;     // connected
  sigslot::signal2<AsyncSocket*, int> SignalCloseEvent;  // closed
};
```

1. 需要创建一个异步socket需要调用CreateAsyncSocket或者CreateSocket

我们就分析 CreateAsyncSocket 接口


```javascript

AsyncSocket* PhysicalSocketServer::CreateAsyncSocket(int family, int type) 
{
//1. 注册事件分配器
  SocketDispatcher* dispatcher = new SocketDispatcher(this);
  // 2. 创建socket 注册读写事件到PhysicalSocketServer 中
  if (dispatcher->Create(family, type)) {
    return dispatcher;
  } else {
    delete dispatcher;
    return nullptr;
  }
}

PhysicalSocket::PhysicalSocket(PhysicalSocketServer* ss, SOCKET s)
    : ss_(ss),
      s_(s),
      error_(0),
      state_((s == INVALID_SOCKET) ? CS_CLOSED : CS_CONNECTED),
      resolver_(nullptr) {
  if (s_ != INVALID_SOCKET) {
    SetEnabledEvents(DE_READ | DE_WRITE);
	// 注册事件哈
    int type = SOCK_STREAM;
    socklen_t len = sizeof(type);
    const int res =
        getsockopt(s_, SOL_SOCKET, SO_TYPE, (SockOptArg)&type, &len);
    RTC_DCHECK_EQ(0, res);
    udp_ = (SOCK_DGRAM == type);
  }
}

bool PhysicalSocket::Create(int family, int type) {
  Close();
  s_ = ::socket(family, type, 0);
  udp_ = (SOCK_DGRAM == type);
  UpdateLastError();
  if (udp_) {
    SetEnabledEvents(DE_READ | DE_WRITE);
  }
  return s_ != INVALID_SOCKET;
}


bool SocketDispatcher::Initialize() {
  RTC_DCHECK(s_ != INVALID_SOCKET);
// Must be a non-blocking
#if defined(WEBRTC_WIN)
  u_long argp = 1;
  ioctlsocket(s_, FIONBIO, &argp);
#elif defined(WEBRTC_POSIX)
  fcntl(s_, F_SETFL, fcntl(s_, F_GETFL, 0) | O_NONBLOCK);
#endif
#if defined(WEBRTC_IOS)
  // iOS may kill sockets when the app is moved to the background
  // (specifically, if the app doesn't use the "voip" UIBackgroundMode). When
  // we attempt to write to such a socket, SIGPIPE will be raised, which by
  // default will terminate the process, which we don't want. By specifying
  // this socket option, SIGPIPE will be disabled for the socket.
  int value = 1;
  ::setsockopt(s_, SOL_SOCKET, SO_NOSIGPIPE, &value, sizeof(value));
#endif
  // 添加读写事件哈
  ss_->Add(this);
  return true;
}
```


下面我们就分析Wait函数

主要逻辑

1. 把注册事件分发配 dispatchers_中事件注册系统WSAEventSelect函数中去
2. 使用系统WSAWaitForMultipleEvents函数去到上面WSAEventSelect函数放到事件地址队列中检查是否有事件发生进行响应事件
3.  然后到发生事件队列中取出事件 使用WSAEnumNetworkEvents函数取出事件 然后触发事件哈，
4. 在WebRTC使用win这个模型中只有一开始会触发写事件哈， 然后随时都可以触发写事件哈， 但是只有第一次可以触发大数据块写去之后的写入就不能写入太的大哈 ， win上这个机制也是玩死我了， 和一般事件都不一样呢 ， 一般事件都注册io事件然后触发事件


```
#if defined(WEBRTC_WIN)
bool PhysicalSocketServer::Wait(int cmsWait, bool process_io) {
  int64_t cmsTotal = cmsWait;
  int64_t cmsElapsed = 0;
  int64_t msStart = Time();

  fWait_ = true;
  while (fWait_) {
    std::vector<WSAEVENT> events;
    std::vector<Dispatcher*> event_owners;

    events.push_back(socket_ev_);

    {
      CritScope cr(&crit_);
      // TODO(jbauch): Support re-entrant waiting.
      RTC_DCHECK(!processing_dispatchers_);

      // Calling "CheckSignalClose" might remove a closed dispatcher from the
      // set. This must be deferred to prevent invalidating the iterator.
      processing_dispatchers_ = true;
      // 1. 从注册事件拿到socket放到， 系统WSEVeventSelect函数中去哈
      for (Dispatcher* disp : dispatchers_) {
        if (!process_io && (disp != signal_wakeup_))
          continue;
        SOCKET s = disp->GetSocket();
        if (disp->CheckSignalClose()) {
          // We just signalled close, don't poll this socket
        } else if (s != INVALID_SOCKET) {
          WSAEventSelect(s, events[0],
                         FlagsToEvents(disp->GetRequestedEvents()));
        } else {
          events.push_back(disp->GetWSAEvent());
          event_owners.push_back(disp);
        }
      }

      processing_dispatchers_ = false;
      // Process deferred dispatchers that have been added/removed while the
      // events were handled above.
      AddRemovePendingDispatchers();
    }

    // Which is shorter, the delay wait or the asked wait?

    int64_t cmsNext;
    if (cmsWait == kForever) {
      cmsNext = cmsWait;
    } else {
      cmsNext = std::max<int64_t>(0, cmsTotal - cmsElapsed);
    }

    // Wait for one of the events to signal
    // 这边得到事件响应的哈
    DWORD dw =
        WSAWaitForMultipleEvents(static_cast<DWORD>(events.size()), &events[0],
                                 false, static_cast<DWORD>(cmsNext), false);

    if (dw == WSA_WAIT_FAILED) {
      // Failed?
      // TODO(pthatcher): need a better strategy than this!
      WSAGetLastError();
      RTC_NOTREACHED();
      return false;
    } else if (dw == WSA_WAIT_TIMEOUT) {
      // Timeout?
      return true;
    } else {
      // Figure out which one it is and call it
      CritScope cr(&crit_);
      int index = dw - WSA_WAIT_EVENT_0;
      if (index > 0) {
        --index;  // The first event is the socket event
        Dispatcher* disp = event_owners[index];
        // The dispatcher could have been removed while waiting for events.
        if (dispatchers_.find(disp) != dispatchers_.end()) {
          disp->OnPreEvent(0);
          disp->OnEvent(0, 0);
        }
      } else if (process_io) {
        processing_dispatchers_ = true;
        for (Dispatcher* disp : dispatchers_) {
          SOCKET s = disp->GetSocket();
          if (s == INVALID_SOCKET)
            continue;

          WSANETWORKEVENTS wsaEvents;
          int err = WSAEnumNetworkEvents(s, events[0], &wsaEvents);
          if (err == 0) {
            {
              if ((wsaEvents.lNetworkEvents & FD_READ) &&
                  wsaEvents.iErrorCode[FD_READ_BIT] != 0) {
                RTC_LOG(WARNING)
                    << "PhysicalSocketServer got FD_READ_BIT error "
                    << wsaEvents.iErrorCode[FD_READ_BIT];
              }
              if ((wsaEvents.lNetworkEvents & FD_WRITE) &&
                  wsaEvents.iErrorCode[FD_WRITE_BIT] != 0) {
                RTC_LOG(WARNING)
                    << "PhysicalSocketServer got FD_WRITE_BIT error "
                    << wsaEvents.iErrorCode[FD_WRITE_BIT];
              }
              if ((wsaEvents.lNetworkEvents & FD_CONNECT) &&
                  wsaEvents.iErrorCode[FD_CONNECT_BIT] != 0) {
                RTC_LOG(WARNING)
                    << "PhysicalSocketServer got FD_CONNECT_BIT error "
                    << wsaEvents.iErrorCode[FD_CONNECT_BIT];
              }
              if ((wsaEvents.lNetworkEvents & FD_ACCEPT) &&
                  wsaEvents.iErrorCode[FD_ACCEPT_BIT] != 0) {
                RTC_LOG(WARNING)
                    << "PhysicalSocketServer got FD_ACCEPT_BIT error "
                    << wsaEvents.iErrorCode[FD_ACCEPT_BIT];
              }
              if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
                  wsaEvents.iErrorCode[FD_CLOSE_BIT] != 0) {
                RTC_LOG(WARNING)
                    << "PhysicalSocketServer got FD_CLOSE_BIT error "
                    << wsaEvents.iErrorCode[FD_CLOSE_BIT];
              }
            }
            uint32_t ff = 0;
            int errcode = 0;
            if (wsaEvents.lNetworkEvents & FD_READ)
              ff |= DE_READ;
            if (wsaEvents.lNetworkEvents & FD_WRITE)
              ff |= DE_WRITE;
            if (wsaEvents.lNetworkEvents & FD_CONNECT) {
              if (wsaEvents.iErrorCode[FD_CONNECT_BIT] == 0) {
                ff |= DE_CONNECT;
              } else {
                ff |= DE_CLOSE;
                errcode = wsaEvents.iErrorCode[FD_CONNECT_BIT];
              }
            }
            if (wsaEvents.lNetworkEvents & FD_ACCEPT)
              ff |= DE_ACCEPT;
            if (wsaEvents.lNetworkEvents & FD_CLOSE) {
              ff |= DE_CLOSE;
              errcode = wsaEvents.iErrorCode[FD_CLOSE_BIT];
            }
            if (ff != 0) 
            {
            // 这里就是调用信号槽函数哈  -. >
              disp->OnPreEvent(ff);
              disp->OnEvent(ff, errcode);
            }
          }
        }

        processing_dispatchers_ = false;
        // Process deferred dispatchers that have been added/removed while the
        // events were handled above.
        AddRemovePendingDispatchers();
      }

      // Reset the network event until new activity occurs
      WSAResetEvent(socket_ev_);
    }

    // Break?
    if (!fWait_)
      break;
    cmsElapsed = TimeSince(msStart);
    if ((cmsWait != kForever) && (cmsElapsed >= cmsWait)) {
      break;
    }
  }

  // Done
  return true;
}
#endif  // WEBRTC_WIN
```

### ②， linux平台网络io
liunx使用epoll_wait进行网络io的管理

就不分析哈 对epoll_wait网络io事件不熟悉的可以看  [Linux网络编程(epoll的边缘触发和水平触发)多路IO高并发服务器](https://chensongpoixs.github.io/2017/11/01/Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%28epoll%E7%9A%84%E8%BE%B9%E7%BC%98%E8%A7%A6%E5%8F%91%E5%92%8C%E6%B0%B4%E5%B9%B3%E8%A7%A6%E5%8F%91%29%E5%A4%9A%E8%B7%AFIO%E9%AB%98%E5%B9%B6%E5%8F%91%E6%9C%8D%E5%8A%A1%E5%99%A8/#/)


## 二， 代码实现

这个是websocket实现一个简单客户端实例哈


```
#include "examples/desktop_capture/desktop_capture.h"
#include "test/video_renderer.h"
#include "rtc_base/logging.h"

#include <thread>
#include "system_wrappers/include/field_trial.h"
#include "test/field_trial.h"
#include  <Windows.h>
#include <iostream>

#include "modules/audio_device/include/audio_device.h"
#include "api/audio/audio_mixer.h"
#include "api/audio_codecs/audio_decoder_factory.h"
#include "api/audio_codecs/audio_encoder_factory.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "modules/audio_device/include/audio_device.h"
#include "common_audio/resampler/include/resampler.h"
#include "modules/audio_processing/aec/echo_cancellation.h"
#include "modules/audio_processing/include/audio_processing.h"
#include "common_audio/vad/include/webrtc_vad.h"
#include "audio/remix_resample.h"

#include "api/audio/audio_frame.h"
#include "audio/utility/audio_frame_operations.h"
#include "common_audio/resampler/include/push_resampler.h"
#include "rtc_base/checks.h"
#include <inttypes.h>

#include <common_audio/resampler/include/push_resampler.h>
#include <api/audio/audio_frame.h>
#include "p2p/base/stun_server.h"
#include "rtc_base/async_tcp_socket.h"
#include "rtc_base/socket_address.h"
#include "rtc_base/socket_server.h"
#include "rtc_base/thread.h"
#include "rtc_base/checks.h"
#include "rtc_base/logging.h"
#include "rtc_base/net_helpers.h"
#include <iostream>
#include <thread>
#include "rtc_base/physical_socket_server.h"
#include "rtc_base/strings/json.h"

#include <mutex>
 


 
/*
1. 需要包含winsock2.h，链接ws2_32.llib



2. 把#include <winsock2.h>放到最前面

至于原因，那是因为windows.h这个头文件已经包含了winsock.h,winsock.h和winsock2.h貌似有冲突  解释在：http://vc.ygman.com/thread/47071



3.MSDN中说使用WSAEventSelect模型等待时是不占cpu时间的，这也是效率比阻塞winsock高的原因；非阻塞模式可以省去send或者recv没有空间发送或没有数据接收时的等待时间；一个线程管理多个会话套接字；



4. 同一个事件会不会由于多个网络事件的集合，同时发生两个网络事件后被屏蔽？

用send做实验，发现会的。因为在TCP连接开始，会在服务器端的会话套接字上触发FD_WRITE事件。如果在会话套接字上监听FD_READ和FD_WRITE事件，会有如下两种情况。

第一、实验在客户端connect后中断，那么在服务器端就会在接听套接字接收到FD_ACCEPT事件，随后在创建的会话套接字上触发FD_WRITE事件，已经通过单步调试验证。

第二、如果在客户端send后中断，那么在服务器端首先还是在监听套接字接收到FD_ACCEPT事件，随后在创建的会话套接字上触发FD_WRITE事件，这个时候不同的是，由于客户端发送数据的缘故，会再这个会话套接字上再触发FD_READ事件。这时就会出现一个套接字上监听的多个网络事件同时触发的问题。因为此时会话套接字上触发了两个网络时间，但是在处理的时候WSAEnumNetworkEvents会自动重置这个事件，比如在处理的时候先处理的是FD_READ事件，过去之后这个event就会被重置，所以FD_WRITE时间就无法处理了；如果先处理的是FD_WRITE事件，过去之后这个event还是会被重置，所以这时FD_READ时间就得不到处理了。





5. FD_WIRTE事件的作用？

一直搞不懂 WSAEventSelect 的 FD_WRITE ，不知道怎么利用他在自己想发数据的时候发数据，后来知道了想发随时发消息 要自己另外去写send方法，FD_WRITE 是用于一开始连接成功侯就开始发送大批量数据的，比如发一个视频连接给别人 ，别人接了 那么这个时候就触发了FD_WRITE ，视频的数据会不停的充满缓存，所以FD_WRITE会不停的触发因为没人教我 只能靠自己苦苦参悟了 希望别的朋友也能看到我的文字，不要 去被 FD_WRITE 烦恼了  想自己随时发数据的时候 ，自己另外去写send方法 如果你不是一次性发送大批量数据的话，就别想着使用FD_WRITE事件了，因为如果既希望于在收到FD_WRITE的时候发送数据，但是又不能发送足够多的数据填满socket缓冲区的话，就只能收到刚刚建连接的时候的第一次事件，之后及不会再收到了，所以当只是发送尽可能少的数据的时候，忘掉FD_WRITE机制，在任何想发送数据的时候直接调用send发送吧。
 

*/








#define CHEN_LOG_SHOW std::cout << "[info]" << __FUNCTION__ << "[" << __LINE__ << "] thread_id = " << std::this_thread::get_id() << std::endl;



#define CHEN_LOG_SHOW_END std::cout << "[info]" << __FUNCTION__ << "[" << __LINE__ << "] end thread_id = " << std::this_thread::get_id() << std::endl;

enum ESESSION_TYPE
{
	ESession_None = 0,
	ESession_Init,
	ESession_Connnecting,
	ESession_Handshake,
	ESession_Connnectd,
	ESession_Close,
	
};
 

enum ENet_Type
{
	ENet_Connect = 0,
	ENet_Connected,
	ENet_Close
};

struct CNet_Message
{
	ENet_Type	m_type;
	uint32_t	m_session;
	std::string m_message;
	CNet_Message()
		: m_type(ENet_Connect)
		, m_session(0)
		, m_message() {}
};



 

class cTestClient : public sigslot::has_slots<>  
{
private:
	typedef rtc::SocketDispatcher			csocket;
	typedef rtc::SocketAddress			caddress;
	typedef rtc::PhysicalSocketServer	cwork;
	typedef std::thread					cthread;
	typedef std::lock_guard<std::mutex> clock_guard;

	// http://tools.ietf.org/html/rfc6455#section-5.2  Base Framing Protocol
	//
	//  0                   1                   2                   3
	//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	// +-+-+-+-+-------+-+-------------+-------------------------------+
	// |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
	// |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
	// |N|V|V|V|       |S|             |   (if payload len==126/127)   |
	// | |1|2|3|       |K|             |                               |
	// +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
	// |     Extended payload length continued, if payload len == 127  |
	// + - - - - - - - - - - - - - - - +-------------------------------+
	// |                               |Masking-key, if MASK set to 1  |
	// +-------------------------------+-------------------------------+
	// | Masking-key (continued)       |          Payload Data         |
	// +-------------------------------- - - - - - - - - - - - - - - - +
	// :                     Payload Data continued ...                :
	// + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
	// |                     Payload Data continued ...                |
	// +---------------------------------------------------------------+
	struct wsheader_type {
		unsigned header_size;
		bool fin;
		bool mask;
		enum opcode_type {
			CONTINUATION = 0x0,
			TEXT_FRAME = 0x1,
			BINARY_FRAME = 0x2,
			CLOSE = 8,
			PING = 9,
			PONG = 0xa,
		} opcode;
		int N0;
		uint64_t N;
		uint8_t masking_key[4];
	};
public:
	cTestClient( ) 
		: m_address()
		, m_socket()
		, m_status(ESession_None)  //后期
		, m_io_work()
		, control_data_()
		, onconnect_data_()
		, m_websocket_protoo_id(10000)

	{	 
		 
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(1, 0);
		WSAStartup(wVersionRequested, &wsaData);
	}
	~cTestClient() {}
public:
 
	bool init(const char * host, uint16_t port)
	{
		CHEN_LOG_SHOW
		m_address.SetIP(host);
		m_address.SetPort(port);



	 	 m_socket =   static_cast<csocket *>(m_io_work.CreateAsyncSocket(m_address.ipaddr().family(), SOCK_STREAM));
		if (!m_socket)
		{
			std::cout << "create async socket failed !!!" << std::endl;
			return false;
		}
		//设置发送缓冲区大小
		m_socket->SetOption(rtc::Socket::OPT_SNDBUF, 1000000);
		m_socket->SignalCloseEvent.connect(this, &cTestClient:: OnClose); 
		m_socket->SignalConnectEvent.connect(this, &cTestClient::OnConnect);
		m_socket->SignalWriteEvent.connect(this, &cTestClient::OnWrite);
		m_socket->SignalReadEvent.connect(this, &cTestClient::OnRead);
		m_status = ESession_Init;
		return true;
	}


	bool connect_to()
	{
		CHEN_LOG_SHOW
		if (!m_socket)
		{
			return false;
		}
		onconnect_data_.clear();
		char line[1024] = {0};

		snprintf(line, 1024, "GET /%s HTTP/1.1\r\n", "/?roomId=chensong&peerId=xiqhlyrn"); 
		onconnect_data_ = line;
		snprintf(line, 1024, "Host: %s:%d\r\n", m_address.hostname().c_str(), m_address.ip()); 
		onconnect_data_ += line;


		static const char * user_agent = "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36\r\n";

		onconnect_data_ += user_agent;

		snprintf(line, 1024, "Upgrade: websocket\r\n"); 
		onconnect_data_ += line;

		snprintf(line, 1024, "Connection: Upgrade\r\n");
		onconnect_data_ += line;

		snprintf(line, 1024, "Origin: http://%s:%u\r\n", m_address.hostname().c_str(), m_address.port()); 
		onconnect_data_ += line;
		snprintf(line, 1024, "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n");
		onconnect_data_ += line;
		snprintf(line, 1024, "Sec-WebSocket-Version: 13\r\n"); 
		onconnect_data_ += line;

		static const char * websocketproto = "Sec-WebSocket-Protocol: protoo\r\n";
		onconnect_data_ += websocketproto;
		onconnect_data_ += "\r\n";
		m_socket->Connect(m_address);
		m_status = ESession_Connnecting;
		// async connect
		return true;
	}


	bool startup()
	{
		m_thread = std::thread(&cTestClient::_work_thread, this);
		return true;
	}
	void OnClose(rtc::AsyncSocket* socket, int error)
	{
		CHEN_LOG_SHOW
	}
	void OnConnect(rtc::AsyncSocket* socket)
	{
		CHEN_LOG_SHOW
			rtc::PacketOptions options;
		socket->Send(onconnect_data_.c_str(), onconnect_data_.length());  
		onconnect_data_.clear();
		m_status = ESession_Handshake;
	}
	void OnWrite(rtc::AsyncSocket* socket )
	{
		CHEN_LOG_SHOW


		Json::StyledWriter writer;
		Json::Value jmessage;

		jmessage["request"] = true;
		jmessage["id"] = ++m_websocket_protoo_id;
		jmessage["method"] = "getRouterRtpCapabilities";
		jmessage["data"] = Json::objectValue;
		//SendMessage(writer.write(jmessage));
		//
		std::string message = writer.write(jmessage);
		RTC_LOG(INFO) << "[INFO]" << "send message = " << message << "   !!!";

		_send_frame(wsheader_type::TEXT_FRAME, (const uint8_t*)message.c_str(), message.length());
		//m_io_work.Remove(m_socket);
		//m_socket->DisableEvents(rtc::DE_WRITE);
	}
	void OnRead(rtc::AsyncSocket* socket )
	{
		CHEN_LOG_SHOW
			//m_socket->OnEvent(rtc::DE_READ, 0);
		if (_read_into_buffer(socket, control_data_))
		{
			RTC_LOG(INFO) << "INFO" << ", recv = " << control_data_;
			if (m_status == ESession_Handshake)
			{
				int status = 0;
				// 判断服务器校验是否成功
				if (sscanf(control_data_.c_str(), "HTTP/1.1 %d", &status) != 1 || status != 101)
				{
					m_status = ESession_Close;
					RTC_LOG(INFO) << "websocket protoo handshake  failed !!! message = " << control_data_;
						_push_msg(ENet_Close, "");
				}
				else
				{
					m_status = ESession_Connnectd; // 连接成功哈
					RTC_LOG(INFO) << "websocket protoo handshake sucesss !!!";
					_push_msg(ENet_Connect, "");
				}

			}
			else if (m_status == ESession_Connnectd)
			{
				for (const char &value : control_data_)
				{
					m_recv_data.push_back(value);
				}
				control_data_.clear();
				while (true)
				{
					wsheader_type ws;
					if (m_recv_data.size() < 2) { return; /* Need at least 2 */ }
					const uint8_t * data = (uint8_t *) &m_recv_data[0]; // peek, but don't consume
					ws.fin = (data[0] & 0x80) == 0x80;
					ws.opcode = (wsheader_type::opcode_type) (data[0] & 0x0f);
					ws.mask = (data[1] & 0x80) == 0x80;
					ws.N0 = (data[1] & 0x7f);
					ws.header_size = 2 + (ws.N0 == 126? 2 : 0) + (ws.N0 == 127? 8 : 0) + (ws.mask? 4 : 0);
					if (m_recv_data.size() < ws.header_size) 
					{ 
						//return; /* Need: ws.header_size - rxbuf.size() */ 
						break;
					}
					int i = 0;
					if (ws.N0 < 126) {
						ws.N = ws.N0;
						i = 2;
					}
					else if (ws.N0 == 126) {
						ws.N = 0;
						ws.N |= ((uint64_t) data[2]) << 8;
						ws.N |= ((uint64_t) data[3]) << 0;
						i = 4;
					}
					else if (ws.N0 == 127) {
						ws.N = 0;
						ws.N |= ((uint64_t) data[2]) << 56;
						ws.N |= ((uint64_t) data[3]) << 48;
						ws.N |= ((uint64_t) data[4]) << 40;
						ws.N |= ((uint64_t) data[5]) << 32;
						ws.N |= ((uint64_t) data[6]) << 24;
						ws.N |= ((uint64_t) data[7]) << 16;
						ws.N |= ((uint64_t) data[8]) << 8;
						ws.N |= ((uint64_t) data[9]) << 0;
						i = 10;
					}
					if (ws.mask) {
						ws.masking_key[0] = ((uint8_t) data[i+0]) << 0;
						ws.masking_key[1] = ((uint8_t) data[i+1]) << 0;
						ws.masking_key[2] = ((uint8_t) data[i+2]) << 0;
						ws.masking_key[3] = ((uint8_t) data[i+3]) << 0;
					}
					else 
					{
						ws.masking_key[0] = 0;
						ws.masking_key[1] = 0;
						ws.masking_key[2] = 0;
						ws.masking_key[3] = 0;
					}
					if (m_recv_data.size() < ws.header_size+ws.N) 
					{
						//return; /* Need: ws.header_size+ws.N - rxbuf.size() */ 
						break;
					}

					// We got a whole message, now do something with it:
					if (
						ws.opcode == wsheader_type::TEXT_FRAME 
						|| ws.opcode == wsheader_type::BINARY_FRAME
						|| ws.opcode == wsheader_type::CONTINUATION
						) {
						if (ws.mask) 
						{
							for (size_t i = 0; i != ws.N; ++i)
							{
								m_recv_data[i+ws.header_size] ^= ws.masking_key[i&0x3]; 
							}
						}
						receivedData.insert(receivedData.end(), m_recv_data.begin()+ws.header_size, m_recv_data.begin()+ws.header_size+(size_t)ws.N);// just feed
						if (ws.fin) 
						{

							if (ws.opcode == wsheader_type::TEXT_FRAME)
							{
								std::string stringMessage(receivedData.begin(), receivedData.end());
								//callable.OnMessage(stringMessage);
								_push_msg(ENet_Connected, stringMessage);
								RTC_LOG(INFO) << "[INFO] recv stringMessage = " <<stringMessage;
							}
							else
							{
								//_push_msg(ENet_Connected, receivedData);
								RTC_LOG(INFO) << "[INFO] recv receivedData = " <<receivedData.data();
								// callable.OnMessage(receivedData);
							}
							receivedData.erase(receivedData.begin(), receivedData.end());
							std::vector<uint8_t> ().swap(receivedData);// free memory
						}
					}
					else if (ws.opcode == wsheader_type::PING)
					{
						if (ws.mask) 
						{
							for (size_t i = 0; i != ws.N; ++i) 
							{
								m_recv_data[i+ws.header_size] ^= ws.masking_key[i&0x3]; 
							}
						}
						std::string data(m_recv_data.begin()+ws.header_size, m_recv_data.begin()+ws.header_size+(size_t)ws.N);
						_send_frame(wsheader_type::PONG,(uint8_t*)data.c_str(),data.length());
					}
					else if (ws.opcode == wsheader_type::PONG) 
					{
					}
					else if (ws.opcode == wsheader_type::CLOSE)
					{
						//  close();
						_push_msg(ENet_Close, "");
					}
					else 
					{
						fprintf(stderr, "ERROR: Got unexpected WebSocket message.\n");
						// close(); 
						_push_msg(ENet_Close, "");
					}

					m_recv_data.erase(m_recv_data.begin(), m_recv_data.begin() + ws.header_size+(size_t)ws.N);
				}
			}
		}
		control_data_.clear();
	}

	void Process()
	{
		std::list<CNet_Message> temp_msgs;

		{
			clock_guard lock(m_msgs_lock);
			temp_msgs.swap(m_msgs);
		}

		// callback // 上层业务的处理哈

	}
private:
	void	_work_thread()
	{
		uint64_t count = 0;
		while (true)
		{
			 
			++count;
			if (count % 5 == 0)
			{
				m_socket->OnEvent(rtc::DE_WRITE, 0);
			}
			
			m_io_work.Wait(100, true); 
		}
		// exit info  try 
	}

	bool _read_into_buffer(rtc::AsyncSocket * socket, std::string& data)
	{
		 char buffer[0Xffff];
		do 
		{
			int bytes = socket->Recv(buffer, sizeof(buffer), nullptr);
			if (bytes <= 0)
			{
				break;
			}
			data.append(buffer, bytes);
		} while (true);
		return data.length() > 0 ? true : false;
			
	}
	// 后期放到session中成员变量 
	void _send_frame(wsheader_type::opcode_type type,const uint8_t* message_begin, size_t message_len)
	{
		// TODO:
		// Masking key should (must) be derived from a high quality random
		// number generator, to mitigate attacks on non-WebSocket friendly
		// middleware:

		std::vector<uint8_t> txbuf;
		const uint8_t masking_key[4] = { 0x12, 0x34, 0x56, 0x78 };



		// TODO: consider acquiring a lock on txbuf...

		std::vector<uint8_t> header;
		header.assign(2 + (message_len >= 126 ? 2 : 0) + (message_len >= 65536 ? 6 : 0) + (true ? 4 : 0), 0);
		header[0] = 0x80 | type;
		if (false) {}
		else if (message_len < 126) {
			header[1] = (message_len & 0xff) | (true ? 0x80 : 0);
			if (true) 
			{
				header[2] = masking_key[0];
				header[3] = masking_key[1];
				header[4] = masking_key[2];
				header[5] = masking_key[3];
			}
		}
		else if (message_len < 65536) {
			header[1] = 126 | (true ? 0x80 : 0);
			header[2] = (message_len >> 8) & 0xff;
			header[3] = (message_len >> 0) & 0xff;
			if (true) {
				header[4] = masking_key[0];
				header[5] = masking_key[1];
				header[6] = masking_key[2];
				header[7] = masking_key[3];
			}
		}
		else { // TODO: run coverage testing here
			header[1] = 127 | (true ? 0x80 : 0);
			header[2] = (message_len >> 56) & 0xff;
			header[3] = (message_len >> 48) & 0xff;
			header[4] = (message_len >> 40) & 0xff;
			header[5] = (message_len >> 32) & 0xff;
			header[6] = (message_len >> 24) & 0xff;
			header[7] = (message_len >> 16) & 0xff;
			header[8] = (message_len >> 8) & 0xff;
			header[9] = (message_len >> 0) & 0xff;
			if (true) {
				header[10] = masking_key[0];
				header[11] = masking_key[1];
				header[12] = masking_key[2];
				header[13] = masking_key[3];
			}
		}
		// N.B. - txbuf will keep growing until it can be transmitted over the socket:
		txbuf.insert(txbuf.end(), header.begin(), header.end());

		//write data
		size_t offset = txbuf.size();
		txbuf.resize(offset+message_len);
		for (size_t i = 0; i< message_len; ++i)
		{
			txbuf[offset+i] = message_begin[i];
		}

		if (true) 
		{
			size_t message_offset = txbuf.size() - message_len;
			for (size_t i = 0; i != message_len; ++i) 
			{
				txbuf[message_offset + i] ^= masking_key[i & 0x3];
			}
		}


		if (txbuf.size())
		{
			m_socket->Send(&txbuf[0], txbuf.size());
		
		}

	}


	void _push_msg(ENet_Type type, const std::string & msg)
	{
		clock_guard lock(m_msgs_lock);
		CNet_Message net_msg;
		net_msg.m_type = type;
		net_msg.m_message = msg;
		m_msgs.push_back(net_msg);
	}
public:
	 
	caddress					m_address;
	csocket*					m_socket; // 目前是客户端的唯一socket
	ESESSION_TYPE				m_status;  //后期单独封装到session中去  socket 
	cwork						m_io_work;
	cthread						m_thread;
	std::string					control_data_;
	std::string					onconnect_data_;
	std::vector<uint8_t>		m_recv_data;
	std::vector<uint8_t>		receivedData;
	std::mutex					m_msgs_lock;
	std::list<CNet_Message>	    m_msgs;
	uint64_t					m_websocket_protoo_id;
	
};
 
 


int main(int argc, char *argv[])
{
	
	cTestClient client;
	client.init("127.0.0.1", 8888);
	client.connect_to();
	client.startup();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}
	 

	return 0;
}



```

运行效果图

![在这里插入图片描述](https://img-blog.csdnimg.cn/4252e5316f7c456f86b5703721a05853.gif#pic_center)


[代码地址 :https://github.com/chensongpoixs/cwebrtc/blob/chensong/examples/websocket/main.cpp](https://github.com/chensongpoixs/cwebrtc/blob/chensong/examples/websocket/main.cpp)



# 总结

<font color=#999AAA > WebRTC网络跨平台






[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)









