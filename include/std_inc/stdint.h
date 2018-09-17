#ifndef _STDINT_H
#define _STDINT_H
// #include "cs_types.h"
// #include "unistd.h"

#ifndef __INT8_T__
typedef char        int8_t;
#define __INT8_T__
#endif

#ifndef __INT16_T__
typedef short       int16_t;
#define __INT16_T__
#endif

#ifndef __INT32_T__
typedef int         int32_t;
#define __INT32_T__
#endif

#ifndef __UINT8_T__
typedef unsigned char   uint8_t;
#define __UINT8_T__
#endif

#ifndef __UINT16_T__
typedef unsigned short  uint16_t;
#define __UINT16_T__
#endif

#ifndef __UINT32_T__
typedef unsigned int    uint32_t;
#define __UINT32_T__
#endif


#ifndef __U8_T__
typedef unsigned char u8_t;
#define __U8_T__
#endif

#ifndef __U16_T__
typedef unsigned short u16_t;
#define __U16_T__
#endif

#ifndef __U32_T__
typedef unsigned int u32_t;
#define __U32_T__
#endif

#ifndef __S8_T__
typedef signed char s8_t;
#define __S8_T__
#endif

#ifndef __S16_T__
typedef signed short s16_t;
#define __S16_T__
#endif

#ifndef __S32_T__
typedef signed int s32_t;
#define __S32_T__
#endif

#ifndef __INT64_T__
#define __INT64_T__
typedef long long   int64_t;
#endif
#ifndef __UINT64_T__
#define __UINT64_T__
typedef unsigned long long  uint64_t;
#endif
#ifndef __S64_T__
#define __S64_T__
typedef int64_t   s64_t;
#endif
#ifndef __U64_T__
#define __U64_T__
typedef uint64_t  u64_t;
#endif

#ifndef INT8_MIN
#define INT8_MIN       (-0x7f-1)
#endif

#ifndef INT8_MAX
#define INT8_MAX       0x7f
#endif

#ifndef UINT8_MAX
#define UINT8_MAX       0xff
#endif

#ifndef INT16_MIN
#define INT16_MIN       (-0x7fff-1)
#endif

#ifndef INT16_MAX
#define INT16_MAX       0x7fff
#endif

#ifndef UINT16_MAX
#define UINT16_MAX       0xffff
#endif

#ifndef INT32_MIN
#define INT32_MIN       (-0x7fffffff-1)
#endif

#ifndef INT32_MAX
#define INT32_MAX       0x7fffffff
#endif

#ifndef UINT32_MAX
#define UINT32_MAX      0xffffffff
#endif

#ifndef INT64_MIN
#define INT64_MIN       (-0x7fffffffffffffffLL-1)
#endif

#ifndef INT64_MAX
#define INT64_MAX	0x7fffffffffffffffL
#endif

#ifndef UINT64_MAX
#define UINT64_MAX 	0xffffffffffffffffUL
#endif

#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST64_MIN  INT64_MIN
#define INT_LEAST8_MAX   INT8_MAX
#define INT_LEAST16_MAX  INT16_MAX
#define INT_LEAST32_MAX  INT32_MAX
#define INT_LEAST64_MAX  INT64_MAX
#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MIN    INT8_MIN
#define INT_FAST16_MIN   INT32_MIN
#define INT_FAST32_MIN   INT32_MIN
#define INT_FAST64_MIN   INT64_MIN
#define INT_FAST8_MAX    INT8_MAX
#define INT_FAST16_MAX   INT32_MAX
#define INT_FAST32_MAX   INT32_MAX
#define INT_FAST64_MAX   INT64_MAX
#define UINT_FAST8_MAX   UINT8_MAX
#define UINT_FAST16_MAX  UINT32_MAX
#define UINT_FAST32_MAX  UINT32_MAX
#define UINT_FAST64_MAX  UINT64_MAX

typedef unsigned int   intptr_t;


typedef signed char        int_least8_t;
typedef short              int_least16_t;
typedef int                int_least32_t;
typedef long long          int_least64_t;
typedef unsigned char      uint_least8_t;
typedef unsigned short     uint_least16_t;
typedef unsigned int       uint_least32_t;
typedef unsigned long long uint_least64_t;

typedef signed char        int_fast8_t;
typedef int                int_fast16_t;
typedef int                int_fast32_t;
typedef long long          int_fast64_t;
typedef unsigned char      uint_fast8_t;
typedef unsigned int       uint_fast16_t;
typedef unsigned int       uint_fast32_t;
typedef unsigned long long uint_fast64_t;



#endif
