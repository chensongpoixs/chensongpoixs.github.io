---
layout:     post
title:      WebRTC的ICE之Dtls/SSL/TLSv1.x协议详解
subtitle:   STUN、ICE、DTLS、OpenSSL
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

# WebRTC的ICE之Dtls/SSL/TLSv1.x协议详解


@[TOC](WebRTC的ICE之Dtls/SSL/TLSv1.x协议详解)

</font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">



<font color='red'>WebRTC的ICE之Dtls/SSL/TLSv1.x协议详解</font>


# 前言


![在这里插入图片描述](https://img-blog.csdnimg.cn/0afe36787230404e803b7ccc31a48e01.png)



![在这里插入图片描述](https://img-blog.csdnimg.cn/1a8e5732d1454b579d98b8f127ba32ca.png)

## 一、 SSL/TLSv1.x的作用

（1）身份认证
通过证书认证来确认对方的身份，防止中间人攻击
（2）数据私密性
使用对称性密钥加密传输的数据，由于密钥只有客户端/服务端有，其他人无法窥探。
（3）数据完整性
使用摘要算法对报文进行计算，收到消息后校验该值防止数据被篡改或丢失。


## 二、 SSL/TLSv1.x传输的步骤

其中1-4是握手阶段，5是指握手后双方使用商议好的秘钥进行通讯。

中并列着Server Hello,Certificate等多个类型，是因为这是一个Multiple Handshake Messages，一次性发送多个握手协议包。

传输过程总体来说：

（1）客户端提供【客户端随机数、可选算法套件、sessionId】等信息
（2）服务端提供【服务端随机数、选用算法套件、sessionId】等信息
（3）服务端提供证书
（4）服务端与客户端互换算法需要的参数
（5）客户端根据前面提到的随机数和参数生成master secret，确认开始使用指定算法加密，并将握手信息加密传输给服务端，用来校验握手信息、秘钥是否正确
（6）服务端进行与（5）一样的操作
（7）客户端、服务端可以用master secret进行加密通讯

## 三、传输中参数介绍
1、 cipher suites
每个算法套件是一组算法，以   TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256为例。
其中（1）ECDHE用于协商秘钥
（2）RSA是用于身份验证
（3）AES_138_GCM用于对称加密通讯
（4）SHA256用于生成摘要，验证数据完整性

## 四、SSL/TLSv1.x四次握手

![在这里插入图片描述](https://img-blog.csdnimg.cn/112427dfee1340d89a1b634c3423b9bf.png)

### 1、第一次握手

![在这里插入图片描述](https://img-blog.csdnimg.cn/1d11616e733c44d496964061d16cb749.png)


四次握手的第一次握手是客户端向服务器发送Client Hello消息，消息以明文的形式传输，里面包括客户端支持的协议版本、加密套件、压缩算法、客户端生成的一个随机数R1、扩展字段等。其中加密套件是四个功能的组合，即：认证算法（Au）、密钥交换算法（KeyExchange）、对称加密算法（Enc）和信息摘要算法，随机数R1则会在后面的密钥生成中使用到。

### 2、第二次握手

![在这里插入图片描述](https://img-blog.csdnimg.cn/16f26e3f99c149cdb526bcdf0480ff6c.png)

① 应对客户端发来的Client Hello，服务器将发送Server Hello消息进行响应，该消息以明文的形式传输，相应消息包括确认使用的协议版本、由服务器生成的随机数R2，确认使用的加密套件、确认使用的压缩方法。

② 在发完Server Hello消息后，服务器会马上将自己的Certificate（公钥证书）发送给客户端。
③ Server Key Exchange并非必需选项，只有在选用了DH算法的情况下，服务器需要将DH参数发送给客户端，若选择了RSA算法则不需要发送Server Key Exchange。
④ Certificate Request也并非必须选项，在对于安全性要求较高的场景中，服务器可要对客户端的身份进行认证，因此发起了对客户端公钥证书的请求，一般情况下浏览器都会内置一对独一无二的公私钥。
⑤ 由于第二次握手中包含一些可选选项，因此需要服务器发送一个Server Hello Done的消息，用来通知客户端Server Hello过程结束。在客户端收到Server Hello Done之后并没有马上进行第三次握手，而是先对服务器传来的证书进行验证，一般会验证证书是否在有效期内，随后根据CRL或者OCSP查询证书是否有效，最后根据证书链从根CA开始验证直到网站证书，以确保证书的真实性。在这个过程中若出现了验证不通过的结果，则抛出相应的错误；若验证通过，就再生成一个随机数Pre-master，并用服务器公钥进行加密，生成PreMaster Key。

### 3、 第三次握手

① Client Key Exchange就是客户端将PreMaster Key发送给服务器，服务器则会用自己的私钥解密得出Pre-master。到这里客户端和服务器都拥有了三个随机数R1、R2和Pre-master，两边再用相同的算法和这三个随机数生成一个密钥，用于握手结束后传输数据的对称加密。
② Change Cipher Spec是客户端向服务器通知，后面发送的消息都会使用协商出来的密钥进行加密。
③ Encrypted Handshake Message是客户端向服务发送握手数据加密信息，该信息是客户端将前面的握手消息利用协商好的摘要算法生成摘要，再用协商好的密钥对摘要进行加密而的出来的，最后将加密信息发送给服务器，这是客户端发出的第一条加密信息。而服务器也会用协商好的密钥进行解密，若能成功解密则说明协商出来的密钥是一致的。
④ Certificate是在第二次握手的第4步有进行的情况下，即服务器有向客户端请求证书的情况才会有的，这一步是客户端向服务器发送客户端的证书，而服务器收到证书后也会对证书进行相同的验证。


### 4、 第四次握手

① Change Cipher Spec是服务器向客户端通知，后面发送的消息都会使用协商出来的密钥进行加密。

② Encrypted Handshake Message与第三次握手类似，是服务器发给客户端的用来确定协商的密钥是一致的，也是一条Server Finish消息。至此TLS四次握手也就完成了，双方已经协商好使用的加密套件和对称密钥，接下来的交互数据都将经过加密后再使用TCP进行传输。可以看出，TLS四次握手的过程是相对复杂的，要消耗一定的资源，若每次建立HTTPS连接都要进行TLS四次握手的话将会消耗较多的资源，导致效率较低。为了提高建立HTTPS连接的速度，TLS协议设置了两种绘画缓存机制：session ID和session ticket。其中session ID是协议中标准字段，所以基本所有服务器都支持，session ID和协商的通讯信息会保存在服务器端；而session ticket是一个扩展字段，需要服务器和客户端都支持，服务器会将协商的通讯信息加密后发送给客户端保存，密钥只有服务器直到，这就占用了较少的服务器资源。


这里的Client Hello多了Session ID（或Session Ticket）参数。且客户端在发送完最后一个握手数据包后就直接开始向服务器发送应用数据。



## 五、 实践

```cpp
/***********************************************************************************************
created: 		2022-05-22

author:			chensong

purpose:		TLSv1.3 协议的学习

原因是WebRTC中有DTSTransport -->
************************************************************************************************/

#include "dtlsv1.x.h"




namespace chen {
	
	///////////////////////////////////////////////////////////////////////////////////////
	enum class CryptoSuite
	{
		NONE = 0,
		AES_CM_128_HMAC_SHA1_80 = 1,
		AES_CM_128_HMAC_SHA1_32,
		AEAD_AES_256_GCM,
		AEAD_AES_128_GCM
	};
	struct SrtpCryptoSuiteMapEntry
	{
		CryptoSuite cryptoSuite;
		const char* name;
	};


	

	////////////////////////////////////////////////////////////////
	std::vector<SrtpCryptoSuiteMapEntry>  srtpCryptoSuites =
	{
		{ CryptoSuite::AEAD_AES_256_GCM, "SRTP_AEAD_AES_256_GCM" },
		{ CryptoSuite::AEAD_AES_128_GCM, "SRTP_AEAD_AES_128_GCM" },
		{ CryptoSuite::AES_CM_128_HMAC_SHA1_80, "SRTP_AES128_CM_SHA1_80" },
		{ CryptoSuite::AES_CM_128_HMAC_SHA1_32, "SRTP_AES128_CM_SHA1_32" }
	};



	std::map<std::string, FingerprintAlgorithm> string2FingerprintAlgorithm =
	{
		{ "sha-1",  FingerprintAlgorithm::SHA1   },
		{ "sha-224",FingerprintAlgorithm::SHA224 },
		{ "sha-256",FingerprintAlgorithm::SHA256 },
		{ "sha-384",FingerprintAlgorithm::SHA384 },
		{ "sha-512",FingerprintAlgorithm::SHA512 }
	};

	///////////////////////////////////////////////////
	



	const char * dtlsCertificateFile = "./certs/fullchain.pem";
	const char * dtlsPrivateKeyFile = "./certs/privkey.pem";


	// clang-format off
	static  int DtlsMtu{ 1350 };
	static constexpr int SslReadBufferSize{ 65536 };
	/* Class variables. */

	thread_local X509*  certificate{ nullptr };
	thread_local EVP_PKEY*  privateKey{ nullptr };
	thread_local SSL_CTX*  sslCtx{ nullptr };
	thread_local uint8_t  sslReadBuffer[SslReadBufferSize];
	thread_local std::vector<Fingerprint>  localFingerprints;



	void dtlsv1x::GenerateCertificateAndPrivateKey()
	{
		 

		int ret{ 0 };
		EC_KEY* ecKey{ nullptr };
		X509_NAME* certName{ nullptr };
		std::string subject = std::string("chensong") + std::to_string(999999);

		// 1. 使用曲线创建关键点。
		ecKey = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);

		if (!ecKey)
		{
			ERROR_EX_LOG("EC_KEY_new_by_curve_name() failed");

			goto error;
		}

		EC_KEY_set_asn1_flag(ecKey, OPENSSL_EC_NAMED_CURVE);

		// NOTE: This can take some time.
		ret = EC_KEY_generate_key(ecKey);

		if (ret == 0)
		{
			ERROR_EX_LOG("EC_KEY_generate_key() failed");

			goto error;
		}

		// 2. 创建私钥对象。
		privateKey = EVP_PKEY_new();

		if (!privateKey)
		{
			ERROR_EX_LOG("EVP_PKEY_new() failed");

			goto error;
		}

		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		ret = EVP_PKEY_assign_EC_KEY( privateKey, ecKey);

		if (ret == 0)
		{
			ERROR_EX_LOG("EVP_PKEY_assign_EC_KEY() failed");

			goto error;
		}

		// The EC key now belongs to the private key, so don't clean it up separately.
		ecKey = nullptr;

		// 3. 创建 X509 证书.
		 certificate = X509_new();

		if (!  certificate)
		{
			ERROR_EX_LOG("X509_new() failed");

			goto error;
		}

		// Set version 3 (note that 0 means version 1). 
		X509_set_version(  certificate, 2);

		// Set serial number (avoid default 0).
		ASN1_INTEGER_set(
			X509_get_serialNumber(  certificate),
			static_cast<uint64_t>(9999999));

		// Set valid period.
		X509_gmtime_adj(X509_get_notBefore(  certificate), -315360000); // -10 years.
		X509_gmtime_adj(X509_get_notAfter( certificate), 315360000);   // 10 years.

		// Set the public key for the certificate using the key.
		ret = X509_set_pubkey( certificate,  privateKey);

		if (ret == 0)
		{
			ERROR_EX_LOG("X509_set_pubkey() failed");

			goto error;
		}

		// Set certificate fields.
		certName = X509_get_subject_name(  certificate);

		if (!certName)
		{
			ERROR_EX_LOG("X509_get_subject_name() failed");

			goto error;
		}

		X509_NAME_add_entry_by_txt(
			certName, "O", MBSTRING_ASC, reinterpret_cast<const uint8_t*>(subject.c_str()), -1, -1, 0);
		X509_NAME_add_entry_by_txt(
			certName, "CN", MBSTRING_ASC, reinterpret_cast<const uint8_t*>(subject.c_str()), -1, -1, 0);

		// It is self-signed so set the issuer name to be the same as the subject.
		ret = X509_set_issuer_name(  certificate, certName);

		if (ret == 0)
		{
			ERROR_EX_LOG("X509_set_issuer_name() failed");

			goto error;
		}

		// Sign the certificate with its own private key.
		ret = X509_sign( certificate,  privateKey, EVP_sha1());

		if (ret == 0)
		{
			ERROR_EX_LOG("X509_sign() failed");

			goto error;
		}

		return;

	error:

		if (ecKey)
		{
			EC_KEY_free(ecKey);
		}

		if (privateKey)
		{
			EVP_PKEY_free(privateKey); // NOTE: This also frees the EC key.
		}

		if (certificate)
		{
			X509_free(certificate);
		}

		ERROR_EX_LOG("DTLS certificate and private key generation failed");
	}

	// 读取公钥和私钥
	void dtlsv1x::ReadCertificateAndPrivateKeyFromFiles()
	{
		FILE* file{ nullptr };
		DEBUG_EX_LOG("");
		file = fopen(dtlsCertificateFile, "r");

		if (!file)
		{
			ERROR_EX_LOG("error reading DTLS certificate file: %s", std::strerror(errno));

			return;
		}
		DEBUG_EX_LOG("");
		certificate = PEM_read_X509(file, nullptr, nullptr, nullptr);

		if (!certificate)
		{
			ERROR_EX_LOG("PEM_read_X509() failed");

			return;
		}
		DEBUG_EX_LOG("");
		fclose(file);

		file = fopen(dtlsPrivateKeyFile, "r");

		if (!file)
		{
			ERROR_EX_LOG("error reading DTLS private key file: %s", std::strerror(errno));

			return;
		}

		privateKey = PEM_read_PrivateKey(file, nullptr, nullptr, nullptr);

		if (!privateKey)
		{
			ERROR_EX_LOG("PEM_read_PrivateKey() failed");

			return;
		}

		fclose(file);
	}

	//DTLS状态回调
	inline static void onSslInfo(const SSL* ssl, int where, int ret)
	{
		//DEBUG_EX_LOG("[where = %d][ret = %d]", where, ret);
		static_cast<dtlsv1x*>(SSL_get_ex_data(ssl, 0))->OnSslInfo(where, ret);
	}

	/* Static methods for OpenSSL callbacks. */

	inline static int onSslCertificateVerify(int /*preverifyOk*/, X509_STORE_CTX* /*ctx*/)
	{

		//DEBUG_EX_LOG("OpenSSL callbacks");
		// Always valid since DTLS certificates are self-signed.
		return 1;
	}

	void  dtlsv1x::CreateSslCtx()
	{


		std::string dtlsSrtpCryptoSuites;
		int ret;

		/* Set the global DTLS context. */

		// Both DTLS 1.0 and 1.2 (requires OpenSSL >= 1.1.0).
		sslCtx = SSL_CTX_new(DTLS_method());

		if (!sslCtx)
		{
			ERROR_EX_LOG("SSL_CTX_new() failed");

			goto error;;
		}

		ret = SSL_CTX_use_certificate(sslCtx, certificate);

		if (ret == 0)
		{
			ERROR_EX_LOG("SSL_CTX_use_certificate() failed");

			goto error;;
		}

		ret = SSL_CTX_use_PrivateKey(sslCtx, privateKey);

		if (ret == 0)
		{
			ERROR_EX_LOG("SSL_CTX_use_PrivateKey() failed");

			goto error;;
		}

		ret = SSL_CTX_check_private_key(sslCtx);

		if (ret == 0)
		{
			ERROR_EX_LOG("SSL_CTX_check_private_key() failed");

			goto error;;
		}

		// Set options.
		SSL_CTX_set_options(
			sslCtx,
			SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_NO_TICKET | SSL_OP_SINGLE_ECDH_USE |
			SSL_OP_NO_QUERY_MTU);

		// Don't use sessions cache.
		SSL_CTX_set_session_cache_mode(sslCtx, SSL_SESS_CACHE_OFF);

		// Read always as much into the buffer as possible.
		// NOTE: This is the default for DTLS, but a bug in non latest OpenSSL
		// versions makes this call required.
		SSL_CTX_set_read_ahead(sslCtx, 1);

		SSL_CTX_set_verify_depth(sslCtx, 4);

		// Require certificate from peer.
		SSL_CTX_set_verify(
			sslCtx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, onSslCertificateVerify);

		// Set SSL info callback.
		SSL_CTX_set_info_callback(sslCtx, onSslInfo);

		// Set ciphers.
		ret = SSL_CTX_set_cipher_list(
			sslCtx, "DEFAULT:!NULL:!aNULL:!SHA256:!SHA384:!aECDH:!AESGCM+AES256:!aPSK");

		if (ret == 0)
		{
			ERROR_EX_LOG("SSL_CTX_set_cipher_list() failed");

			goto error;;
		}

		// Enable ECDH ciphers.
		// DOC: http://en.wikibooks.org/wiki/OpenSSL/Diffie-Hellman_parameters
		// NOTE: https://code.google.com/p/chromium/issues/detail?id=406458
		// NOTE: https://bugs.ruby-lang.org/issues/12324

		// For OpenSSL >= 1.0.2.
		SSL_CTX_set_ecdh_auto( sslCtx, 1);

		// Set the "use_srtp" DTLS extension.
		for (auto it = srtpCryptoSuites.begin(); it != srtpCryptoSuites.end(); ++it)
		{
			if (it != srtpCryptoSuites.begin())
			{
				dtlsSrtpCryptoSuites += ":";
			}

			SrtpCryptoSuiteMapEntry* cryptoSuiteEntry = std::addressof(*it);
			dtlsSrtpCryptoSuites += cryptoSuiteEntry->name;
		}

		DEBUG_EX_LOG("setting SRTP cryptoSuites for DTLS: %s", dtlsSrtpCryptoSuites.c_str());

		// NOTE: This function returns 0 on success.
		ret = SSL_CTX_set_tlsext_use_srtp(sslCtx, dtlsSrtpCryptoSuites.c_str());

		if (ret != 0)
		{
			ERROR_EX_LOG(
				"SSL_CTX_set_tlsext_use_srtp() failed when entering '%s'", dtlsSrtpCryptoSuites.c_str());
			ERROR_EX_LOG("SSL_CTX_set_tlsext_use_srtp() failed");

			goto error;
		}

		return;

	error:

		if (sslCtx)
		{
			SSL_CTX_free(sslCtx);
			sslCtx = nullptr;
		}

		ERROR_EX_LOG("SSL context creation failed");
	}

	static FingerprintAlgorithm GetFingerprintAlgorithm(const std::string& fingerprint)
	{
		auto it = string2FingerprintAlgorithm.find(fingerprint);

		if (it != string2FingerprintAlgorithm.end())
		{
			return it->second;
		}
		 
		return  FingerprintAlgorithm::NONE;
		 
	}
	void dtlsv1x::GenerateFingerprints()
	{


		for (auto& kv : string2FingerprintAlgorithm)
		{
			const std::string& algorithmString = kv.first;
			FingerprintAlgorithm algorithm = kv.second;
			uint8_t binaryFingerprint[EVP_MAX_MD_SIZE];
			unsigned int size{ 0 };
			char hexFingerprint[(EVP_MAX_MD_SIZE * 3) + 1];
			const EVP_MD* hashFunction = NULL;;
			int ret;

			switch (algorithm)
			{
				case FingerprintAlgorithm::SHA1:
				{
					hashFunction = EVP_sha1();
					break;
				}

				case FingerprintAlgorithm::SHA224:
				{
					hashFunction = EVP_sha224();
					break;
				}

				case FingerprintAlgorithm::SHA256:
				{
					hashFunction = EVP_sha256();
					break;
				}

				case FingerprintAlgorithm::SHA384:
				{
					hashFunction = EVP_sha384();
					break;
				}

				case FingerprintAlgorithm::SHA512:
				{
					hashFunction = EVP_sha512();
					break;
				}

				default:
				{
					ERROR_EX_LOG("unknown algorithm");
				}
			}

			ret = X509_digest(certificate, hashFunction, binaryFingerprint, &size);

			//(void *)hashFunction;
			if (ret == 0)
			{
				ERROR_EX_LOG("X509_digest() failed");
				ERROR_EX_LOG("Fingerprints generation failed");
			}

			// Convert to hexadecimal format in uppercase with colons.
			for (unsigned int i{ 0 }; i < size; ++i)
			{
				std::sprintf(hexFingerprint + (i * 3), "%.2X:", binaryFingerprint[i]);
			}
			hexFingerprint[(size * 3) - 1] = '\0';

			DEBUG_EX_LOG("%-7s fingerprint: %s", algorithmString.c_str(), hexFingerprint);

			// Store it in the vector.
			Fingerprint fingerprint;

			fingerprint.algorithm = GetFingerprintAlgorithm(algorithmString);
			fingerprint.value = hexFingerprint;

			localFingerprints.push_back(fingerprint);
		}
	}

	void dtlsv1x::Run(Role localRole)
	{
		Role previousLocalRole = this->localRole;

		if (localRole == previousLocalRole)
		{
			ERROR_EX_LOG("[server_name = %s]same local DTLS role provided, doing nothing", m_server_name.c_str());

			return;
		}

		// If the previous local DTLS role was 'client' or 'server' do reset.
		if (previousLocalRole == Role::CLIENT || previousLocalRole == Role::SERVER)
		{
			DEBUG_EX_LOG("[server_name = %s]resetting DTLS due to local role change", m_server_name.c_str());

			Reset();
		}

		// Update local role.
		this->localRole = localRole;

		// Set state and notify the listener.
		this->state = DtlsState::CONNECTING;
		 

		switch (this->localRole)
		{
			case Role::CLIENT:
			{
				DEBUG_EX_LOG("running [role:client]");

				///  ????????????????????????????? dtls ???? 交换协商的流程

				SSL_set_connect_state(this->ssl);
				SSL_do_handshake(this->ssl);
				SendPendingOutgoingDtlsData();


				break;
			}

			case Role::SERVER:
			{
				DEBUG_EX_LOG("running [role:server]");

				SSL_set_accept_state(this->ssl);
				SSL_do_handshake(this->ssl);

				break;
			}

			default:
			{
				ERROR_EX_LOG("[server_name = %s]invalid local DTLS role", m_server_name.c_str());
			}

		}
	}

	void dtlsv1x::ProcessDtlsData(const uint8_t * data, size_t len)
	{
		DEBUG_EX_LOG("[server_name = %s] ", m_server_name.c_str());
		dtsl_data temp_data;
		temp_data.data = new uint8_t[len];
		memcpy(temp_data.data, data, len);
		temp_data.size = len;
		{
			std::lock_guard<std::mutex> locak(m_mutex);

			m_quene.push_back(temp_data);
		}
	}


	void dtlsv1x::Reset()
	{
		int ret;

		if (!IsRunning())
		{
			DEBUG_EX_LOG("");
			return;
		}

		WARNING_EX_LOG( "[server_name = %s]resetting DTLS transport", m_server_name.c_str());

		// Stop the DTLS timer.
		//this->timer->Stop();

		// We need to reset the SSL instance so we need to "shutdown" it, but we
		// don't want to send a Close Alert to the peer, so just don't call
		// SendPendingOutgoingDTLSData().
		SSL_shutdown(this->ssl);

		this->localRole = Role::NONE;
		this->state = DtlsState::NEW;
		this->handshakeDone = false;
		this->handshakeDoneNow = false;

		// Reset SSL status.
		// NOTE: For this to properly work, SSL_shutdown() must be called before.
		// NOTE: This may fail if not enough DTLS handshake data has been received,
		// but we don't care so just clear the error queue.
		ret = SSL_clear(this->ssl);

		if (ret == 0)
		{
			ERR_clear_error();
		}
	}

	bool dtlsv1x::CheckStatus(int returnCode)
	{
		int err;
		bool wasHandshakeDone = this->handshakeDone;

		err = SSL_get_error(this->ssl, returnCode);

		switch (err)
		{
		case SSL_ERROR_NONE:
			break;

		case SSL_ERROR_SSL:
			ERROR_EX_LOG("[server_name = %s]SSL status: SSL_ERROR_SSL", m_server_name.c_str());
			break;

		case SSL_ERROR_WANT_READ:
			break;

		case SSL_ERROR_WANT_WRITE:
			WARNING_EX_LOG(  "[server_name = %s]SSL status: SSL_ERROR_WANT_WRITE", m_server_name.c_str());
			break;

		case SSL_ERROR_WANT_X509_LOOKUP:
			DEBUG_EX_LOG(  "[server_name = %s]SSL status: SSL_ERROR_WANT_X509_LOOKUP", m_server_name.c_str());
			break;

		case SSL_ERROR_SYSCALL:
			ERROR_EX_LOG("[server_name = %s]SSL status: SSL_ERROR_SYSCALL", m_server_name.c_str());
			break;

		case SSL_ERROR_ZERO_RETURN:
			break;

		case SSL_ERROR_WANT_CONNECT:
			WARNING_EX_LOG( "[server_name = %s]SSL status: SSL_ERROR_WANT_CONNECT", m_server_name.c_str());
			break;

		case SSL_ERROR_WANT_ACCEPT:
			WARNING_EX_LOG( "[server_name = %s]SSL status: SSL_ERROR_WANT_ACCEPT", m_server_name.c_str());
			break;

		default:
			WARNING_EX_LOG(  "[server_name = %s]SSL status: unknown error", m_server_name.c_str());
		}
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		// Check if the handshake (or re-handshake) has been done right now.
		if (this->handshakeDoneNow)
		{
			this->handshakeDoneNow = false;
			this->handshakeDone = true;

			// Stop the timer.
			//this->timer->Stop();
			DEBUG_EX_LOG("[server_name = %s][wasHandshakeDone = %u][remoteFingerprint.algorithm = %u]", m_server_name.c_str(), wasHandshakeDone, remoteFingerprint.algorithm);
			// Process the handshake just once (ignore if DTLS renegotiation).
			if (!wasHandshakeDone && this->remoteFingerprint.algorithm != FingerprintAlgorithm::NONE)
			{
				DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
				return ProcessHandshake();
			}

			return true;
		}
		// Check if the peer sent close alert or a fatal error happened.
		else if (((SSL_get_shutdown(this->ssl) & SSL_RECEIVED_SHUTDOWN) != 0) || err == SSL_ERROR_SSL || err == SSL_ERROR_SYSCALL)
		{
			if (this->state == DtlsState::CONNECTED)
			{
				DEBUG_EX_LOG(  "[server_name = %s]disconnected", m_server_name.c_str());

				Reset();

				// Set state and notify the listener.
				this->state = DtlsState::CLOSED; 
			}
			else
			{
				WARNING_EX_LOG(  "[server_name = %s]connection failed", m_server_name.c_str());

				Reset();
 
				this->state = DtlsState::FAILED;
				 
			}

			return false;
		}

		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		 return true;
		 
	}

	bool dtlsv1x::ProcessHandshake()
	{
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		// Validate the remote fingerprint.
		if (!CheckRemoteFingerprint())
		{
			Reset();

			// Set state and notify the listener.
			this->state = DtlsState::FAILED;
			 

			return false;
		}
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		 
		return true;
	 
		 
	}

	bool dtlsv1x::CheckRemoteFingerprint()
	{
		X509* certificate;
		uint8_t binaryFingerprint[EVP_MAX_MD_SIZE];
		unsigned int size{ 0 };
		char hexFingerprint[(EVP_MAX_MD_SIZE * 3) + 1];
		const EVP_MD* hashFunction = NULL;
		int ret;
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		certificate = SSL_get_peer_certificate(this->ssl);

		if (!certificate)
		{
			WARNING_EX_LOG(  "[server_name = %s]no certificate was provided by the peer", m_server_name.c_str());

			return false;
		}
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		switch (this->remoteFingerprint.algorithm)
		{
			case FingerprintAlgorithm::SHA1:
			{
				hashFunction = EVP_sha1();
				break;

			}
			case FingerprintAlgorithm::SHA224:
			{
				hashFunction = EVP_sha224();
				break;
			}

			case FingerprintAlgorithm::SHA256:
			{
				hashFunction = EVP_sha256();
				break;
			}

			case FingerprintAlgorithm::SHA384:
			{
				hashFunction = EVP_sha384();
				break;
			}

			case FingerprintAlgorithm::SHA512:
			{
				hashFunction = EVP_sha512();
				break;
			}

			default:
			{
				ERROR_EX_LOG("[server_name = %s]unknown algorithm", m_server_name.c_str());
			}
		}

		// Compare the remote fingerprint with the value given via signaling.
		ret = X509_digest(certificate, hashFunction, binaryFingerprint, &size);
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		//(void *)hashFunction;
		if (ret == 0)
		{
			ERROR_EX_LOG("[server_name = %s]X509_digest() failed", m_server_name.c_str());

			X509_free(certificate);

			return false;
		}

		// Convert to hexadecimal format in uppercase with colons.
		for (unsigned int i{ 0 }; i < size; ++i)
		{
			std::sprintf(hexFingerprint + (i * 3), "%.2X:", binaryFingerprint[i]);
		}
		hexFingerprint[(size * 3) - 1] = '\0';

		if (this->remoteFingerprint.value != hexFingerprint)
		{
			WARNING_EX_LOG( 
				"[server_name = %s]fingerprint in the remote certificate (%s) does not match the announced one (%s)", m_server_name.c_str(),
				hexFingerprint,
				this->remoteFingerprint.value.c_str());

			X509_free(certificate);

			return false;
		}

		DEBUG_EX_LOG(  "[server_name = %s]valid remote fingerprint", m_server_name.c_str());

		// Get the remote certificate in PEM format.

		BIO* bio = BIO_new(BIO_s_mem());

		// Ensure the underlying BUF_MEM structure is also freed.
		// NOTE: Avoid stupid "warning: value computed is not used [-Wunused-value]" since
		// BIO_set_close() always returns 1.
		(void)BIO_set_close(bio, BIO_CLOSE);

		ret = PEM_write_bio_X509(bio, certificate);

		if (ret != 1)
		{
			ERROR_EX_LOG("[server_name = %s]PEM_write_bio_X509() failed", m_server_name.c_str());

			X509_free(certificate);
			BIO_free(bio);

			return false;
		}

		BUF_MEM* mem;

		BIO_get_mem_ptr(bio, &mem); // NOLINT[cppcoreguidelines-pro-type-cstyle-cast]

		if (!mem || !mem->data || mem->length == 0u)
		{
			ERROR_EX_LOG("[server_name = %s]BIO_get_mem_ptr() failed", m_server_name.c_str());

			X509_free(certificate);
			BIO_free(bio);

			return false;
		}
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		this->remoteCert = std::string(mem->data, mem->length);

		X509_free(certificate);
		BIO_free(bio);

		return true;
	}

	void dtlsv1x::show() const
	{
		std::string state{ "new" };
		std::string role{ "none " };
		switch (this->state)
		{
		case DtlsState::CONNECTING:
			state = "connecting";
			break;
		case DtlsState::CONNECTED:
			state = "connected";
			break;
		case DtlsState::FAILED:
			state = "failed";
			break;
		case DtlsState::CLOSED:
			state = "closed";
			break;
		default:;
		}

		switch (this->localRole)
		{
		case Role::AUTO:
			role = "auto";
			break;
		case Role::SERVER:
			role = "server";
			break;
		case Role::CLIENT:
			role = "client";
			break;
		default:;
		}

		DEBUG_EX_LOG("[server_name = %s]<dtlsv1x>", m_server_name.c_str());
		DEBUG_EX_LOG("  state           : %s", state.c_str());
		DEBUG_EX_LOG("  role            : %s", role.c_str());
		DEBUG_EX_LOG("  handshake done: : %s", this->handshakeDone ? "yes" : "no");
		DEBUG_EX_LOG("</dtlsv1x>");
	}

		void dtlsv1x::SetListener(Listener * ptr)
		{
			this->listener = ptr;
		}

		void dtlsv1x::startup()
		{
			m_thread = std::thread(&dtlsv1x::_work_thread, this);
		}

		void dtlsv1x::OnSslInfo(int where, int ret)
		{
			int w = where & -SSL_ST_MASK;
			const char* role;

			if ((w & SSL_ST_CONNECT) != 0)
				role = "client";
			else if ((w & SSL_ST_ACCEPT) != 0)
				role = "server";
			else
				role = "undefined";

			if ((where & SSL_CB_LOOP) != 0)
			{
				DEBUG_EX_LOG(  "[server_name = %s][role:%s, action:'%s']", m_server_name.c_str(), role, SSL_state_string_long(this->ssl));
			}
			else if ((where & SSL_CB_ALERT) != 0)
			{
				const char* alertType;

				switch (*SSL_alert_type_string(ret))
				{
				case 'W':
					alertType = "warning";
					break;

				case 'F':
					alertType = "fatal";
					break;

				default:
					alertType = "undefined";
				}

				if ((where & SSL_CB_READ) != 0)
				{
					WARNING_EX_LOG( "[server_name = %s]received DTLS %s alert: %s", m_server_name.c_str(), alertType, SSL_alert_desc_string_long(ret));
				}
				else if ((where & SSL_CB_WRITE) != 0)
				{
					DEBUG_EX_LOG(  "[server_name = %s]sending DTLS %s alert: %s", m_server_name.c_str(), alertType, SSL_alert_desc_string_long(ret));
				}
				else
				{
					DEBUG_EX_LOG(  "[server_name = %s]DTLS %s alert: %s", m_server_name.c_str(), alertType, SSL_alert_desc_string_long(ret));
				}
			}
			else if ((where & SSL_CB_EXIT) != 0)
			{
				if (ret == 0)
				{
					DEBUG_EX_LOG("[server_name = %s][role:%s, failed:'%s']", m_server_name.c_str(), role, SSL_state_string_long(this->ssl));
				}
				else if (ret < 0)
				{
					DEBUG_EX_LOG("[server_name = %s]role: %s, waiting:'%s']", m_server_name.c_str(), role, SSL_state_string_long(this->ssl));
				}
			}
			else if ((where & SSL_CB_HANDSHAKE_START) != 0)
			{
				DEBUG_EX_LOG(  "[server_name = %s]DTLS handshake start", m_server_name.c_str());
			}
			else if ((where & SSL_CB_HANDSHAKE_DONE) != 0)
			{
				DEBUG_EX_LOG( "[server_name = %s]DTLS handshake done", m_server_name.c_str());

				this->handshakeDoneNow = true;
			}

			// NOTE: checking SSL_get_shutdown(this->ssl) & SSL_RECEIVED_SHUTDOWN here upon
			// receipt of a close alert does not work (the flag is set after this callback).
		}

		

	inline static unsigned int onSslDtlsTimer(SSL* /*ssl*/, unsigned int timerUs)
	{
		if (timerUs == 0)
		{
			return 100000;
		}
		else if (timerUs >= 4000000)
		{
			return 4000000;
		}
		//else
		return 2 * timerUs;
	}

	dtlsv1x::dtlsv1x(const char * server_name/*Listener *listener*/)
		: listener(NULL)
		, ssl(NULL)
		, sslBioFromNetwork(NULL)
		, sslBioToNetwork(NULL)
		, state(DtlsState::NEW)
		, localRole(Role::NONE)
		, remoteFingerprint()
		, handshakeDone(false)
		, handshakeDoneNow(false)
		, remoteCert()
		, m_server_name(server_name)
	{
		/* Set SSL. */

		this->ssl = SSL_new( sslCtx);

		if (!this->ssl)
		{
			ERROR_EX_LOG("[server_name = %s]SSL_new() failed", m_server_name.c_str());

			goto error;
		}

		// Set this as custom data.
		SSL_set_ex_data(this->ssl, 0, static_cast<void*>(this));

		this->sslBioFromNetwork = BIO_new(BIO_s_mem());

		if (!this->sslBioFromNetwork)
		{
			ERROR_EX_LOG("[server_name = %s]BIO_new() failed", m_server_name.c_str());

			SSL_free(this->ssl);

			goto error;
		}

		this->sslBioToNetwork = BIO_new(BIO_s_mem());

		if (!this->sslBioToNetwork)
		{
			ERROR_EX_LOG("[server_name = %s]BIO_new() failed", m_server_name.c_str());

			BIO_free(this->sslBioFromNetwork);
			SSL_free(this->ssl);

			goto error;
		}

		SSL_set_bio(this->ssl, this->sslBioFromNetwork, this->sslBioToNetwork);

		// Set the MTU so that we don't send packets that are too large with no fragmentation.
		SSL_set_mtu(this->ssl, DtlsMtu);
		DTLS_set_link_mtu(this->ssl, DtlsMtu);

		// Set callback handler for setting DTLS timer interval.
		//DTLS_set_timer_cb(this->ssl, onSslDtlsTimer);

		// Set the DTLS timer.
		//this->timer = new Timer(this);

		return;

	error:

		// NOTE: At this point SSL_set_bio() was not called so we must free BIOs as
		// well.
		if (this->sslBioFromNetwork)
		{
			BIO_free(this->sslBioFromNetwork);
		}

		if (this->sslBioToNetwork)
		{
			BIO_free(this->sslBioToNetwork);
		}

		if (this->ssl)
		{
			SSL_free(this->ssl);
		}

		// NOTE: If this is not catched by the caller the program will abort, but
		// this should never happen.
		ERROR_EX_LOG("[server_name = %s]DtlsTransport instance creation failed", m_server_name.c_str());
	}
	bool dtlsv1x::IsRunning() const
	{
		switch (this->state)
		{
			case DtlsState::NEW:
			{
				return false;
			}
			case DtlsState::CONNECTING:
			case DtlsState::CONNECTED:
			{
				return true;
			}
			case DtlsState::FAILED:
			case DtlsState::CLOSED:
			{
				return false;
			}
		}

		// Make GCC 4.9 happy.
		return false;
	}

	void dtlsv1x::SendPendingOutgoingDtlsData()
	{
		if (BIO_eof(this->sslBioToNetwork))
		{
			DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
			return;
		}

		int64_t read;
		char* data{ nullptr };

		read = BIO_get_mem_data(this->sslBioToNetwork, &data); // NOLINT

		if (read <= 0)
		{
			WARNING_EX_LOG("[server_name = %s]", m_server_name.c_str());
			return;
		}

		DEBUG_EX_LOG("[server_name = %s]%u bytes of DTLS data ready to sent to the peer", m_server_name.c_str(), read);

		// Notify the listener.
		this->listener->ProcessDtlsData(  reinterpret_cast<uint8_t*>(data), static_cast<size_t>(read));

		// Clear the BIO buffer.
		// NOTE: the (void) avoids the -Wunused-value warning.
		(void)BIO_reset(this->sslBioToNetwork);
	}

	void dtlsv1x::_work_thread()
	{
		while (true)
		{
			if (m_quene.empty())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			else
			{
				while (!m_quene.empty())
				{
					DEBUG_EX_LOG("quene size = %u", m_quene.size());
					dtsl_data temp_data;
					{
						std::lock_guard<std::mutex> locak(m_mutex);
						temp_data = m_quene.front();
						m_quene.pop_front();
					}
					std::this_thread::sleep_for(std::chrono::seconds(1));
					_process_data(temp_data.data, temp_data.size);
					delete[] temp_data.data;
					temp_data.data = NULL;
					temp_data.size = 0;
				}
			}
		}
	}

	void dtlsv1x::_process_data(const uint8_t * data, size_t len)
	{
		int written;
		int read;

		if (!IsRunning())
		{
			ERROR_EX_LOG("[server_name = %s]cannot process data while not running", m_server_name.c_str());

			return;
		}
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		// Write the received DTLS data into the sslBioFromNetwork.
		written =
			BIO_write(this->sslBioFromNetwork, static_cast<const void*>(data ), static_cast<int>(len));
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		if (written != static_cast<int>(len))
		{
			WARNING_EX_LOG(
				"[server_name = %s]OpenSSL BIO_write() wrote less (%zu bytes) than given data (%zu bytes)", m_server_name.c_str(),
				static_cast<size_t>(written),
				len);
		}
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		// Must call SSL_read() to process received DTLS data.
		read = SSL_read(this->ssl, static_cast<void*>(sslReadBuffer), SslReadBufferSize);
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		// Send data if it's ready.
		SendPendingOutgoingDtlsData();
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		// Check SSL status and return if it is bad/closed.
		if (!CheckStatus(read))
		{
			return;
		}
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
		// Set/update the DTLS timeout.
	/*	if (!SetTimeout())
			return;*/

			// Application data received. Notify to the listener.
		if (read > 0)
		{
			// It is allowed to receive DTLS data even before validating remote fingerprint.
			if (!this->handshakeDone)
			{
				WARNING_EX_LOG("[server_name = %s]ignoring application data received while DTLS handshake not done", m_server_name.c_str());

				return;
			}
			DEBUG_EX_LOG("[server_name = %s]SctpAssociation", m_server_name.c_str());
			// Notify the listener.
			/*this->listener->OnDtlsTransportApplicationDataReceived(
				this, (uint8_t*) sslReadBuffer, static_cast<size_t>(read));*/
		}
		DEBUG_EX_LOG("[server_name = %s]", m_server_name.c_str());
	}


	/*dtlsv1x::~dtlsv1x()
	{
	}*/
}


///////////////////////////////////

/***********************************************************************************************
created: 		2022-05-22

author:			chensong

purpose:		TLSv1.3 协议的学习

原因是WebRTC中有DTSTransport --> 
************************************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <iostream>
#include <mutex>
#include <openssl/asn1.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

#include <vector>
#include <map>
#include <string>
#include "clog.h"
#include "dtlsv1.x.h"
/* Static. */

static std::once_flag globalInitOnce;

int main(int argc, char *argv[])
{
	std::call_once(globalInitOnce, [] {
		DEBUG_EX_LOG( "openssl version: %s" ,  OpenSSL_version(OPENSSL_VERSION));
		// Initialize some crypto stuff.
		RAND_poll();
	});
	
	/////////////
	// global ssl
	////////////
	//chen::dtlsv1x::ReadCertificateAndPrivateKeyFromFiles();
	chen::dtlsv1x::GenerateCertificateAndPrivateKey();

	// Create a global SSL_CTX.
	chen::dtlsv1x::CreateSslCtx();

	// Generate certificate fingerprints.
	chen::dtlsv1x::GenerateFingerprints();

	 


	chen::dtlsv1x client("client");
	chen::dtlsv1x server("server");
	client.SetListener(&server);
	server.SetListener(&client);

	client.startup();
	server.startup();
	//server 准备handhasb
	server.Run(chen::Role::SERVER);

	client.Run(chen::Role::CLIENT);

	while (true)
	{
		DEBUG_EX_LOG("main sleep 1 seconds ...");
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return EXIT_SUCCESS;
}
```


运行效果图:

![在这里插入图片描述](https://img-blog.csdnimg.cn/15bfae63eccd4eca895b0fff2c1fdb84.png)



# 总结

[代码地址：https://github.com/chensongpoixs/crtc_doc/tree/master/TLSv1.3/TLSv1.3_demo](https://github.com/chensongpoixs/crtc_doc/tree/master/TLSv1.3/TLSv1.3_demo)

