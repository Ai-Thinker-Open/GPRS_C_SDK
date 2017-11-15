#ifndef __API_INC_SMS_H__
#define __API_INC_SMS_H__

#include <cs_types.h>

/*#################      sms      #################################*/
typedef enum{
    SMS_FORMAT_PDU  = 0, // not support now
    SMS_FORMAT_TEXT = 1,
    SMS_FORMAT_MAX
}SMS_Format_t;


typedef enum{
    CHARSET_CP437      = 0  ,
    CHARSET_CP737           ,
    CHARSET_CP775           ,
    CHARSET_CP850           ,
    CHARSET_CP852           ,
    CHARSET_CP855           ,
    CHARSET_CP857           ,
    CHARSET_CP860           ,
    CHARSET_CP861           ,
    CHARSET_CP862           ,
    CHARSET_CP863           ,
    CHARSET_CP864           ,
    CHARSET_CP865           ,
    CHARSET_CP869           ,
    CHARSET_CP874           ,
    CHARSET_CP932           ,
    CHARSET_CP936           ,  // GBK
    CHARSET_CP950           ,
    CHARSET_CP1250          ,
    CHARSET_CP1251          ,
    CHARSET_CP1255          ,
    CHARSET_CP1256          ,
    CHARSET_CP1258          ,
    CHARSET_ISO8859_1       , //
    CHARSET_ISO8859_2       ,
    CHARSET_ISO8859_3       ,
    CHARSET_ISO8859_4       ,
    CHARSET_ISO8859_5       ,
    CHARSET_ISO8859_6       ,
    CHARSET_ISO8859_7       ,
    CHARSET_ISO8859_9       ,
    CHARSET_ISO8859_13      ,
    CHARSET_ISO8859_14      ,
    CHARSET_ISO8859_15      ,
    CHARSET_MAX
}Charset_t;


typedef struct
{
    uint8_t fo;  //17
    uint8_t vp;  //167
    uint8_t pid; //0
    uint8_t dcs; //0 4 8
    const char* charset; //NULL or SMS_GetCharset(Charset_t)
} SMS_Parameter_t;

#endif
