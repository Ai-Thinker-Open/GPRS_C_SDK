#ifndef __API_INC_OS_H__
#define __API_INC_OS_H__

#include <cs_types.h>

/*#################      OS       #################################*/
typedef void (*PTASK_FUNC_T)(PVOID pParameter);
typedef void (*OS_CALLBACK_FUNC_T)(void* param);
typedef struct _OS_HEAP_STATUS
{
    UINT32 nUsedSize;
    UINT32 nTotalSize;
}OS_Heap_Status_t;


#endif
