
#ifndef _ASSERT_H_
#define _ASSERT_H_

#  include "sdk_init.h"


#define __assert     CSDK_FUNC(__assert)

#define assert(boolcondition) { if(!(boolcondition)) __assert( \
    "ASSERTION FAILED"); }

void Assert(bool valid,const char* fmt);

#endif
