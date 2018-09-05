#ifndef __LIMITS_H
#define __LIMITS_H
// #include "cs_types.h"


#define MAX_INT8                    127
#define MIN_INT8                    (-MAX_INT8 - 1)
#define MAX_UINT8                   255
#define MIN_UINT8                   0
#define MAX_INT16                   32767
#define MIN_INT16                   (-MAX_INT16 - 1)
#define MAX_UINT16                  65535
#define MIN_UINT16                  0
#define MAX_INT32                   2147483647L
#define MIN_INT32                   (-MAX_INT32 - 1)
#define MAX_UINT32                  4294967295U
#define MIN_UINT32                  0U
#ifdef WITH_LONG_LONG
#define INTMAX_MAX                  0x7fffffffffffffff
#define INTMAX_MIN                  -(0x7fffffffffffffff-1)
#define UINTMAX_MAX                 0xffffffffffffffff
#else
#define INTMAX_MAX                  0x7fffffff
#define INTMAX_MIN                  -(0x7fffffff-1)
#define UINTMAX_MAX                 0xffffffff
#endif
#define INT_MAX                     0x7fffffff
#define UINT_MAX                    0xffffffff
#define LONG_MAX                    0x7fffffff
#define LONG_MIN                    -(0x7fffffff-1)
#define ULONG_MAX                   0xffffffff

#define CHAR_BIT      8
#define SCHAR_MIN     MIN_INT8
#define SCHAR_MAX     MAX_INT8
#define UCHAR_MAX     MAX_UINT8
#define CHAR_MIN      MIN_INT8
#define CHAR_MAX      MAX_INT8
#define MB_LEN_MAX    1
#define SHRT_MIN	  MIN_INT16	
#define SHRT_MAX	  MAX_INT16	
#define USHRT_MAX	  MAX_UINT16	
#define INT_MIN	      MIN_INT32	
// #define INT_MAX	      MAX_INT32
// #define UINT_MAX	  MAX_UINT32	
// #define LONG_MIN	  LONG_MIN
// #define LONG_MAX	  LONG_MAX
// #define ULONG_MAX     ULONG_MAX



#endif
