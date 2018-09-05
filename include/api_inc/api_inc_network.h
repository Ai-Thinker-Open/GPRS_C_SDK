#ifndef __API_INC_NETWORK_H__
#define __API_INC_NETWORK_H__

#include "stdbool.h"
#include "stdint.h"

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

typedef enum{
    NETWORK_REGISTER_MODE_MANUAL = 0,
    NETWORK_REGISTER_MODE_AUTO   = 1,
    NETWORK_REGISTER_MODE_MANUAL_AUTO = 4,
    NETWORK_REGISTER_MODE_MAX
}Network_Register_Mode_t;

/* 
API_GSM_450, API_GSM_480, API_GSM_900R bands are not supported by STACK now, 
MMI should not set Band to these value. These macros are reserved only for compatible.
*/
typedef enum{
    // NETWORK_FREQ_BAND_GSM_450  = ( 1 << 0 ),
    // NETWORK_FREQ_BAND_GSM_480  = ( 1 << 1 ),
    NETWORK_FREQ_BAND_GSM_900P = ( 1 << 2 ),
    NETWORK_FREQ_BAND_GSM_900E = ( 1 << 3 ),
    // NETWORK_FREQ_BAND_GSM_900R = ( 1 << 4 ),
    NETWORK_FREQ_BAND_GSM_850  = ( 1 << 5 ),
    NETWORK_FREQ_BAND_DCS_1800 = ( 1 << 6 ),
    NETWORK_FREQ_BAND_PCS_1900 = ( 1 << 7 ),
    NETWORK_FREQ_BAND_MAX
}Network_Freq_Band_t;

typedef struct
{
    uint8_t operatorId[6];
    uint8_t status;      //0:unkown, 1: available, 2: current, 3: disabled
    uint8_t hasPSFlag;   //1: has PLMN PS, 0: No PLMN PS
} Network_Operator_Info_t;

typedef struct
{
    uint8_t status;
    uint8_t cause;           //only use PS no service
    uint8_t psType;          //add for diff GPRS or EDGE by wuys 2013-07-19.  1:GPRS  2:EDGE
    uint8_t csStatusChange;  //add for if CS status have changed,we will set the TRUE.
    uint8_t areaCode[5];
    uint8_t cellId[2];
    uint8_t currRat;         //reserved! 
} Network_Status_Info_t;

typedef void (*Network_Callback_Func_t)(Network_Status_t status);

#endif
