#ifndef __API_INC_GIZWITS_H
#define __API_INC_GIZWITS_H

#include "stdbool.h"
#include "stdint.h"


typedef struct 
{
	char	   imei[32];
	char	   did[32];
	char	   passcode[16];
	char	   pk[48];
	char	   pk_secret[48];
	char	   hard_version[16];
	char	   soft_version[16];
    uint16_t   alive;
}Gizwits_Config_t; 


typedef enum{
    GIZWITS_FIXED_LENGTH_ACTION_CONTROL			   =  0x01 ,
    GIZWITS_FIXED_LENGTH_ACTION_READ_STATUS		   =  0x02 ,
    GIZWITS_FIXED_LENGTH_ACTION_READ_STATUS_ACK	   =  0x03 ,
    GIZWITS_FIXED_LENGTH_ACTION_REPORT_STATUS	   =  0x04 ,
    GIZWITS_FIXED_LENGTH_ACTION_TRANS_RECV		   =  0x05 ,
    GIZWITS_FIXED_LENGTH_ACTION_TRANS_SEND		   =  0x06 ,
    GIZWITS_FIXED_LENGTH_ACTION_PUSH_OTA	       =  0xfe ,

    GIZWITS_VARIABLE_LENGTH_ACTION_CONTROL			 = 0x11 ,
    GIZWITS_VARIABLE_LENGTH_ACTION_READ_STATUS		 = 0x12 ,
    GIZWITS_VARIABLE_LENGTH_ACTION_READ_STATUS_ACK	 = 0x13 ,
    GIZWITS_VARIABLE_LENGTH_ACTION_REPORT_STATUS	 = 0x14 ,
    GIZWITS_VARIABLE_LENGTH_ACTION_TRANS_RECV		 = 0x15 ,
    GIZWITS_VARIABLE_LENGTH_ACTION_TRANS_SEND		 = 0x16 ,
    GIZWITS_VARIABLE_LENGTH_ACTION_PUSH_OTA	         = 0xfe ,

    GIZWITS_ACTION_MAX
}Gizwits_Action_t;



typedef struct{
    Gizwits_Config_t* config;
    void*    cloud;
    char*    otaUrl;
}Gizwits_t;

typedef void (*OnReceivedOnePacket_Callback_t)(Gizwits_t* gizwits, Gizwits_Action_t action,uint8_t* data, int len);


#endif
