#ifndef __API_INC_OS_H__
#define __API_INC_OS_H__

#include "stdbool.h"
#include "stdint.h"

#ifndef TYPE_HANDLE
#define TYPE_HANDLE
typedef uint32_t  HANDLE;
#endif

/*#################      OS       #################################*/

#define OS_WAIT_FOREVER 0xFFFFFFFF
#define OS_NO_WAIT 0x0

#define OS_EVENT_PRI_NORMAL 0
#define OS_EVENT_PRI_URGENT 1


#define OS_TIME_OUT_WAIT_FOREVER 0xFFFFFFFF
#define OS_TIME_OUT_NO_WAIT      0x0


typedef struct{
    uint32_t usedSize;
    uint32_t totalSize;
}OS_Heap_Status_t;


typedef struct{
    const char*    name;
    uint8_t        priority;
    uint16_t       stackSize; // unit: Word(4 Bytes), so stack size in byte is stackSize*4
    uint32_t       stackTop;
}OS_Task_Info_t;


typedef void (*PTASK_FUNC_T)(void* pParameter);
typedef void (*OS_CALLBACK_FUNC_T)(void* param);

#endif
