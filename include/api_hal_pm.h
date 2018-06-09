/*
 * @File  api_hal_pm.h
 * @Brief api of power manager
 * 
 * @Author: Neucrack 
 * @Date: 2017-10-28 10:10:24 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2017-11-02 11:52:53
 */


#ifndef __API_HAL_PM_H__
#define __API_HAL_PM_H__

#include <sdk_init.h>

/*
typedef enum{
    POWER_TYPE_VPAD = 0,            // GPIO0  ~ GPIO7  and GPIO25 ~ GPIO36    2.8V   //always on
    POWER_TYPE_MMC,                 // GPIO8  ~ GPIO13                        1.9V
    POWER_TYPE_LCD,                 // GPIO14 ~ GPIO18                        1.9V
    POWER_TYPE_CAM,                 // GPIO19 ~ GPIO24                        1.9V
    POWER_TYPE_MAX
}Power_Type_t;

typedef enum{
    POWER_ON_CAUSE_KEY = 0  ,
    POWER_ON_CAUSE_CHARGE   ,
    POWER_ON_CAUSE_ALARM    ,
    POWER_ON_CAUSE_EXCEPTION,
    POWER_ON_CAUSE_RESET    ,
    POWER_ON_CAUSE_MAX
}Power_On_Cause_t;
*/

#define PM_PowerEnable      CSDK_FUNC(PM_PowerEnable)
#define PM_SleepMode        CSDK_FUNC(PM_SleepMode)
#define PM_Voltage          CSDK_FUNC(PM_Voltage)
#define PM_ShutDown         CSDK_FUNC(PM_ShutDown)
#define PM_Restart          CSDK_FUNC(PM_Restart)
#define PowerEnable         CSDK_FUNC(PM_PowerEnable)

// void PM_SetSysMinFreq(PM_Sys_Freq_t freq);
#define PM_SetSysMinFreq    CSDK_FUNC(PM_SetSysMinFreq)

#endif

