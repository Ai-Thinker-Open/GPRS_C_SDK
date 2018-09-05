
#ifndef STDDEF_H
#define STDDEF_H
#include "stdint.h"
#include "stdbool.h"
#include "limits.h"

#undef NULL 
// #if defined(__cplusplus)
#define NULL 0
// #else
// #define NULL ((void *)0)
// #endif

#ifndef _USERGEN
#ifndef size_t
typedef unsigned int                size_t;
#endif

#ifndef ssize_t
typedef int                         ssize_t;
#endif
#define SSIZE_MAX INT_MAX
#endif

#define offsetof(TYPE, MEMBER)          ((uint32_t) &((TYPE *)0)->MEMBER)
#define offsetofvar(VAR, MEMBER)        (((uint32_t) &(VAR.MEMBER)) - ((uint32_t) &VAR))

#ifndef _PTRDIFF_T_DEFINED
typedef int ptrdiff_t;
#define PTRDIFF(p1, p2, type)   ((p1) - (p2))
#define _PTRDIFF_T_DEFINED
#endif

#endif    /* STDDEF_H */
