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

#endif
