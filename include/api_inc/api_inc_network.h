#ifndef __API_INC_NETWORK_H__
#define __API_INC_NETWORK_H__

#include <cs_types.h>

/*#################      network      #################################*/
#define PDP_APN_MAX_LENGTH         64
#define PDP_USER_NAME_MAX_LENGTH   64
#define PDP_USER_PASSWD_MAX_LENGTH 64

typedef enum{
    NETWORK_STATUS_OFFLINE = 0      ,
    NETWORK_STATUS_REGISTERING      ,
    NETWORK_STATUS_REGISTERED       ,
    NETWORK_STATUS_DETACHED         ,
    NETWORK_STATUS_ATTACHING        ,
    NETWORK_STATUS_ATTACHED         ,
    NETWORK_STATUS_DEACTIVED        ,
    NETWORK_STATUS_ACTIVATING       ,
    NETWORK_STATUS_ACTIVATED        ,
    NETWORK_STATUS_ATTACH_FAILED    ,
    NETWORK_STATUS_ACTIVATE_FAILED  ,
    NETWORK_STATUS_MAX
}Network_Status_t;

typedef struct{
    char    apn[PDP_APN_MAX_LENGTH];                     // Access Point Name
    char    userName[PDP_USER_NAME_MAX_LENGTH];
    char    userPasswd[PDP_USER_PASSWD_MAX_LENGTH];
}Network_PDP_Context_t;

typedef struct 
{
    uint8_t  sMcc[3];
    uint8_t  sMnc[3];
    uint16_t sLac;
    uint16_t sCellID;
    uint8_t  iBsic;
    int8_t   iRxLev;
    uint8_t  iRxLevSub;
    uint16_t nArfcn;
} Network_Location_t;

typedef enum{
    NETWORK_ERROR_NONE = 0      ,
    NETWORK_ERROR_MAX
} Network_Error_t;

typedef struct{
    uint8_t signalLevel;
    uint8_t bitError;
}Network_Signal_Quality_t;

typedef void (*Network_Callback_Func_t)(Network_Status_t status);

#endif
