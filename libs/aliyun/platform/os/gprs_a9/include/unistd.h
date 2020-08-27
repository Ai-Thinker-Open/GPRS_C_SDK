#ifndef __UNISTD_H
#define __UNISTD_H

#include "stdio.h"

#define stdout (void *)1
#define stderr (void *)2
#define fflush(...)

#define perror(...) printf(__VA_ARGS__)

#endif
