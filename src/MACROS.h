#ifndef MACROS_H
#define MACROS_H

#pragma once
#include <math.h>
#include <stdint.h>
#include <stdexcept>

#define MAX(x,y) ((x > y) ? x : y)
#define MIN(x,y) ((x < y) ? x : y)

template <typename T>
inline T gcd(T a, T b) {
    return (b == 0 ? a : gcd(b, a % b));
}

// Returns the linear equation between two points: { m, b } 
template <typename T>
inline double* linear_eq(T x1, T y1, T x2, T y2) {
    double m = static_cast<double>((y2 - y1))/(x2 - x1);
    double b = y1 - (m*x1);
    return new double[]{ m, b };
}

// Given y, find x.
template <typename T>
inline double interp(T x1, T y1, T x2, T y2, T y3) {
    // Vertical line.
    if (x1 == x2) {
        return x1;
    }

    // y = mx + b
    double m = static_cast<double>((y2-y1))/(x2-x1);
    double b = y1 - (m*x1);

    return (y3-b)/m;
}

template <typename T>
inline double distance(T x1, T y1, T x2, T y2) {
    return sqrt(pow((x1-x2), 2) + pow((y2-y1), 2));
}

// [Ax(By - Cy) + Bx(Cy - Ay) + Cx(Ay - By)]/2
template <typename T>
inline double signed_triangle_area(T x1, T y1, T x2, T y2, T x3, T y3) {
    return 0.5 * (x1*(y2 - y3) + x2(y3 - y1) + x3(y1 - y2));
}

template <typename T>
inline T mod(T N, T M) {
    while (N < 0) {
        N += M;
    }
    return (N % M);
}

// Collinear if (y2-y1)/(x2-x1) = (y3-y2)/(x3-x2)
// => (x3-x2)(y2-y1) = (x2-x1)(y3-y2)
template <typename T>
inline bool collinear(T x1, T y1, T x2, T y2, T x3, T y3) {
    return ((x3-x2)*(y2-y1)) == ((x2-x1)*(y3-y2));
}

#endif
