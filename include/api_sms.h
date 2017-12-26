
#ifndef __API_SMD_H_
#define __API_SMD_H_

#include <sdk_init.h>

/// \brief  bool SMS_SetFormat(SMS_Format_t format, SIM_ID_t simID);
/// \param  SMS_Format_t format  , set the sms format
/// \param  SIM_ID_t simID  , set the sim id
/// \return bool , 
#define SMS_SetFormat                               CSDK_FUNC(SMS_SetFormat)

/// \brief  bool SMS_SetParameter(SMS_Parameter_t* smsParameter,SIM_ID_t simID);
/// \param  SMS_Parameter_t* smsParameter , get sms parameter
/// \param  SIM_ID_t simID  , set the sim id
/// \return bool , 
#define SMS_SetParameter                            CSDK_FUNC(SMS_SetParameter)

/// \brief  bool SMS_SendMessage(const char* phoneNumber, uint8_t* message,  uint8_t pduLength, SIM_ID_t simID);
/// \param  const char* phoneNumber  , the phone number sent sms to 
/// \param  uint8_t* message  , the meassage sented 
/// \param  uint8_t length  , length
/// \param  SIM_ID_t simID  , set the sim id
/// \return bool , 
#define SMS_SendMessage                             CSDK_FUNC(SMS_SendMessage)

/// \brief  const char* SMS_GetCharset(Charset_t);
/// \param  HANDLE hSem  , 
/// \return const char* , get the charset
#define SMS_GetCharset                              CSDK_FUNC(SMS_GetCharset)

#define SMS_Unicode2LocalLanguage                   CSDK_FUNC(SMS_Unicode2LocalLanguage)
#define SMS_LocalLanguage2Unicode                   CSDK_FUNC(SMS_LocalLanguage2Unicode)

#endif

