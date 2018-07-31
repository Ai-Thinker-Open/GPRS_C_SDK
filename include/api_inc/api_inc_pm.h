/*
 * @File  api_hal_power_manager.h
 * @Brief 
 * 
 * @Author: Neucrack 
 * @Date: 2017-10-28 10:10:24 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2017-11-02 11:52:53
 */


#ifndef __API_POWER_MANAGER_H__
#define __API_POWER_MANAGER_H__


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

typedef enum{
    PM_SYS_FREQ_32K     = 32768,
    PM_SYS_FREQ_13M     = 13000000,
    PM_SYS_FREQ_26M     = 26000000,
    PM_SYS_FREQ_39M     = 39000000,
    PM_SYS_FREQ_52M     = 52000000,
    PM_SYS_FREQ_78M     = 78000000,
    PM_SYS_FREQ_89M     = 89142857, //624M/7
    PM_SYS_FREQ_104M    = 104000000,
    PM_SYS_FREQ_113M    = 113454545, //624M/5.5
    PM_SYS_FREQ_125M    = 124800000,
    PM_SYS_FREQ_139M    = 138666667, //624M/4.5
    PM_SYS_FREQ_156M    = 156000000,
    PM_SYS_FREQ_178M    = 178285714, //624M/3.5
    PM_SYS_FREQ_208M    = 208000000,
    PM_SYS_FREQ_250M    = 249600000, //624M/2.5
    PM_SYS_FREQ_312M    = 312000000,
    PM_SYS_FREQ_UNKNOWN = 0,
} PM_Sys_Freq_t;

typedef enum{
    PM_CHARGER_STATE_DISCONNECTED      = 0,
    PM_CHARGER_STATE_CONNECTED         = 1,
    PM_CHARGER_STATE_CHRGING           = 2,
    PM_CHARGER_STATE_FINISHED          = 3,
    PM_CHARGER_STATE_ERROR_TEMPERATURE = 4,
    PM_CHARGER_STATE_ERROR_VOLTAGE     = 5,
    PM_CHARGER_STATE_ERROR_UNKNOWN     = 9,
    PM_CHARGER_STATE_MAX
}PM_Charger_State_t;

typedef enum{
    PM_BATTERY_STATE_NORMAL     = 0,
    PM_BATTERY_STATE_LOW        = 1,
    PM_BATTERY_STATE_CRITICAL   = 2,
    PM_BATTERY_STATE_SHUTDOWN   = 3,
    PM_BATTERY_STATE_UNKNOWN    = 4,
    PM_BATTERY_STATE_MAX
}PM_Battery_State_t;


#endif

