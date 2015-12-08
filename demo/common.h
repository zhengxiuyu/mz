#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <assert.h>

#define warn(...)                                                             \
    do {                                                                      \
        printf("%s (%d) : ", __FILE__, __LINE__);                             \
        printf(__VA_ARGS__);                                                  \
    } while(0)

#endif /* end of include guard: COMMON_H */
