---
layout:     post
title:      WebRTC的ICE之STUN协议
subtitle:   ICE、STUN、媒体协商
date:       2022-05-20
times:       17:：29:：37 
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog:    true
tags:
    - WebRTC
    - 网络编程
  
---

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

<font color='read'>WebRTC是音视频行业的标杆， 如果要学习音视频， WebRTC是进入音视频行业最好方法， 里面可以有成熟方案， 例如：音频中3A 算法、网络评估、自适应码流、Simulcast、SVC等等 ， 非常适合刚刚进入音视频行业小伙伴哈^_^  我也是哦， 以后再音视频行业长期打算的小伙伴的学习项目。 里面有大量知识点  </font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/c120a00096ed435c956c9ea8d74da54c.png)




 

# 一、 STUN协议

![在这里插入图片描述](https://img-blog.csdnimg.cn/62c4a343586e49d685cd7a0cb99928ba.png)


 <font color='red'>STUN header和STUN Body组成的</font>

1. 包括20字节的STUN header
2. Body中可以有0个或多个Attribute

# 二、 STUN协议头格式

![在这里插入图片描述](https://img-blog.csdnimg.cn/108fd244f2294350a30db65b5515b7f9.png)


# 三、 STUN Header

1.   <font color='red'>2个字节</font>(16bit)类型
2.   <font color='red'>2个字节</font>(16bit)信息长度、不包括信息头
3.   <font color='red'>16个字节</font>(128bit)事务ID、请求与响应事务ID相同

# 四、STUN Message Type

1. 前两位必须是00、以区分复用同一个端口时STUN协议
2. 2位用于分类、即C0和C1
3. 12 位用于定义请求/指示



 <font color='red'>C表示分类，M 表示方法</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/04eba40f08824742b597b287584215ec.png)

## 1 、C0C1


1. 0b00:   表示是一个请求
2. 0b01:   表示是一个指示
3. 0b10:   表式是请求成功的响应
4. 0b11:   表示是请求失败的响应


## 2、 STUN信息类型

![在这里插入图片描述](https://img-blog.csdnimg.cn/12977433035c41f7bc66c30ac67999bc.png)


## 3 、大小端模式


大端模式:  数据的高字节保存在内存的低地址中

小端模式:  数据的高字节保存在内存的高地址中

<font color='red'>网络字节顺序:  采用大端排序方式</font>


# 五、 StunMagicCookie


4字节、32位、固定值0x2112A442。通过它可以判断客户端是否可以支持某些属性



# 六、Transaction ID事务ID

12字节、96位、标识同一个事物的请求和响应

# 七、STUN Message Body

1. 信息头后有0或多个属性
2. 每个塑性进行TLV编码: Type、Length、Value

![在这里插入图片描述](https://img-blog.csdnimg.cn/0a17aa8dc35c44cab83e84aaebbc85a4.png)

# 八、RFC 3489定义的属性

![在这里插入图片描述](https://img-blog.csdnimg.cn/2545ce231a1f416ea7908f518da71fab.png)



# 九、 WebRTC中代码

```cpp

bool StunMessage::Read(ByteBufferReader* buf) {
  if (!buf->ReadUInt16(&type_))
    return false;

  if (type_ & 0x8000) {
    // RTP and RTCP set the MSB of first byte, since first two bits are version,
    // and version is always 2 (10). If set, this is not a STUN packet.
    return false;
  }

  if (!buf->ReadUInt16(&length_))
    return false;

  std::string magic_cookie;//0x2112A442
  if (!buf->ReadString(&magic_cookie, kStunMagicCookieLength))
    return false;

  std::string transaction_id; // 12bit
  if (!buf->ReadString(&transaction_id, kStunTransactionIdLength))
    return false;

  uint32_t magic_cookie_int;
  static_assert(sizeof(magic_cookie_int) == kStunMagicCookieLength,
                "Integer size mismatch: magic_cookie_int and kStunMagicCookie");
  std::memcpy(&magic_cookie_int, magic_cookie.data(), sizeof(magic_cookie_int));
  if (rtc::NetworkToHost32(magic_cookie_int) != kStunMagicCookie) {
    // If magic cookie is invalid it means that the peer implements
    // RFC3489 instead of RFC5389.
    transaction_id.insert(0, magic_cookie);
  }
  RTC_DCHECK(IsValidTransactionId(transaction_id));
  transaction_id_ = transaction_id;
  reduced_transaction_id_ = ReduceTransactionId(transaction_id_);
  printf(
      "type_ = %u, magic_cookie = %s, transaction_id = %s, magic_cookie_int = "
      "%u\n ",
      type_, magic_cookie.c_str(), transaction_id.c_str(), magic_cookie_int);
  if (length_ != buf->Length())
    return false;

  attrs_.resize(0);

  size_t rest = buf->Length() - length_;
  while (buf->Length() > rest) {
    uint16_t attr_type, attr_length;
    if (!buf->ReadUInt16(&attr_type))
      return false;
    if (!buf->ReadUInt16(&attr_length))
      return false;

    std::unique_ptr<StunAttribute> attr(
        CreateAttribute(attr_type, attr_length));
    if (!attr) {
      // Skip any unknown or malformed attributes.
      if ((attr_length % 4) != 0) {
        attr_length += (4 - (attr_length % 4));
      }
      if (!buf->Consume(attr_length))
        return false;
    } else {
      if (!attr->Read(buf))
        return false;
      attrs_.push_back(std::move(attr));
    }
  }

  RTC_DCHECK(buf->Length() == rest);
  return true;
}
```


# 总结：

[WebRTC源码分析地址：https://github.com/chensongpoixs/cwebrtc](https://github.com/chensongpoixs/cwebrtc)

