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


#endif
