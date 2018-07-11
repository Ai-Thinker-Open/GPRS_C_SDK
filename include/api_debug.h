
#ifndef __API_DEBUG_H_
#define __API_DEBUG_H_

#include "sdk_init.h"


/**
  * @brief bool Trace(uint16_t nIndex,PCSTR fmt, ...)
  *   send debug infomation to tracer of coolwatcher
  * @param      nIndex: trace level( cool watcher will print level as `MMI nIndex`)
  * @param      fmt: format string eg:"test integet:%d"
  * @param      ...: parameters
  * @attention not support float(%f) yet! can use `gcvt()` convert to string firstly
  */ 
#define Trace                                   CSDK_FUNC(Trace)
#define MEMBLOCK_Trace                          CSDK_FUNC(MEMBLOCK_Trace)
void Trace_MemBlock(UINT16 nIndex, UINT8 *buffer, UINT16 len, UINT8 radix);

#define LIBS_DEBUG_I       15
#define LIBS_DEBUG_E       16

#endif