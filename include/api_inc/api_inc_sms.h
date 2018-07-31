#ifndef __API_INC_SMS_H__
#define __API_INC_SMS_H__


#define  SMS_PHONE_NUMBER_MAX_LEN  21
#define  SMS_BODY_MAX_LEN  176

#include <api_inc_time.h>

typedef enum{
    // SMS_FORMAT_PDU  = 0, // not support now
    SMS_FORMAT_TEXT = 1, // default
    SMS_FORMAT_MAX
} SMS_Format_t;


typedef enum{
    SMS_ERROR_DECODE_ERROR = 0,
    SMS_ERROR_MAX
} SMS_Error_t;

typedef enum{
    SMS_ENCODE_TYPE_ASCII  = 0,
    SMS_ENCODE_TYPE_UNICODE   ,
    SMS_ENCODE_TYPE_MAX
} SMS_Encode_Type_t;


typedef struct
{
    uint8_t fo;  // default: 17
    uint8_t vp;  // default: 167
    uint8_t pid; // default: 0
    uint8_t dcs; // default: 0:English 7bit, 4:English 8 bit, 8:Unicode 2 Bytes(UCS2)
} SMS_Parameter_t;

typedef enum{
    SMS_NUMBER_TYPE_UNKNOWN         = 129 ,
    SMS_NUMBER_TYPE_INTERNATIONAL   = 145 ,
    SMS_NUMBER_TYPE_NATIONAL        = 161
} SMS_Number_Type_t; //SMS_Server_Center_Addr_Type_t;

typedef struct{
    char*          addr;
    SMS_Number_Type_t addrType;
} SMS_Server_Center_Info_t;

typedef enum{
    SMS_STATUS_UNREAD                    = 0x01 ,
    SMS_STATUS_READ                      = 0x02 ,
    SMS_STATUS_UNSENT                    = 0x04 ,
    SMS_STATUS_SENT_NOT_SR_REQ           = 0x08 ,
    SMS_STATUS_SENT_SR_REQ_NOT_RECV      = 0x10 ,
    SMS_STATUS_SENT_SR_REQ_NOT_STORE     = 0x20 ,
    SMS_STATUS_SENT_SR_REQ_RECV_STORE    = 0x40 ,
    SMS_STATUS_ALL                       = 0x80 ,
    SMS_STATUS_MAX
} SMS_Status_t;

typedef struct{
    //header
    uint8_t           index;
    SMS_Status_t      status;
    SMS_Number_Type_t phoneNumberType;
    char              phoneNumber[SMS_PHONE_NUMBER_MAX_LEN];
    RTC_Time_t        time;
    
    //body
    uint16_t          dataLen;
    uint8_t*          data;
} SMS_Message_Info_t;

typedef struct{
    uint16_t used;
    uint16_t total;
    uint16_t unReadRecords;
    uint16_t readRecords;
    uint16_t sentRecords;
    uint16_t unsentRecords;
    uint16_t unknownRecords;
    uint16_t storageId;
} SMS_Storage_Info_t;

typedef enum{
    SMS_STORAGE_FLASH    = 1,
    SMS_STORAGE_SIM_CARD = 2,
    SMS_STORAGE_MAX
} SMS_Storage_t;

#endif
