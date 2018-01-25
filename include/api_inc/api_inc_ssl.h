#ifndef __API_INC_SSL_H
#define __API_INC_SSL_H

#ifdef __cplusplus
extern "C"{
#endif

typedef enum{
    SSL_VERSION_SSLv3 = 0,
    SSL_VERSION_TLSv1    ,
    SSL_VERSION_TLSv1_1  ,
    SSL_VERSION_TLSv1_2  ,
    SSL_VERSION_MAX
}SSL_Version_t;

typedef enum{
    SSL_ERROR_NONE       =  0x00 ,
    SSL_ERROR_PARAM      = -0x01 ,
    SSL_ERROR_PARSE      = -0x02 ,
    SSL_ERROR_MALLOC_FAIL= -3    ,
    SSL_ERROR_CONNECTION = -4    ,
    SSL_ERROR_TIMEOUT    = -5    ,
    SSL_ERROR_FAIL       = -6    ,
    SSL_ERROR_INTERNAL           ,
    SSL_ERROR_MAX
} SSL_Error_t;

typedef enum{
    SSL_VERIFY_MODE_NONE     = 0,
    SSL_VERIFY_MODE_OPTIONAL = 1,
    SSL_VERIFY_MODE_REQUIRED = 2,
    SSL_VERIFY_MODE_MAX
}SSL_Verify_Mode_t;

typedef struct{
    const char*       caCert;
    const char*       caCrl;
    const char*       clientCert;
    const char*       clientKey;
    const char*       clientKeyPasswd;
    const char*       hostName;
    SSL_Version_t     minVersion;
    SSL_Version_t     maxVersion;
    SSL_Verify_Mode_t verifyMode;
    const char*       entropyCustom;
    //////////////////////////////////
    void* obj;//do not edit it
    //////////////////////////////////
} SSL_Config_t;

#ifdef __cplusplus
}
#endif

#endif


