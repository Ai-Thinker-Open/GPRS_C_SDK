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


#endif

