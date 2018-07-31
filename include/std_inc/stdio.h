
#ifndef    STDIO_H
#define    STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include <stdarg.h>
#include "sdk_init.h"



#define  sprintf       CSDK_FUNC(sprintf)
#define  snprintf      CSDK_FUNC(snprintf)
#define  sscanf        CSDK_FUNC(sscanf)
#define  vsprintf      CSDK_FUNC(vsprintf)
#define  vsnprintf     CSDK_FUNC(vsnprintf)
#define  vsscanf       CSDK_FUNC(vsscanf)


// int sprintf(char * buf, const char *fmt, ...);
// int snprintf(char * buf, size_t len, const char *fmt, ...);
// int sscanf(const char * buf, const char * fmt, ...);
// int vsprintf(char *buf, const char *fmt, va_list ap);
// int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);
// int vsscanf (const char *fp, const char *fmt0, va_list ap);

#ifdef __cplusplus
}
#endif

#endif /* STDIO_H */



