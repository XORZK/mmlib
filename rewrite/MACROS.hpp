#ifndef MACROS_HPP
#define MACROS_HPP

#pragma once
#include <stdint.h>

#define MAX(x,y) ((x>y)?x:y)
#define MIN(x,y) ((x<y)?x:y)

inline int64_t mod(int64_t N, int64_t M) {
    while (N < 0)
        N += M;

    return (N%M);
}

#endif
