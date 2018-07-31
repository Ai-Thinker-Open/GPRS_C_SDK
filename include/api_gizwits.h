#ifndef __API_GIZWITS_H
#define __API_GIZWITS_H

#include "sdk_init.h"


// bool GIZWITS_GetConfig(Gizwits_Config_t* config, const char* path);
// int GIZWITS_Connect(Gizwits_t* gizwits, Gizwits_Config_t* config, const char* configPath);
// int GIZWITS_Send(Gizwits_t* gizwits, Gizwits_Action_t action, uint8_t* data, int len);
// int GIZWITS_DoReceive(Gizwits_t* gizwits, OnReceivedOnePacket_Callback_t onOnePacket);
// bool GIZWITS_Ping(Gizwits_t* gizwits);
// int GIZWITS_GetSocket(Gizwits_t* gizwits);
// bool GIZWITS_Close(Gizwits_t* gizwits);

#define  GIZWITS_GetConfig       CSDK_FUNC(GIZWITS_GetConfig)
#define  GIZWITS_Connect         CSDK_FUNC(GIZWITS_Connect)
#define  GIZWITS_Send            CSDK_FUNC(GIZWITS_Send)
#define  GIZWITS_DoReceive       CSDK_FUNC(GIZWITS_DoReceive)
#define  GIZWITS_Ping            CSDK_FUNC(GIZWITS_Ping)
#define  GIZWITS_GetSocket       CSDK_FUNC(GIZWITS_GetSocket)
#define  GIZWITS_Close           CSDK_FUNC(GIZWITS_Close)



#endif
