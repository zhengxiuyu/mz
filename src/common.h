#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#ifdef DEBUG
    #define debug(...)                                                          \
        printf("[mz] %-10s(%-5s): ", __FILE__, __LINE__);                       \
        printf(__VA_ARGS__)
#else
    #define debug(...)
#endif

#define checkmem(x) if (!(x)) goto error

#endif /* end of include guard: COMMON_H */
