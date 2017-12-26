#ifndef __API_INC_SMS_H__
#define __API_INC_SMS_H__

#include <cs_types.h>

#define  SMS_BODY_MAX_LEN  176


typedef enum{
    // SMS_FORMAT_PDU  = 0, // not support now
    SMS_FORMAT_TEXT = 1, // default
    SMS_FORMAT_MAX
}SMS_Format_t;


typedef enum{
    SMS_ERROR_DECODE_ERROR = 0,
    SMS_ERROR_MAX
}SMS_Error_t;

typedef enum{
    SMS_ENCODE_TYPE_ASCII  = 0,
    SMS_ENCODE_TYPE_UNICODE   ,
    SMS_ENCODE_TYPE_MAX
}SMS_Encode_Type_t;


typedef struct
{
    uint8_t fo;  // default: 17
    uint8_t vp;  // default: 167
    uint8_t pid; // default: 0
    uint8_t dcs; // default: 0:English 7bit, 4:English 8 bit, 8:Unicode 2 Bytes(UCS2)
} SMS_Parameter_t;

#endif
