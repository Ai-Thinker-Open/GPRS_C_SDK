#ifndef __API_SSL_H
#define __API_SSL_H

#ifdef __cplusplus
extern "C"{
#endif

#include "sdk_init.h"


// SSL_Error_t SSL_Init(SSL_Config_t* sslConfig);
// SSL_Error_t SSL_Connect(SSL_Config_t* sslConfig, const char* server, const char* port);
// int         SSL_Write(SSL_Config_t* sslConfig, uint8_t* data, int length, int timeoutMs);
// int         SSL_Read(SSL_Config_t* sslConfig, uint8_t* data, int length, int timeoutMs);
// SSL_Error_t SSL_Close(SSL_Config_t* sslConfig);
// SSL_Error_t SSL_Destroy(SSL_Config_t* sslConfig);


#define SSL_Init     CSDK_FUNC(SSL_Init)
#define SSL_Connect  CSDK_FUNC(SSL_Connect)
#define SSL_Write    CSDK_FUNC(SSL_Write)
#define SSL_Read     CSDK_FUNC(SSL_Read)
#define SSL_Close    CSDK_FUNC(SSL_Close)
#define SSL_Destroy  CSDK_FUNC(SSL_Destroy)



#ifdef __cplusplus
}
#endif

#endif


