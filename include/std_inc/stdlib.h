
#ifndef STDLIB_H
#define    STDLIB_H
#ifdef __cplusplus
extern "C" {
#endif

#include "cs_types.h"
#include "stddef.h"

/* Standard atoi() function. Work as the libc one. */
int          atoi(const char *s);
long         atol(const char *s);
long long    atoll(const char *s);
int          atox(const char *s, int len);
double       atof(const char *s);

unsigned char* itoa(int value, char *str, int radix);
char*          gcvt(double value, int ndigit, char *buf);

long          strtol(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);

/* Standard random functions, work as the libc ones. */
#define RAND_MAX        32767

int  rand (void);
void srand(unsigned int seed);


typedef INT STD_COMPAR_FUNC_T (const VOID*, const VOID*);

PUBLIC VOID  qsort (VOID *baseP, INT32 nElem, INT32 width, STD_COMPAR_FUNC_T *compar);


void *
bsearch (const void *key, const void *base, size_t nmemb, size_t size,
         int (*compar) __P ((const void *, const void *)));


#ifdef __cplusplus
}
#endif

#endif /* STDLIB_H */
