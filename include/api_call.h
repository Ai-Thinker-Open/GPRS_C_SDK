
#ifndef __API_CALL_H
#define __API_CALL_H

#include "sdk_init.h"



#define CALL_Dial            CSDK_FUNC(CALL_Dial)
#define CALL_HangUp          CSDK_FUNC(CALL_HangUp)
#define CALL_Answer          CSDK_FUNC(CALL_Answer)
/**
 * bool CALL_DTMF(char dtmf, CALL_DTMF_Gain_t attenuation, uint8_t duration, uint8_t speakerGain, bool send);
 * @param number: '0'~'9', '#', '*', 'A'~'D'
 * @param duration: DTMF duration
 * @param send: false:play DTMF but not send to  a remote subscriber, true: plat DTMF and send
 */
#define CALL_DTMF            CSDK_FUNC(CALL_DTMF)

/**
 * bool                (*CALL_Status)(CALL_Status_t** callStatus_, uint8_t* count);
 * @attention must call OS_Free() to free first param
 * 
 */
#define CALL_Status          CSDK_FUNC(CALL_Status)


#endif

