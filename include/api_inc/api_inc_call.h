
#ifndef __API_INC_CALL_H
#define __API_INC_CALL_H


typedef enum
{
    CALL_DTMF_GAIN_0dB = 0,
    CALL_DTMF_GAIN_m3dB,
    CALL_DTMF_GAIN_m9dB,
    CALL_DTMF_GAIN_m15dB,

    CALL_DTMF_GAIN_MAX
} CALL_DTMF_Gain_t;

typedef enum{
    CALL_ERROR_NONE              = 0  ,
    CALL_ERROR_NO_DIAL_TONE           ,  //TERMINAL USER HAS POWERED OFF
    CALL_ERROR_BUSY                   ,  //TERMINAL USER RETURN UDUB
    CALL_ERROR_NO_ANSWER              ,  //TERMINAL USER DOESN'T ANSWER THE CALL
    CALL_ERROR_NO_CARRIER             ,  //remote hang up
    CALL_ERROR_NETWORK_TIME_OUT       ,
    CALL_ERROR_LINK_IS_BEINDG_BUILT   ,  //link is being built, can not hang up
    CALL_ERROR_UNKOWN                 ,
    
    CALL_ERROR_MAX
}CALL_Error_t;

typedef struct {
    uint8_t index;
    uint8_t direction;
    uint8_t status;
    uint8_t mode;
    bool    multiparty;
    char    number[22];
    uint8_t numberType;
}CALL_Status_t;


#endif

