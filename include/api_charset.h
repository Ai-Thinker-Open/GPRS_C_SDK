
#ifndef __API_CHARSET_H
#define __API_CHARSET_H


#include "sdk_init.h"

#define GetCharset                        CSDK_FUNC(GetCharset)
#define Unicode2LocalLanguage             CSDK_FUNC(Unicode2LocalLanguage)
#define LocalLanguage2Unicode             CSDK_FUNC(LocalLanguage2Unicode)
#define Unicode2LocalLanguageBigEndian    CSDK_FUNC(Unicode2LocalLanguageBigEndian)
#define LocalLanguage2UnicodeBigEndian    CSDK_FUNC(LocalLanguage2UnicodeBigEndian)

// int32_t GSM_8BitTo7Bit(const uint8_t* pSrc, uint8_t* pDest, uint16_t nSrcSize );
#define GSM_8BitTo7Bit    CSDK_FUNC(GSM_8BitTo7Bit)
// int32_t GSM_7BitTo8Bit(const uint8_t* pSrc, uint8_t* pDest, uint16_t nSrcSize );
#define GSM_7BitTo8Bit    CSDK_FUNC(GSM_7BitTo8Bit)

#endif

