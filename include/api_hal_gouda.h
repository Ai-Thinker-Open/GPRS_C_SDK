#ifndef __API_HAL_GOUDA_H
#define __API_HAL_GOUDA_H


#include "sdk_init.h"


#define hal_GoudaSerialOpen        CSDK_FUNC(hal_GoudaSerialOpen)
#define hal_GoudaClose             CSDK_FUNC(hal_GoudaClose)
#define hal_GoudaWriteCmd          CSDK_FUNC(hal_GoudaWriteCmd)
#define hal_GoudaWriteData         CSDK_FUNC(hal_GoudaWriteData)
#define hal_GoudaWriteReg          CSDK_FUNC(hal_GoudaWriteReg)
#define hal_GoudaReadData          CSDK_FUNC(hal_GoudaReadData)
#define hal_GoudaReadReg           CSDK_FUNC(hal_GoudaReadReg)
#define hal_GoudaBlitRoi           CSDK_FUNC(hal_GoudaBlitRoi)
#define hal_GoudaBlitRoi2Ram       CSDK_FUNC(hal_GoudaBlitRoi2Ram)
#define hal_GoudaIsActive          CSDK_FUNC(hal_GoudaIsActive)
#define hal_GoudaSetBgColor        CSDK_FUNC(hal_GoudaSetBgColor)
#define hal_GoudaGetBgColor        CSDK_FUNC(hal_GoudaGetBgColor)
#define hal_GoudaOvlLayerOpen      CSDK_FUNC(hal_GoudaOvlLayerOpen)
#define hal_GoudaOvlLayerClose     CSDK_FUNC(hal_GoudaOvlLayerClose)
#define hal_GoudaSetBlock          CSDK_FUNC(hal_GoudaSetBlock)





#endif

