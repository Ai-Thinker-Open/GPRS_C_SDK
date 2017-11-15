
/*
 * @File  api_info.h
 * @Brief infomation of api of system infomation
 * 
 * @Author: Neucrack 
 * @Date: 2017-11-09 17:14:03 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2017-11-15 17:54:36
 */


#ifndef __API_INFO_H_
#define __API_INFO_H_

#include <sdk_init.h>

/**
  * @brief get IMEI of GSM module
  *
  * @param  pImei: IMEI return, array size must at least 15 bytes
  * @return Is get IMEI success
  * 
  */
// bool INFO_GetIMEI(uint8_t* pImei);

#define INFO_GetIMEI CSDK_FUNC(INFO_GetIMEI)

#endif

