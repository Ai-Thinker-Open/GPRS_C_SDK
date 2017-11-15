

#ifndef SETJMP_H
#define SETJMP_H
#ifdef __cplusplus
extern "C" {
#endif


#ifdef FPU
typedef unsigned long jmp_buf[22];

#else
typedef unsigned long jmp_buf[12];
#endif

int           setjmp (jmp_buf env);

volatile void longjmp (jmp_buf env,  int value);

#ifdef __cplusplus
}
#endif
#endif //SETJMP_H

