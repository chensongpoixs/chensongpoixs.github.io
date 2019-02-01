#include "csocket_ops.h"


namespace chen
{
	
	socket_type socket(int af, int type, int protocol, int32& ec)
	{
		socket_type s = error_wrapper(::socket(af, type, protocol), ec);
		if (s == invalid_socket)
		{
			ec 
			return s;
		}
    

		int32 optval = 1;
		int32 result = ::setsockopt(s, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)), ec); // 对端 关闭close socket , 第二次调用write会报 信号SIGPIPE 程序崩溃  Broken pipe向一个没有读端的管道写数据 , 设置SO_NOSIGPIPE 就忽略信号 会返回错误码了
		if (result != 0) // RST 
		{
			::close(s);
			return invalid_socket;
		}

		return s;
	}
	
	int32 setsockopt(socket_type s, state_type& state, int32 level, int32 optname,
    const void* optval, std::size_t optlen, int32& ec)
	{
	  if (s == invalid_socket)
	  {
		ec = boost::asio::error::bad_descriptor;
		return socket_error_retval;
	  }

	  if (level == custom_socket_option_level && optname == always_fail_option)
	  {
		ec = boost::asio::error::invalid_argument;
		return socket_error_retval;
	  }

	  if (level == custom_socket_option_level
		  && optname == enable_connection_aborted_option)
	  {
		if (optlen != sizeof(int))
		{
		  ec = boost::asio::error::invalid_argument;
		  return socket_error_retval;
		}

		if (*static_cast<const int*>(optval))
		  state |= enable_connection_aborted;
		else
		  state &= ~enable_connection_aborted;
		ec = boost::system::error_code();
		return 0;
	  }

	  if (level == SOL_SOCKET && optname == SO_LINGER)
		state |= user_set_linger;

#if defined(_MSC_VER)

	  // Mysteriously, using the getsockopt and setsockopt functions directly with
	  // Borland C++ results in incorrect values being set and read. The bug can be
	  // worked around by using function addresses resolved with GetProcAddress.
	  if (HMODULE winsock_module = ::GetModuleHandleA("ws2_32"))
	  {
		typedef int (WSAAPI *sso_t)(SOCKET, int, int, const char*, int);
		if (sso_t sso = (sso_t)::GetProcAddress(winsock_module, "setsockopt"))
		{
		  clear_last_error();
		  return error_wrapper(sso(s, level, optname,
				reinterpret_cast<const char*>(optval),
				static_cast<int>(optlen)), ec);
		}
	  }
	  ec = boost::asio::error::fault;
	  return socket_error_retval;
	
#elif defined(__GNUC__)
	  clear_last_error();
	  int result = error_wrapper(call_setsockopt(&msghdr::msg_namelen,
			s, level, optname, optval, optlen), ec);
	  if (result == 0)
	  {
		ec = boost::system::error_code();

	#if defined(__MACH__) && defined(__APPLE__) \
	  || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__)
		// To implement portable behaviour for SO_REUSEADDR with UDP sockets we
		// need to also set SO_REUSEPORT on BSD-based platforms.
		if ((state & datagram_oriented)
			&& level == SOL_SOCKET && optname == SO_REUSEADDR)
		{
		  call_setsockopt(&msghdr::msg_namelen, s,
			  SOL_SOCKET, SO_REUSEPORT, optval, optlen);
		}
	#endif
	  }

	  return result;
	#endif // defined(__BORLANDC__)
	}
	
	
}
