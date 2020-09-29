---
layout:     post
title:      redis源码分析之bitops数据使用场景
subtitle:   缓存穿透,缓存击穿,缓存雪崩,统计数量汉明宽度
date:       2020-09-29
times:       22::22::24
author:     chensong
header-img: img/2019-02-02/bg_socketopt.jpg
catalog: 	 true
tags:
    - Redis源码探秘
    - 算法
---

=====================================================

redis源码学习系列文章：

[ redis源码分析之sha1算法分析](https://chensongpoixs.github.io/2019/10/01/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%E4%B9%8Bsha1%E7%AE%97%E6%B3%95%E5%88%86%E6%9E%90/)

[redis源码分析之字典源码分析](https://chensongpoixs.github.io/2019/10/14/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%E4%B9%8B%E5%AD%97%E5%85%B8%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/)

[redis源码分析之内存编码分析intset, ziplist编码分析](https://chensongpoixs.github.io/2019/10/10/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%E4%B9%8B%E5%86%85%E5%AD%98%E7%BC%96%E7%A0%81%E5%88%86%E6%9E%90/)

[redis源码分析之跳跃表](https://chensongpoixs.github.io/2019/04/15/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%E4%B9%8B%E8%B7%B3%E8%B7%83%E8%A1%A8/)

[ redis源码分析之内存淘汰策略的原理分析](https://chensongpoixs.github.io/2019/10/08/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%E4%B9%8B%E5%86%85%E5%AD%98%E6%B7%98%E6%B1%B0%E7%AD%96%E7%95%A5%E7%9A%84%E5%8E%9F%E7%90%86%E5%88%86%E6%9E%90/)

[redis源码分析之对象系统源码分析string, list链表,hash哈希,set集合,zset有序集合](https://chensongpoixs.github.io/2019/10/14/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%E4%B9%8B%E5%AF%B9%E8%B1%A1%E7%B3%BB%E7%BB%9F%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/)

[redis源码分析之异步进程保存数据rdb文件和aof文件源码分析](https://chensongpoixs.github.io/2019/10/19/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%E4%B9%8B%E5%BC%82%E6%AD%A5%E8%BF%9B%E7%A8%8B%E4%BF%9D%E5%AD%98%E6%95%B0%E6%8D%AErdb%E6%96%87%E4%BB%B6%E5%92%8Caof%E6%96%87%E4%BB%B6%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/)

[redis源码之sentinel高可用架构分析](https://chensongpoixs.github.io/2020/08/16/redis%E6%BA%90%E7%A0%81%E4%B9%8Bsentinel%E9%AB%98%E5%8F%AF%E7%94%A8%E6%9E%B6%E6%9E%84%E5%88%86%E6%9E%90/)


[redis源码分析之集群之一的槽的分配算法crc16原理分析](https://chensongpoixs.github.io/2020/08/16/redis%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%E4%B9%8B%E9%9B%86%E7%BE%A4%E4%B9%8B%E4%B8%80%E7%9A%84%E6%A7%BD%E7%9A%84%E5%88%86%E9%85%8D%E7%AE%97%E6%B3%95crc16%E5%8E%9F%E7%90%86%E5%88%86%E6%9E%90/)

=====================================================

在我的github上会持续更新Redis代码的中文分析，地址送出https://github.com/chensongpoixs/credis_source，共同学习进步


## 前言

redis中bitops的数据结构在  在这个数据结构中说存在时有可能存在也有可能不存在， 但是说不存在时就一定不存在的所以在高并发中使用的缓存穿透,缓存击穿,缓存雪崩

## 正文

### 一, setbit设置方法


一个字节等于八个bit  2^3




```

/* SETBIT key offset bitvalue */
void setbitCommand(client *c) {
    robj *o;
    char *err = "bit is not an integer or out of range";
    size_t bitoffset;
    ssize_t byte, bit;
    int byteval, bitval;
    long on;
    // 得到偏移量 offset的值
    if (getBitOffsetFromArgument(c,c->argv[2],&bitoffset,0,0) != C_OK)
        return;

    if (getLongFromObjectOrReply(c,c->argv[3],&on,err) != C_OK)
        return;

    /* Bits can only be set or cleared... */
    //只能设置一个比特位  on不是退出
    if (on & ~1) {
        addReplyError(c,err);
        return;
    }

    if ((o = lookupStringForBitCommand(c,bitoffset)) == NULL) return;

    /* Get current values */
    // 这里>> 3就相当于 除以 8 === > [2^3 = 8]
    byte = bitoffset >> 3;     //  例子: 53   --> 0011 0101 => 0000 0011
    byteval = ((uint8_t*)o->ptr)[byte];
    bit = 7 - (bitoffset & 0x7); // 底的3位拿到 -> 
    bitval = byteval & (1 << bit); // 0001 ====> 1000, 0100, 0010

    /* Update byte with new bit value and return original value */
    byteval &= ~(1 << bit);
    byteval |= ((on & 0x1) << bit);
    ((uint8_t*)o->ptr)[byte] = byteval;
    signalModifiedKey(c->db,c->argv[1]);
    notifyKeyspaceEvent(NOTIFY_STRING,"setbit",c->argv[1],c->db->id);
    server.dirty++;
    addReply(c, bitval ? shared.cone : shared.czero);
}

```

### 二, getbit方法

```
/* GETBIT key offset */
void getbitCommand(client *c) {
    robj *o;
    char llbuf[32];
    size_t bitoffset;
    size_t byte, bit;
    size_t bitval = 0;

    if (getBitOffsetFromArgument(c,c->argv[2],&bitoffset,0,0) != C_OK)
        return;

    if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.czero)) == NULL ||
        checkType(c,o,OBJ_STRING)) return;

    byte = bitoffset >> 3;
    bit = 7 - (bitoffset & 0x7);
    if (sdsEncodedObject(o)) {
        if (byte < sdslen(o->ptr))
            bitval = ((uint8_t*)o->ptr)[byte] & (1 << bit);
    } else {
        if (byte < (size_t)ll2string(llbuf,sizeof(llbuf),(long)o->ptr))
            bitval = llbuf[byte] & (1 << bit);
    }

    addReply(c, bitval ? shared.cone : shared.czero);
}
```



### 三, bitfield方法

### 四, bitop

### 五, bitcount 


这里redis使用两种方法进行统计二进制中有多少1

1. 查表法
2. 汉明宽度(分而治之的思想)


这里主要说明一下汉明宽度计算思想


//汉明宽度计算的思路是使用分而治之的思想处理的  
 
这里是先计算二进制下标[0,1],[2, 3][4,5],[6, 7][8,9]... 上的位置"1"的个数所以二个bit就可以存放了


![](https://github.com/chensongpoixs/chensongpoixs.github.io/blob/master/img/2020-09-29/hamming_weight.png?raw=true)


```
typedef unsigned int UINT32;
const UINT32 m1  = 0x55555555;  // 01010101010101010101010101010101
const UINT32 m2  = 0x33333333;  // 00110011001100110011001100110011
const UINT32 m4  = 0x0f0f0f0f;  // 00001111000011110000111100001111
const UINT32 m8  = 0x00ff00ff;  // 00000000111111110000000011111111
const UINT32 m16 = 0x0000ffff;  // 00000000000000001111111111111111
const UINT32 h01 = 0x01010101;  // the sum of 256 to the power of 0, 1, 2, 3

/* This is a naive implementation, shown for comparison, and to help in 
* understanding the better functions. It uses 20 arithmetic operations
* (shift, add, and). */
int popcount(UINT32 x)
{
  x = (x & m1) + ((x >> 1) & m1);
  x = (x & m2) + ((x >> 2) & m2);
  x = (x & m4) + ((x >> 4) & m4);
  x = (x & m8) + ((x >> 8) & m8);
  x = (x & m16) + ((x >> 16) & m16);
  return x;
}
```


 
 
``` 
 例子:  先一个bit为一个块, 再以4个bit为块的处理 以此类推  
				   0110 0010 
0011 0011  [公式 = ((x & 0011 0011) + ((x >> 1) & 0011 0011)) ]  => 0010 0001 
0000 1111  [公式 = ((x & 0000 1111) + ((x >> 4) & 0000 1111)) ]  => 0000 0011 
```     



```
/* Count number of bits set in the binary array pointed by 's' and long
 * 'count' bytes. The implementation of this function is required to
 * work with a input string length up to 512 MB. */
size_t redisPopcount(void *s, long count) {
    size_t bits = 0;
    unsigned char *p = s;
    uint32_t *p4;
    /**
     * 这边8个比特位对应"1"个数罗列出来了  char = > 256   使用空间换时间的做法
     */
    static const unsigned char bitsinbyte[256] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};

    /* Count initial bytes not aligned to 32 bit. */
    /// 这边有一个技巧  就是 & 0X3  低2位就进行查表, 低的2位不是0就走下面的的汉明重量 或者直接一个字节的查表
    while((unsigned long)p & 3 && count) {
        bits += bitsinbyte[*p++];
        count--;
    }

    /* Count bits 28 bytes at a time */
    //汉明宽度计算的思路是使用分而治之的思想处理的  
    /**
     *  例子:  先一个bit为一个块, 再以4个bit为块的处理 以此类推  
     *                   0110 0010 
     *   0011 0011  [公式 = ((x & 0011 0011) + ((x >> 1) & 0011 0011)) ]  => 0010 0001 
     *   0000 1111  [公式 = ((x & 0000 1111) + ((x >> 4) & 0000 1111)) ]  => 0000 0011 
     */
    // 汉明宽度处理是28个字节一处理比查表的28次要快 充分利用CPU
    p4 = (uint32_t*)p;
    while(count>=28) 
    {
        uint32_t aux1, aux2, aux3, aux4, aux5, aux6, aux7;

        aux1 = *p4++;
        aux2 = *p4++;
        aux3 = *p4++;
        aux4 = *p4++;
        aux5 = *p4++;
        aux6 = *p4++;
        aux7 = *p4++;
        count -= 28;
        // 0101 0101 0101 0101 0101 0101 0101 0101 ==> 0x55555555
        // 0011 0011 0011 0011 0011 0011 0011 0011 ==> 0x33333333
        // 0000 1111 0000 1111 0000 1111 0000 1111 ==> 0x0F0F0F0F
        // 0x01010101
        aux1 = aux1 - ((aux1 >> 1) & 0x55555555);                   // 0101 0101 0101 0101 0101 0101 0101 0101 ==> 0x55555555
        aux1 = (aux1 & 0x33333333) + ((aux1 >> 2) & 0x33333333);    // 0011 0011 0011 0011 0011 0011 0011 0011 ==> 0x33333333
        aux2 = aux2 - ((aux2 >> 1) & 0x55555555);
        aux2 = (aux2 & 0x33333333) + ((aux2 >> 2) & 0x33333333);
        aux3 = aux3 - ((aux3 >> 1) & 0x55555555);
        aux3 = (aux3 & 0x33333333) + ((aux3 >> 2) & 0x33333333);
        aux4 = aux4 - ((aux4 >> 1) & 0x55555555);
        aux4 = (aux4 & 0x33333333) + ((aux4 >> 2) & 0x33333333);
        aux5 = aux5 - ((aux5 >> 1) & 0x55555555);
        aux5 = (aux5 & 0x33333333) + ((aux5 >> 2) & 0x33333333);
        aux6 = aux6 - ((aux6 >> 1) & 0x55555555);
        aux6 = (aux6 & 0x33333333) + ((aux6 >> 2) & 0x33333333);
        aux7 = aux7 - ((aux7 >> 1) & 0x55555555);
        aux7 = (aux7 & 0x33333333) + ((aux7 >> 2) & 0x33333333);
        // 0000 1111 0000 1111 0000 1111 0000 1111 ==> 0x0F0F0F0F
        // 0000 0000 1111 1111 0000 0000 1111 1111 ==> 0x01010101
        // y = ( x + (x >> 4))
      
        bits += ((((aux1 + (aux1 >> 4)) & 0x0F0F0F0F) +  ((aux2 + (aux2 >> 4)) & 0x0F0F0F0F) + ((aux3 + (aux3 >> 4)) & 0x0F0F0F0F) + ((aux4 + (aux4 >> 4)) & 0x0F0F0F0F) + ((aux5 + (aux5 >> 4)) & 0x0F0F0F0F) + ((aux6 + (aux6 >> 4)) & 0x0F0F0F0F) + ((aux7 + (aux7 >> 4)) & 0x0F0F0F0F))* 0x01010101) >> 24; 
    }
    /* Count the remaining bytes. */
    p = (unsigned char*)p4;
    while(count--) 
    {
        bits += bitsinbyte[*p++];
    }
    return bits;
}

/* BITCOUNT key [start end] */
void bitcountCommand(client *c) {
    robj *o;
    long start, end, strlen;
    unsigned char *p;
    char llbuf[LONG_STR_SIZE];

    /* Lookup, check for type, and return 0 for non existing keys. */
    if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.czero)) == NULL ||
        checkType(c,o,OBJ_STRING)) return;
    p = getObjectReadOnlyString(o,&strlen,llbuf);

    /* Parse start/end range if any. */
    if (c->argc == 4) {
        if (getLongFromObjectOrReply(c,c->argv[2],&start,NULL) != C_OK)
            return;
        if (getLongFromObjectOrReply(c,c->argv[3],&end,NULL) != C_OK)
            return;
        /* Convert negative indexes */
        if (start < 0 && end < 0 && start > end) {
            addReply(c,shared.czero);
            return;
        }
        if (start < 0) start = strlen+start;
        if (end < 0) end = strlen+end;
        if (start < 0) start = 0;
        if (end < 0) end = 0;
        if (end >= strlen) end = strlen-1;
    } else if (c->argc == 2) {
        /* The whole string. */
        start = 0;
        end = strlen-1;
    } else {
        /* Syntax error. */
        addReply(c,shared.syntaxerr);
        return;
    }

    /* Precondition: end >= 0 && end < strlen, so the only condition where
     * zero can be returned is: start > end. */
    if (start > end) {
        addReply(c,shared.czero);
    } else {
        long bytes = end-start+1;

        addReplyLongLong(c,redisPopcount(p+start,bytes));
    }
}
```




### 六, bitops

## 结语