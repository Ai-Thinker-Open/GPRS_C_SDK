#ifndef API_SS_H
#define API_SS_H

#include "stdint.h"

typedef enum{
    USSD_ERROR_LOCAL_NETWORK_REJECT = 0xfa,
    USSD_ERROR_NETWORK_RETURN = 0xfb,
    USSD_ERROR_NETWORK_RETURN_IE = 0xfd,

    USSD_ERROR_SERVICE_NOT_IMPLEMENTED = 10,
    USSD_ERROR_INVALID_OPERATION = 11,
    USSD_ERROR_INVALID_SERVICE_CODE = 12,
    USSD_ERROR_MEM = 14,
    USSD_ERROR_MM_ESTABLISHMENT = 20,
    USSD_ERROR_TRANSACTION_TIME_OUT = 21,
    USSD_ERROR_MM_CONNECTION_RELEASED = 22,
    USSD_ERROR_UNKNOWN_COMPONENT_RECEIVED = 23,
    USSD_ERROR_MAX
}USSD_Error_t;


// -----------------------------------------------------------------------------------------------------------------------------
// parameter
// pUsdString[in]:     pointer the to USSD string. It should be decoded codes.
// nUsdStringSize[in]:   size of param pUsdString. different meaning in different nOption, see below
// nOption[in]:      operation option of the USSD string. available value can be divided into two groups,
// one is for USSD v2:
// 0 (response the NETWORK with an error), in this case ,param nUsdStringSize must be input as api_SsUSSDErrorPresent_t, not just UINT8.
// 1 (response the NETWORK with reject), in this case ,param nUsdStringSize must be input as api_SsProblemCodeTag_t, not just UINT8.
// 2 (response the NETWORK with MS release request)
// 3 (response the NETWORK with user data or invoke USSD to network)
// the other is for USSD v1:
// 128 (response the NETWORK with an error),  in this case ,param nUsdStringSize must be input as api_SsUSSDErrorPresent_t, not just UINT8.
// 129 (response the NETWORK with reject), in this case ,param nUsdStringSize must be input as api_SsProblemCodeTag_t, not just UINT8.
// 130 (response the NETWORK with MS release request)
// 131 (response the NETWORK with user data or invoke USSD to network)
// nDcs[in]:   decode scheme for pUsdString, available only when nOption support USSD V2.
// -------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
    uint8_t* usdString; //pointer the to USSD string. It should be decoded codes.
                        //(e.g. if use GSM 7bit data, you can call GSM_8BitTo7Bit(...) to vonvert 8bit data to 7bit data)
    uint8_t  usdStringSize;
    uint8_t  option; //0 1 2 3
    uint8_t  dcs;   //Data Coding Scheme, 0x0f: Default GSM 7bit and language unspecified
} USSD_Type_t;


#endif
