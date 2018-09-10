#include "stdlib.h"
#include "api_os.h"

void* malloc(uint32_t size)
{
    return OS_Malloc(size);
}

void free(void* p)
{
    OS_Free(p);
}

void* realloc(void* p, uint32_t size)
{
    return OS_Realloc(p,size);
}

void *calloc(size_t nmemb, size_t size) {
    return malloc(nmemb * size);
}

//not the standard strtod, just impl with atof!!
double strtod(const char *restrict s, char **restrict p)
{
    double r = atof(s);
    *p = NULL;
	return r;
}

