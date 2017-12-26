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

//TODO:

/*******************************************************************************************/

#endif
