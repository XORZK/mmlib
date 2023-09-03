#ifndef MACROS_HPP
#define MACROS_HPP

#pragma once
#include <stdint.h>

#define MAX(x,y) ((x>y)?x:y)
#define MIN(x,y) ((x<y)?x:y)
#define ABS(x) (x < 0 ? -x : x)

inline int64_t mod(int64_t N, int64_t M) {
    while (N < 0)
        N += M;

    return (N%M);
}

template <typename T> 
inline double interp(T x1, T y1, T x2, T y2, T y3) {
    if (x2 == x1)
        return x1;

    // y = mx + b, m = (y2-y1)/(x2-x1) 
    // B = y1 - m * x1
    double M = ((double) (y2 - y1))/(x2 - x1);
    double B = y1 - M * x1;

    return (y3 - B)/M;
}

inline int64_t fact(int64_t N) {
	int64_t F = 1;

	for (int64_t k = 1; k <= N; k++)
		F *= k;

	return F;
}

inline int64_t comb(int64_t N, int64_t k) {
	return (fact(N))/(fact(N-k) * fact(k));
}

#endif
