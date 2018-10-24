#ifndef __API_NETWORK_H_
#define __API_NETWORK_H_

#include <sdk_init.h>
/*
#define PDP_APN_MAX_LENGTH         64
#define PDP_USER_NAME_MAX_LENGTH   64
#define PDP_USER_PASSWD_MAX_LENGTH 64

typedef enum{
    NETWORK_STATUS_OFFLINE = 0      ,
    NETWORK_STATUS_REGISTERING      ,
    NETWORK_STATUS_REGISTERED       ,
    NETWORK_STATUS_DETACHED         ,
    NETWORK_STATUS_ATTACHING        ,
    NETWORK_STATUS_ATTACHED         ,
    NETWORK_STATUS_DEACTIVED        ,
    NETWORK_STATUS_ACTIVATING       ,
    NETWORK_STATUS_ACTIVATED        ,
    NETWORK_STATUS_ATTACH_FAILED    ,
    NETWORK_STATUS_ACTIVATE_FAILED  ,
    NETWORK_STATUS_MAX
}Network_Status_t;

typedef struct{
    char    apn[PDP_APN_MAX_LENGTH];                     // Access Point Name
    char    userName[PDP_USER_NAME_MAX_LENGTH];
    char    userPasswd[PDP_USER_PASSWD_MAX_LENGTH];
}Network_PDP_Context_t;

typedef void (*Network_Callback_Func_t)(Network_Status_t status);
*/

/**
  *
  * @param callback register status changed callback, register status changed event also can be caught in the AppMain task( so callback can be set to NULL)
  *
  */
// void Network_SetStatusChangedCallback(Network_Callback_Func_t callback);
// bool Network_StartAttach();
// bool Network_StartDetach();
// bool Network_StartActive(Network_PDP_Context_t context);
// bool Network_StartDeactive(uint8_t contextID);




/// \brief  void Network_SetStatusChangedCallback(Network_Callback_Func_t callback);
/// \param  Network_Callback_Func_t callback, set the network state change callback func
/// \return void
#define Network_SetStatusChangedCallback  CSDK_FUNC(Network_SetStatusChangedCallback)

/// \brief  bool Network_StartAttach();
/// \param  void , start network attach
/// \return bool
#define Network_StartAttach               CSDK_FUNC(Network_StartAttach)

/// \brief  bool Network_StartDetach();
/// \param  void , start network detach
/// \return bool
#define Network_StartDetach               CSDK_FUNC(Network_StartDetach)

/// \brief  bool Network_StartActive(Network_PDP_Context_t context);
/// \param  Network_PDP_Context_t context, the pdp context need when active network
/// \return bool
#define Network_StartActive               CSDK_FUNC(Network_StartActive)

/// \brief  bool Network_StartDeactive(uint8_t contextID);
/// \param  uint8_t contextID, the pdp context id to start deactive network
/// \return bool
#define Network_StartDeactive             CSDK_FUNC(Network_StartDeactive)


#define Network_GetAttachStatus           CSDK_FUNC(Network_GetAttachStatus)

#define Network_GetActiveStatus           CSDK_FUNC(Network_GetActiveStatus)


// bool Network_GetIp(char* ip, uint8_t size);
#define Network_GetIp                     CSDK_FUNC(Network_GetIp)

// bool Network_GetCellInfoRequst();
#define Network_GetCellInfoRequst         CSDK_FUNC(Network_GetCellInfoRequst)

// bool Network_GetSignalQuality(Network_Signal_Quality_t* sq);
#define Network_GetSignalQuality         CSDK_FUNC(Network_GetSignalQuality)

// bool Network_GetStatusInfo(Network_Status_Info_t* status);
#define Network_GetStatusInfo         CSDK_FUNC(Network_GetStatusInfo)

// bool Network_SetFlightMode(bool enable);
#define Network_SetFlightMode         CSDK_FUNC(Network_SetFlightMode)

// bool Network_GetFlightMode(bool* enable);
#define Network_GetFlightMode         CSDK_FUNC(Network_GetFlightMode)

/**
 * 
 * @param operatorID[6] e.g. [4,6,0,0,0,0]
 */
// bool Network_Register(uint8_t* operatorID, Network_Register_Mode_t mode);
#define Network_Register         CSDK_FUNC(Network_Register)

// bool Network_DeRegister();
#define Network_DeRegister         CSDK_FUNC(Network_DeRegister)

/**
 * 
 * @param freqBand Value from Network_Freq_Band_t, can be multiple value,
 *                 e.g. NETWORK_FREQ_BAND_DCS_1800|NETWORK_FREQ_BAND_PCS_1900
 */
// bool Network_SetFrequencyBand(uint8_t freqBand);
#define Network_SetFrequencyBand         CSDK_FUNC(Network_SetFrequencyBand)

// bool Network_GetCurrentOperator(uint8_t operatorId[6], Network_Register_Mode_t* mode );
#define Network_GetCurrentOperator         CSDK_FUNC(Network_GetCurrentOperator)

/**
 * Get operator name and id from query table
 * e.g.
 * bool ret = ture;
 * uint8_t index = 0;
 * uint8_t* operatorId;
 * uint8_t* operatorName;
 * while(ret)
 * {
 *     ret= Network_GetOperatorInfo(index++,&operatorId,&operatorName);
 *     if(ret)
 *         printf("ID:%d%d%d%d%d%d,name:%s",operatorId[0],operatorId[1],operatorId[2],operatorId[3],operatorId[4],operatorId[5],operatorName);
 * }
 */
// bool Network_GetOperatorInfo(uint8_t index, uint8_t** operatorId, uint8_t** operatorName);
#define Network_GetOperatorInfo         CSDK_FUNC(Network_GetOperatorInfo)

// bool Network_GetOperatorNameById(uint8_t* operatorId, uint8_t** operatorName);
#define Network_GetOperatorNameById         CSDK_FUNC(Network_GetOperatorNameById)

// bool Network_GetOperatorIdByName(uint8_t* operatorName,uint8_t** operatorId);
#define Network_GetOperatorIdByName         CSDK_FUNC(Network_GetOperatorIdByName)

/**
 * Send get available
 */
// bool Network_GetAvailableOperatorReq();
#define Network_GetAvailableOperatorReq         CSDK_FUNC(Network_GetAvailableOperatorReq)

#endif
