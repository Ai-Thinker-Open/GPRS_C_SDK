#ifndef __API_INC_SOCKET_H__
#define __API_INC_SOCKET_H__

#include <cs_types.h>

/*#################      socket      #################################*/
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


typedef enum{
    /** No error, everything OK. */
    API_SOCKET_ERROR_NONE = 0,
    /** Out of memory error.     */
    API_SOCKET_ERROR_MEM        = -1,
    /** Buffer error.            */
    API_SOCKET_ERROR_BUF        = -2,
    /** Timeout.                 */
    API_SOCKET_ERROR_TIMEOUT    = -3,
    /** Routing problem.         */
    API_SOCKET_ERROR_RTE        = -4,
    /** Operation in progress    */
    API_SOCKET_ERROR_INPROGRESS = -5,
    /** Illegal value.           */
    API_SOCKET_ERROR_VAL        = -6,
    /** Operation would block.   */
    API_SOCKET_ERROR_WOULDBLOCK = -7,
    /** Address in use.          */
    API_SOCKET_ERROR_USE        = -8,
    /** Already connecting.      */
    API_SOCKET_ERROR_ALREADY    = -9,
    /** Conn already established.*/
    API_SOCKET_ERROR_ISCONN     = -10,
    /** Not connected.           */
    API_SOCKET_ERROR_CONN       = -11,
    /** Low-level netif error    */
    API_SOCKET_ERROR_IF         = -12,

    /** Connection aborted.      */
    API_SOCKET_ERROR_ABRT       = -13,
    /** Connection reset.        */
    API_SOCKET_ERROR_RST        = -14,
    /** Connection closed.       */
    API_SOCKET_ERROR_CLSD       = -15,
    /** Illegal argument.        */
    API_SOCKET_ERROR_ARG        = -16
}API_Socket_Error_t;

typedef VOID (*DNS_CALLBACK_FUNC_T)(void *param);

#endif
