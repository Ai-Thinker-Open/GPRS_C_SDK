#ifndef __API_FOTA_H__
#define __API_FOTA_H__

/******************************************************************************/

#include "sdk_init.h"
/*********************************************************************************/
/// \brief  init fota buf ,need large malloc
/// \param  size to malloc
// bool API_FotaInit(int size);
#define API_FotaInit     CSDK_FUNC(API_FotaInit)
/*********************************************************************************/

/*********************************************************************************/
/// \brief  upgrade data need to write to flash
/// \param  data
/// \param  len
// int API_FotaReceiveData(unsigned char *data, int len);
//success return length else 0
#define API_FotaReceiveData     CSDK_FUNC(API_FotaReceiveData)
/*********************************************************************************/

/*********************************************************************************/
/// \brief  free the malloc data
// void API_FotaClean(void);
#define API_FotaClean     CSDK_FUNC(API_FotaClean)
/*********************************************************************************/

/*********************************************************************************/
/// \brief  get upgrade from server
/// \param  url get pack form url
/// \param  data_process the function receive data
// bool API_FotaInit(int size);
#define API_FotaByServer     CSDK_FUNC(API_FotaByServer)
/*********************************************************************************/
#endif
