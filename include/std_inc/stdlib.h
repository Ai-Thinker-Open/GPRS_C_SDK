
#ifndef  STDLIB_H
#define  STDLIB_H
#ifdef __cplusplus
extern "C" {
#endif

#include "cs_types.h"
#include "stddef.h"
#include "sdk_init.h"

/* Standard atoi() function. Work as the libc one. */
int          atoi(const char *s);

#define  atol        CSDK_FUNC(atol)
#define  atoll       CSDK_FUNC(atoll)
#define  atox        CSDK_FUNC(atox)
#define  atof        CSDK_FUNC(atof)
#define  itoa        CSDK_FUNC(itoa)
#define  gcvt        CSDK_FUNC(gcvt)

long          strtol(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);

/* Standard random functions, work as the libc ones. */
#define RAND_MAX        32767

#define  rand         CSDK_FUNC(rand)
#define  srand        CSDK_FUNC(srand)

typedef INT STD_COMPAR_FUNC_T (const VOID*, const VOID*);

PUBLIC VOID  qsort (VOID *baseP, INT32 nElem, INT32 width, STD_COMPAR_FUNC_T *compar);


void *
bsearch (const void *key, const void *base, size_t nmemb, size_t size,
         int (*compar) __P ((const void *, const void *)));



#include "api_os.h"
#define malloc  OS_Malloc
#define free    OS_Free
#define realloc OS_Realloc

#ifdef __cplusplus
}
#endif

#endif /* STDLIB_H */
