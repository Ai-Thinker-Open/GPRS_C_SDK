

#ifndef SETJMP_H
#define SETJMP_H
#ifdef __cplusplus
extern "C" {
#endif

#include "sdk_init.h"

// int           setjmp (jmp_buf env);
#define setjmp     CSDK_FUNC(setjmp)

// volatile void longjmp (jmp_buf env,  int value);
#define longjmp     CSDK_FUNC(longjmp)

#ifdef __cplusplus
}
#endif
#endif //SETJMP_H

