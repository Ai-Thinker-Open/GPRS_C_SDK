#ifndef __API_SOCKET__H_
#define __API_SOCKET__H_

#include <sdk_init.h>

/*****************************************************************************************/
///////////////////////////////////////not standard////////////////////////////////////////
/*
#define MAX_SOCKET_NUM 20
#define MAX_DNS_DOMAIN_LENGTH

typedef enum{
    TCP = 0,
    UDP =1
}TCP_UDP_t;

typedef enum{
    DNS_STATUS_ERROR = -1,
    DNS_STATUS_OK    =  0,
    DNS_STATUS_WAIT  =  1,
    DNS_STATUS_MAX
}DNS_Status_t;

*/

// int      Socket_TcpipConnect(TCP_UDP_t tcpOrUdp, const char* ip,uint16_t port);
// int      Socket_TcpipWrite(int socketFd, uint8_t* data, uint16_t length);
// int      Socket_TcpipRead( int socketFd, uint8_t* data, uint16_t length);
// bool     Socket_TcpipClose(int socketFd);
// DNS_Status_t     DNS_GetHostByName(uint8_t* domain, uint8_t* ip);


/// \brief  int      Socket_TcpipConnect(TCP_UDP_t tcpOrUdp, const char* ip,uint16_t port);
/// \param  TCP_UDP_t tcpOrUdp , socket protocol TCP or UDP
/// \param  const char* ip  , the ip addr socket connect to 
/// \param  uint16_t port , the port socket connect to 
/// \return int , return the socket id
#define Socket_TcpipConnect                        CSDK_FUNC(Socket_TcpipConnect)

/// \brief  int      Socket_TcpipWrite(int socketFd, uint8_t* data, uint16_t length);
/// \param  int socketFd , the connected socket id you want to sent to 
/// \param  uint8_t* data  , the data you want to sent  
/// \param  uint16_t length , the length of data 
/// \return int , return the operate code
#define Socket_TcpipWrite                          CSDK_FUNC(Socket_TcpipWrite)

/// \brief  int      Socket_TcpipRead(int socketFd, uint8_t* data, uint16_t length);
/// \param  int socketFd , the connected socket id you want to get data from
/// \param  uint8_t* data  , the data buf 
/// \param  uint16_t length , the max length of data 
/// \return int , return read the data length
#define Socket_TcpipRead                           CSDK_FUNC(Socket_TcpipRead)

/// \brief  bool     Socket_TcpipClose(int socketFd);
/// \param  int socketFd , the connected socket id you want to close
/// \return bool , 
#define Socket_TcpipClose                          CSDK_FUNC(Socket_TcpipClose)

/// \brief  DNS_Status_t     DNS_GetHostByName(uint8_t* domain, uint8_t* ip);
/// \param  uint8_t* domain , the dns domain 
/// \param  uint8_t* ip , return ip if dns ok
/// \return DNS_Status_t , the status of dns
#define DNS_GetHostByName                          CSDK_FUNC(DNS_GetHostByName)

#define DNS_GetHostByNameEX                        CSDK_FUNC(DNS_GetHostByNameEX)
/**
 * 
 * 
 * @return 0: success !=0:fail
 */
#define DNS_GetHostByName2                         CSDK_FUNC(DNS_GetHostByName2)


/*******************************************************************************************/


/*******************************************************************************************/
///////////////////////////////////////socket standard///////////////////////////////////////

/*
int                 (*accept)(int s, struct sockaddr *addr, socklen_t *addrlen);
int                 (*bind)(int s, const struct sockaddr *name, socklen_t namelen);
int                 (*shutdown)(int s, int how);
int                 (*getpeername) (int s, struct sockaddr *name, socklen_t *namelen);
int                 (*getsockname) (int s, struct sockaddr *name, socklen_t *namelen);
int                 (*getsockopt) (int s, int level, int optname, void *optval, socklen_t *optlen);
int                 (*setsockopt) (int s, int level, int optname, const void *optval, socklen_t optlen);
int                 (*close)(int s);
int                 (*connect)(int s, const struct sockaddr *name, socklen_t namelen);
int                 (*listen)(int s, int backlog);
ssize_t             (*recv)(int s, void *mem, size_t len, int flags);
ssize_t             (*read)(int s, void *mem, size_t len);
ssize_t             (*recvfrom)(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
ssize_t             (*recvmsg)(int s, struct msghdr *message, int flags);
ssize_t             (*send)(int s, const void *dataptr, size_t size, int flags);
ssize_t             (*sendmsg)(int s, const struct msghdr *message, int flags);
ssize_t             (*sendto)(int s, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
int                 (*socket)(int domain, int type, int protocol);
ssize_t             (*write)(int s, const void *dataptr, size_t size);
ssize_t             (*writev)(int s, const struct iovec *iov, int iovcnt);
int                 (*select)(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,struct timeval *timeout);
int                 (*ioctl)(int s, long cmd, void *argp);
int                 (*fcntl)(int s, int cmd, int val);
char*               (*ip6addr_ntoa_r)(const ip6_addr_t *addr, char *buf, int buflen);
char*               (*ip4addr_ntoa_r)(const ip4_addr_t *addr, char *buf, int buflen);
int                 (*ip6addr_aton)(const char *cp, ip6_addr_t *addr);
int                 (*ip4addr_aton)(const char *cp, ip4_addr_t *addr);
u16_t               (*htons)(u16_t x);
u32_t               (*htonl)(u32_t x);
const char*         (*strerr)(err_t err);
int                 (*err_to_errno)(err_t err);
*/


#define accept               CSDK_FUNC(lwip_accept)
#define bind                 CSDK_FUNC(lwip_bind)
#define shutdown             CSDK_FUNC(lwip_shutdown)
#define getpeername          CSDK_FUNC(lwip_getpeername)
#define getsockname          CSDK_FUNC(lwip_getsockname)
#define getsockopt           CSDK_FUNC(lwip_getsockopt)
#define setsockopt           CSDK_FUNC(lwip_setsockopt)
#define close                CSDK_FUNC(lwip_close)
#define connect              CSDK_FUNC(lwip_connect)
#define listen               CSDK_FUNC(lwip_listen)
#define recv                 CSDK_FUNC(lwip_recv)
#define read                 CSDK_FUNC(lwip_read)
#define recvfrom             CSDK_FUNC(lwip_recvfrom)
#define recvmsg              CSDK_FUNC(lwip_recvmsg)
#define send                 CSDK_FUNC(lwip_send)
#define sendmsg              CSDK_FUNC(lwip_sendmsg)
#define sendto               CSDK_FUNC(lwip_sendto)
#define socket               CSDK_FUNC(lwip_socket)
#define write                CSDK_FUNC(lwip_write)
#define writev               CSDK_FUNC(lwip_writev)
#define select               CSDK_FUNC(lwip_select)
#define ioctl                CSDK_FUNC(lwip_ioctl)
#define fcntl                CSDK_FUNC(lwip_fcntl)
#define ip6addr_ntoa_r       CSDK_FUNC(ip6addr_ntoa_r)
#define ip4addr_ntoa_r       CSDK_FUNC(ip4addr_ntoa_r)
#define ip6addr_ntoa         CSDK_FUNC(ip6addr_ntoa_r)
#define ip4addr_ntoa         CSDK_FUNC(ip4addr_ntoa_r)
#define ip6addr_aton         CSDK_FUNC(ip6addr_aton)
#define ip4addr_aton         CSDK_FUNC(ip4addr_aton)
#define htons                CSDK_FUNC(lwip_htons)
#define htonl                CSDK_FUNC(lwip_htonl)
#define strerr               CSDK_FUNC(lwip_strerr)

/* directly map this to the lwip internal functions */
#define inet_addr(cp)                   ipaddr_addr(cp)
#define inet_aton(cp, addr)             ip4addr_aton(cp, (ip4_addr_t*)addr)
#define inet_ntoa(addr)                 ip4addr_ntoa((const ip4_addr_t*)&(addr))
#define inet_ntoa_r(addr, buf, buflen)  ip4addr_ntoa_r((const ip4_addr_t*)&(addr), buf, buflen)

/* directly map this to the lwip internal functions */
#define inet6_aton(cp, addr)            ip6addr_aton(cp, (ip6_addr_t*)addr)
#define inet6_ntoa(addr)                ip6addr_ntoa((const ip6_addr_t*)&(addr))
#define inet6_ntoa_r(addr, buf, buflen) ip6addr_ntoa_r((const ip6_addr_t*)&(addr), buf, buflen)

#define inet_ntop(af,src,dst,size) \
    (((af) == AF_INET6) ? ip6addr_ntoa_r((const ip6_addr_t*)(src),(dst),(size)) \
     : (((af) == AF_INET) ? ip4addr_ntoa_r((const ip4_addr_t*)(src),(dst),(size)) : NULL))
#define inet_pton(af,src,dst) \
    (((af) == AF_INET6) ? ip6addr_aton((src),(ip6_addr_t*)(dst)) \
     : (((af) == AF_INET) ? ip4addr_aton((src),(ip4_addr_t*)(dst)) : 0))

#define ntohl(x) PP_NTOHL(x)


/*******************************************************************************************/

#endif
