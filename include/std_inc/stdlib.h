
#ifndef  STDLIB_H
#define  STDLIB_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"
#include "limits.h"
#include "sdk_init.h"
#include "ctype.h"

/* Standard atoi() function. Work as the libc one. */
int          atoi(const char *s);

#define  atol        CSDK_FUNC(atol)
#define  atoll       CSDK_FUNC(atoll)
#define  atox        CSDK_FUNC(atox)
#define  atof        CSDK_FUNC(atof)
#define  itoa        CSDK_FUNC(itoa)
#define  gcvt        CSDK_FUNC(gcvt)
extern double strtod(const char *restrict s, char **restrict p);
extern int strtol(const char *nptr, char **endptr, int base);
extern unsigned int strtoul(const char *nptr, char **endptr, int base);

// long          strtol(const char *nptr, char **endptr, int base);
// unsigned long strtoul(const char *nptr, char **endptr, int base);

//!!not the standard strtod, just impl with atof!!
double        strtod(const char *restrict s, char **restrict p);

/* Standard random functions, work as the libc ones. */
#define RAND_MAX        32767

#define  rand         CSDK_FUNC(rand)
#define  srand        CSDK_FUNC(srand)

typedef int STD_COMPAR_FUNC_T (const void*, const void*);

void  qsort (void *baseP, int32_t nElem, int32_t width, STD_COMPAR_FUNC_T *compar);


void *
bsearch (const void *key, const void *base, size_t nmemb, size_t size,
         int (*compar)(const void *, const void *));




void* malloc(uint32_t size);
void free(void* p);
void* realloc(void* p, uint32_t size);
void *calloc(size_t nmemb, size_t size);


#ifdef __cplusplus
}
#endif

#endif /* STDLIB_H */
