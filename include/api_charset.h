
#ifndef __API_CHARSET_H
#define __API_CHARSET_H


#include "sdk_init.h"

// const char*         GetCharset(Charset_t index);
// bool                Unicode2LocalLanguage(uint8_t* unicodeIn, uint16_t unicodeLenIn, Charset_t localLanguage, uint8_t** localOut, uint32_t* localLenOut);
// bool                LocalLanguage2Unicode(uint8_t* localIn, uint16_t localLenIn, Charset_t localLanguage, uint8_t** unicodeOut, uint32_t* unicodeLenOut);
// bool                Unicode2LocalLanguageBigEndian(uint8_t* unicodeIn, uint16_t unicodeLenIn, Charset_t localLanguage, uint8_t** localOut, uint32_t* localLenOut);
// bool                LocalLanguage2UnicodeBigEndian(uint8_t* localIn, uint16_t localLenIn, Charset_t localLanguage, uint8_t** unicodeOut, uint32_t* unicodeLenOut);
// int32_t             GSM_8BitTo7Bit(const uint8_t* pSrc, uint8_t* pDest, uint16_t nSrcSize );
// int32_t             GSM_7BitTo8Bit(const uint8_t* pSrc, uint8_t* pDest, uint16_t nSrcSize );

#define GetCharset                        CSDK_FUNC(GetCharset)
/**
 * @brief UCS2(unicode) to Local language
 * @param localOut: local language, need to call OS_Free() to free memory 
 */
#define Unicode2LocalLanguage             CSDK_FUNC(Unicode2LocalLanguage)
/**
 * @param localOut: local language, need to call OS_Free() to free memory 
 */
#define LocalLanguage2Unicode             CSDK_FUNC(LocalLanguage2Unicode)
/**
 * @param localOut: local language, need to call OS_Free() to free memory 
 */
#define Unicode2LocalLanguageBigEndian    CSDK_FUNC(Unicode2LocalLanguageBigEndian)
/**
 * @param localOut: local language, need to call OS_Free() to free memory 
 */
#define LocalLanguage2UnicodeBigEndian    CSDK_FUNC(LocalLanguage2UnicodeBigEndian)

// int32_t GSM_8BitTo7Bit(const uint8_t* pSrc, uint8_t* pDest, uint16_t nSrcSize );
#define GSM_8BitTo7Bit    CSDK_FUNC(GSM_8BitTo7Bit)
// int32_t GSM_7BitTo8Bit(const uint8_t* pSrc, uint8_t* pDest, uint16_t nSrcSize );
#define GSM_7BitTo8Bit    CSDK_FUNC(GSM_7BitTo8Bit)

#endif

