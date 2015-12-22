#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
    #define M_PI        3.14159265359
#endif

#ifdef DEBUG
    #define mz_debug(...)                                                       \
        do {                                                                    \
            printf("[mz] %-10s(%-5s): ", __FILE__, __LINE__);                   \
            printf(__VA_ARGS__)                                                 \
        } while(0)
    #define mz_debug_if(x, ...)                                                 \
        if ((x))                                                                \
            mz_debug(__VA_ARGS__)
#else
    #define mz_debug(...)
    #define mz_debug_if(x, ...)
#endif

#define mz_calloc(x, n, s)                                                      \
    if (!((x) = calloc((n), (s))))                                              \
        goto error

#define mz_printif(x, ...) if ((x)) printf(__VA_ARGS__)
#define mz_checkmem(x) if (!(x)) goto error
#define mz_min(a, b) (a) < (b) ? (a) : (b)
#define mz_max(a, b) (a) > (b) ? (a) : (b)
#define mz_clamp(x, a, b) mz_min((b), mz_max((x), (a)))
#define mz_inrange(x, a, b) ((x) >= (a) && (x) <= (b))

#define mz_set(v, a, b)                                                         \
    (v)[0] = (a);                                                               \
    (v)[1] = (b)
#define mz_sub(c, a, b)                                                         \
    (c)[0] = (a)[0] - (b)[0];                                                   \
    (c)[1] = (a)[1] - (b)[1]
#define mz_dot(a, b)                                                            \
    ((a)[0] * (b)[0] + (a)[1] * (b)[1])

#endif /* end of include guard: COMMON_H */
