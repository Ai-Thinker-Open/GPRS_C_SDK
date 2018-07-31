#ifndef __API_SIM_H_
#define __API_SIM_H_


#include <sdk_init.h>



/**
 * 
 * @param ccid: ccid out, length >= 20
 * 
 */
#define SIM_GetICCID     CSDK_FUNC(SIM_GetICCID)
#define SIM_GetIMSI      CSDK_FUNC(SIM_GetIMSI)


#endif
