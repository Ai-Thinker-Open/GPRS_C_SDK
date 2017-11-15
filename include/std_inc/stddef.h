
#ifndef STDDEF_H
#define    STDDEF_H
#include "cs_types.h"
#ifndef _PTRDIFF_T_DEFINED
typedef int ptrdiff_t;
#define PTRDIFF(p1, p2, type)   ((p1) - (p2))
#define _PTRDIFF_T_DEFINED
#endif

#endif    /* STDDEF_H */
