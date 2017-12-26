#ifndef ___TIME_H__
#define ___TIME_H__

#include <sdk_init.h>

#define gettimeofday                  CSDK_FUNC(gettimeofday)
#define TIME_SetIsAutoUpdateRtcTime   CSDK_FUNC(TIME_SetIsAutoUpdateRtcTime)
#define TIME_IsAutoUpdateRtcTime      CSDK_FUNC(TIME_IsAutoUpdateRtcTime)
#define TIME_SetRtcTime               CSDK_FUNC(TIME_SetRtcTime)
#define TIME_GetRtcTIme               CSDK_FUNC(TIME_GetRtcTIme)
#define clock                         CSDK_FUNC(clock)


#endif