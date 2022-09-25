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


# WebRTC的ICE之STUN协议


 

<font color='red'>WebRTC专题开嗨鸭 ！！！  </font>


# 前言

<font color=#999AAA >WebRTC是音视频行业的标杆， 如果要学习音视频， WebRTC是进入音视频行业最好方法， 里面可以有成熟方案， 例如：音频中3A 算法、网络评估、自适应码流、Simulcast、SVC等等 ， 非常适合刚刚进入音视频行业小伙伴哈^_^  我也是哦， 以后再音视频行业长期打算的小伙伴的学习项目。 里面有大量知识点  </font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/c120a00096ed435c956c9ea8d74da54c.png)





<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

<font color=#999AAA >提示：以下是本篇文章正文内容，下面案例可供参考


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

