#ifndef _C_SOCKET_TYPES_H_
#define _C_SOCKET_TYPES_H_
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <new>
#include <codecvt>
#include <locale>
#include <string>

#if defined(_MSC_VER)

	// win 
# include <winsock2.h>
# include <ws2tcpip.h>
#   pragma comment(lib, "ws2.lib")
#   pragma comment(lib, "ws2_32.lib")
#    pragma comment(lib, "mswsock.lib")

#elif defined(__GNUC__)

	// unix
	
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#  include <sys/time.h>
#  include <sys/select.h>
# include <sys/socket.h>
# include <sys/uio.h>
# include <sys/un.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <net/if.h>
# include <limits.h>
#else
#pragma error "unknow platform!!!"

#endif
namespace chen 
{
	typedef signed char		int8;
	typedef unsigned char	uint8;

	typedef signed short	int16;
	typedef unsigned short	uint16;

	typedef signed int		int32;
	typedef unsigned int	uint32;

#if defined(_MSC_VER)

	typedef signed long long	int64;
	typedef unsigned long long	uint64;

#elif defined(__GNUC__)

	typedef signed long		int64;
	typedef unsigned long	uint64;
#else
#pragma error "unknow platform!!!"

#endif
	
	
	
}

namespace chen
{
#if defined(_MSC_VER)

typedef SOCKET socket_type;
const SOCKET invalid_socket = INVALID_SOCKET;
const int32 socket_error_retval = SOCKET_ERROR;
const int32 max_addr_v4_str_len = 256;
const int32 max_addr_v6_str_len = 256;
typedef sockaddr socket_addr_type;
typedef in_addr in4_addr_type;
typedef ip_mreq in4_mreq_type;
typedef sockaddr_in sockaddr_in4_type;

typedef ::linger linger_type;
typedef unsigned long ioctl_arg_type;
typedef u_long u_long_type;
typedef u_short u_short_type;
typedef int32 signed_size_type;


// win socket选项
# define CHEN_OS_DEF(c) CHNE_OS_DEF_##c
# define CHEN_OS_DEF_AF_UNSPEC AF_UNSPEC
# define CHEN_OS_DEF_AF_INET AF_INET
# define CHEN_OS_DEF_AF_INET6 AF_INET6
# define CHEN_OS_DEF_SOCK_STREAM SOCK_STREAM
# define CHEN_OS_DEF_SOCK_DGRAM SOCK_DGRAM
# define CHEN_OS_DEF_SOCK_RAW SOCK_RAW
# define CHEN_OS_DEF_SOCK_SEQPACKET SOCK_SEQPACKET
# define CHEN_OS_DEF_IPPROTO_IP IPPROTO_IP
# define CHEN_OS_DEF_IPPROTO_IPV6 IPPROTO_IPV6
# define CHEN_OS_DEF_IPPROTO_TCP IPPROTO_TCP
# define CHEN_OS_DEF_IPPROTO_UDP IPPROTO_UDP
# define CHEN_OS_DEF_IPPROTO_ICMP IPPROTO_ICMP
# define CHEN_OS_DEF_IPPROTO_ICMPV6 IPPROTO_ICMPV6
# define CHEN_OS_DEF_FIONBIO FIONBIO
# define CHEN_OS_DEF_FIONREAD FIONREAD
# define CHEN_OS_DEF_INADDR_ANY INADDR_ANY
# define CHEN_OS_DEF_MSG_OOB MSG_OOB
# define CHEN_OS_DEF_MSG_PEEK MSG_PEEK
# define CHEN_OS_DEF_MSG_DONTROUTE MSG_DONTROUTE
# define CHEN_OS_DEF_MSG_EOR 0 // Not supported on Windows.
# define CHEN_OS_DEF_SHUT_RD SD_RECEIVE
# define CHEN_OS_DEF_SHUT_WR SD_SEND
# define CHEN_OS_DEF_SHUT_RDWR SD_BOTH
# define CHEN_OS_DEF_SOMAXCONN SOMAXCONN
# define CHEN_OS_DEF_SOL_SOCKET SOL_SOCKET
# define CHEN_OS_DEF_SO_BROADCAST SO_BROADCAST
# define CHEN_OS_DEF_SO_DEBUG SO_DEBUG
# define CHEN_OS_DEF_SO_DONTROUTE SO_DONTROUTE
# define CHEN_OS_DEF_SO_KEEPALIVE SO_KEEPALIVE
# define CHEN_OS_DEF_SO_LINGER SO_LINGER
# define CHEN_OS_DEF_SO_SNDBUF SO_SNDBUF
# define CHEN_OS_DEF_SO_RCVBUF SO_RCVBUF
# define CHEN_OS_DEF_SO_SNDLOWAT SO_SNDLOWAT
# define CHEN_OS_DEF_SO_RCVLOWAT SO_RCVLOWAT
# define CHEN_OS_DEF_SO_REUSEADDR SO_REUSEADDR
# define CHEN_OS_DEF_TCP_NODELAY TCP_NODELAY
# define CHEN_OS_DEF_IP_MULTICAST_IF IP_MULTICAST_IF
# define CHEN_OS_DEF_IP_MULTICAST_TTL IP_MULTICAST_TTL
# define CHEN_OS_DEF_IP_MULTICAST_LOOP IP_MULTICAST_LOOP
# define CHEN_OS_DEF_IP_ADD_MEMBERSHIP IP_ADD_MEMBERSHIP
# define CHEN_OS_DEF_IP_DROP_MEMBERSHIP IP_DROP_MEMBERSHIP
# define CHEN_OS_DEF_IP_TTL IP_TTL
# define CHEN_OS_DEF_IPV6_UNICAST_HOPS IPV6_UNICAST_HOPS
# define CHEN_OS_DEF_IPV6_MULTICAST_IF IPV6_MULTICAST_IF
# define CHEN_OS_DEF_IPV6_MULTICAST_HOPS IPV6_MULTICAST_HOPS
# define CHEN_OS_DEF_IPV6_MULTICAST_LOOP IPV6_MULTICAST_LOOP
# define CHEN_OS_DEF_IPV6_JOIN_GROUP IPV6_JOIN_GROUP
# define CHEN_OS_DEF_IPV6_LEAVE_GROUP IPV6_LEAVE_GROUP
# define CHEN_OS_DEF_AI_CANONNAME AI_CANONNAME
# define CHEN_OS_DEF_AI_PASSIVE AI_PASSIVE
# define CHEN_OS_DEF_AI_NUMERICHOST AI_NUMERICHOST
# if defined(AI_NUMERICSERV)
#  define CHEN_OS_DEF_AI_NUMERICSERV AI_NUMERICSERV
# else
#  define CHEN_OS_DEF_AI_NUMERICSERV 0
# endif
# if defined(AI_V4MAPPED)
#  define CHEN_OS_DEF_AI_V4MAPPED AI_V4MAPPED
# else
#  define CHEN_OS_DEF_AI_V4MAPPED 0
# endif
# if defined(AI_ALL)
#  define CHEN_OS_DEF_AI_ALL AI_ALL
# else
#  define CHEN_OS_DEF_AI_ALL 0
# endif
# if defined(AI_ADDRCONFIG)
#  define CHEN_OS_DEF_AI_ADDRCONFIG AI_ADDRCONFIG
# else
#  define CHEN_OS_DEF_AI_ADDRCONFIG 0
# endif
# if defined (_WIN32_WINNT)
const int32 max_iov_len = 64;
# else
const int32 max_iov_len = 16;
# endif

#elif defined(__GNUC__)

typedef int32 socket_type;
const int32 invalid_socket = -1;
const int32 socket_error_retval = -1;
const int32 max_addr_v4_str_len = INET_ADDRSTRLEN;
#if defined(INET6_ADDRSTRLEN)
const int32 max_addr_v6_str_len = INET6_ADDRSTRLEN + 1 + IF_NAMESIZE;
#else // defined(INET6_ADDRSTRLEN)
const int32 max_addr_v6_str_len = 256;
#endif // defined(INET6_ADDRSTRLEN)
typedef sockaddr socket_addr_type;
typedef in_addr in4_addr_type;
// 
typedef sockaddr_in sockaddr_in4_type;
typedef in6_addr in6_addr_type;
typedef ipv6_mreq in6_mreq_type;
typedef sockaddr_in6 sockaddr_in6_type;
typedef sockaddr_storage sockaddr_storage_type;
typedef sockaddr_un sockaddr_un_type;
typedef addrinfo addrinfo_type;
// unix
typedef ::linger linger_type;
typedef int32 ioctl_arg_type;
typedef uint32 u_long_type;
typedef uint16 u_short_type;

// unix socket选项
# define CHNE_OS_DEF(c) BOOST_ASIO_OS_DEF_##c
# define CHNE_OS_DEF_AF_UNSPEC AF_UNSPEC
# define CHNE_OS_DEF_AF_INET AF_INET
# define CHNE_OS_DEF_AF_INET6 AF_INET6
# define CHNE_OS_DEF_SOCK_STREAM SOCK_STREAM
# define CHNE_OS_DEF_SOCK_DGRAM SOCK_DGRAM
# define CHNE_OS_DEF_SOCK_RAW SOCK_RAW
# define CHNE_OS_DEF_SOCK_SEQPACKET SOCK_SEQPACKET
# define CHNE_OS_DEF_IPPROTO_IP IPPROTO_IP
# define CHNE_OS_DEF_IPPROTO_IPV6 IPPROTO_IPV6
# define CHNE_OS_DEF_IPPROTO_TCP IPPROTO_TCP
# define CHNE_OS_DEF_IPPROTO_UDP IPPROTO_UDP
# define CHNE_OS_DEF_IPPROTO_ICMP IPPROTO_ICMP
# define CHNE_OS_DEF_IPPROTO_ICMPV6 IPPROTO_ICMPV6
# define CHNE_OS_DEF_FIONBIO FIONBIO
# define CHNE_OS_DEF_FIONREAD FIONREAD
# define CHNE_OS_DEF_INADDR_ANY INADDR_ANY
# define CHNE_OS_DEF_MSG_OOB MSG_OOB
# define CHNE_OS_DEF_MSG_PEEK MSG_PEEK
# define CHNE_OS_DEF_MSG_DONTROUTE MSG_DONTROUTE
# define CHNE_OS_DEF_MSG_EOR MSG_EOR
# define CHNE_OS_DEF_SHUT_RD SHUT_RD
# define CHNE_OS_DEF_SHUT_WR SHUT_WR
# define CHNE_OS_DEF_SHUT_RDWR SHUT_RDWR
# define CHNE_OS_DEF_SOMAXCONN SOMAXCONN
# define CHNE_OS_DEF_SOL_SOCKET SOL_SOCKET
# define CHNE_OS_DEF_SO_BROADCAST SO_BROADCAST
# define CHNE_OS_DEF_SO_DEBUG SO_DEBUG
# define CHNE_OS_DEF_SO_DONTROUTE SO_DONTROUTE
# define CHNE_OS_DEF_SO_KEEPALIVE SO_KEEPALIVE
# define CHNE_OS_DEF_SO_LINGER SO_LINGER
# define CHNE_OS_DEF_SO_SNDBUF SO_SNDBUF
# define CHNE_OS_DEF_SO_RCVBUF SO_RCVBUF
# define CHNE_OS_DEF_SO_SNDLOWAT SO_SNDLOWAT
# define CHNE_OS_DEF_SO_RCVLOWAT SO_RCVLOWAT
# define CHNE_OS_DEF_SO_REUSEADDR SO_REUSEADDR
# define CHNE_OS_DEF_TCP_NODELAY TCP_NODELAY
# define CHNE_OS_DEF_IP_MULTICAST_IF IP_MULTICAST_IF
# define CHNE_OS_DEF_IP_MULTICAST_TTL IP_MULTICAST_TTL
# define CHNE_OS_DEF_IP_MULTICAST_LOOP IP_MULTICAST_LOOP
# define CHNE_OS_DEF_IP_ADD_MEMBERSHIP IP_ADD_MEMBERSHIP
# define CHNE_OS_DEF_IP_DROP_MEMBERSHIP IP_DROP_MEMBERSHIP
# define CHNE_OS_DEF_IP_TTL IP_TTL
# define CHNE_OS_DEF_IPV6_UNICAST_HOPS IPV6_UNICAST_HOPS
# define CHNE_OS_DEF_IPV6_MULTICAST_IF IPV6_MULTICAST_IF
# define CHNE_OS_DEF_IPV6_MULTICAST_HOPS IPV6_MULTICAST_HOPS
# define CHNE_OS_DEF_IPV6_MULTICAST_LOOP IPV6_MULTICAST_LOOP
# define CHNE_OS_DEF_IPV6_JOIN_GROUP IPV6_JOIN_GROUP
# define CHNE_OS_DEF_IPV6_LEAVE_GROUP IPV6_LEAVE_GROUP
# define CHNE_OS_DEF_AI_CANONNAME AI_CANONNAME
# define CHNE_OS_DEF_AI_PASSIVE AI_PASSIVE
# define CHNE_OS_DEF_AI_NUMERICHOST AI_NUMERICHOST
# if defined(AI_NUMERICSERV)
#  define CHEN_OS_DEF_AI_NUMERICSERV AI_NUMERICSERV
# else
#  define CHNE_OS_DEF_AI_NUMERICSERV 0
# endif
#else
#pragma error "unknow platform!!!"
#endif
const int custom_socket_option_level = 0xA5100000;
const int enable_connection_aborted_option = 1;
const int always_fail_option = 2;	
}

#endif // _C_SOCKET_TYPES_H_