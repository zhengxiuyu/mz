#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef DEBUG
    #define mz_debug(...)                                                          \
        printf("[mz] %-10s(%-5s): ", __FILE__, __LINE__);                       \
        printf(__VA_ARGS__)
#else
    #define debug(...)
#endif

#define mz_checkmem(x) if (!(x)) goto error

#define mz_min(a, b) (a) < (b) ? (a) : (b)
#define mz_max(a, b) (a) > (b) ? (a) : (b)
#define mz_clamp(x, a, b) mz_min(b, mz_max(x, a))

#endif /* end of include guard: COMMON_H */
