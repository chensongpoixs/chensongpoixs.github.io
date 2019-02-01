#ifndef _C_SOCKET_OPS_H_
#define _C_SOCKET_OPS_H_

#include "csocket_types.h"
/***********************************************************************************************
	created: 		2019-01-31
	
	author:			chensong
					
	purpose:		unix IPv4 and IPv6
	
	later tasks:	add win socket api
************************************************************************************************/
namespace chen
{
	
	namespace socket_ops
	{
		
		// Socket state bits.
		enum
		{
		  // The user wants a non-blocking socket.
		  user_set_non_blocking = 1,

		  // The socket has been set non-blocking.
		  internal_non_blocking = 2,

		  // Helper "state" used to determine whether the socket is non-blocking.
		  non_blocking = user_set_non_blocking | internal_non_blocking,

		  // User wants connection_aborted errors, which are disabled by default.
		  // 用户希望连接中断错误，默认情况下禁用该错误。
		  enable_connection_aborted = 4,

		  // The user set the linger option. Needs to be checked when closing.
		  // 用户设置了逗留选项。关闭时需要检查。
		  user_set_linger = 8,

		  // The socket is stream-oriented.
		  stream_oriented = 16,

		  // The socket is datagram-oriented.
		  datagram_oriented = 32,

		  // The socket may have been dup()-ed.
		  possible_dup = 64
		};
#if defined(_MSC_VER)


#elif defined(__GNUC__)


		// create socket 
		socket_type socket(int af, int type, int protocol, int32& ec);  // return > 0 ok , return <= 0 err

		int32 setsockopt(socket_type s, state_type& state, int32 level, int32 optname,
    const void* optval, std::size_t optlen, int32& ec);
#else
#pragma error "unknow platform!!!"
#endif
	}// namespace socket_ops
}// namespace chen


#endif // _C_SOCKET_OPS_H_