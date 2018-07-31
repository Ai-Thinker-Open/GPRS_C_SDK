/*
 * @File  api_lbs.c
 * @Brief get location(longitude and latitude) from server though base station(BS) information
 * 
 * @Author: Neucrack 
 * @Date: 2018-06-21 14:32:29 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-21 14:57:14
 */


#ifndef __API_LBS_H
#define __API_LBS_H

#include "stdint.h"
#include "stdbool.h"
#include "api_network.h"
#include "sdk_init.h"


// bool LBS_GetLocation(Network_Location_t* bsInfo, uint8_t bsNumber,int timeoutS, float* longitude, float* latitude);
#define  LBS_GetLocation       CSDK_FUNC(LBS_GetLocation)


#endif

