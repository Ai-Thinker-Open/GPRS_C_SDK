

#ifndef SETJMP_INC_H
#define SETJMP_INC_H
#ifdef __cplusplus
extern "C" {
#endif


#ifdef FPU
typedef unsigned long jmp_buf[22];

#else
typedef unsigned long jmp_buf[12];
#endif

#ifdef __cplusplus
}
#endif
#endif //SETJMP_INC_H

