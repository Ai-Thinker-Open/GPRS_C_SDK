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
