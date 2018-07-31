
#include "assert.h"

void Assert(bool valid,const char* fmt)
{
    if(!valid)
        __assert(fmt);
}

