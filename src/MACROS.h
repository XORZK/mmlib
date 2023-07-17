#ifndef MACROS_H
#define MACROS_H

#pragma once
#include <stdint.h>
#include <stdexcept>

#define MAX(x,y) ((x > y) ? x : y)
#define MIN(x,y) ((x < y) ? x : y)

template <typename T>
T gcd(T a, T b) {
    return (b == 0 ? a : gcd(b, a % b));
}

// Returns the linear equation between two points: { m, b } 
template <typename T>
double* linear_eq(T x1, T y1, T x2, T y2) {
    double m = static_cast<double>((y2 - y1))/(x2 - x1);
    double b = y1 - (m*x1);
    return new double[]{ m, b };
}

// Given y, find x.
template <typename T>
double interp(T x1, T y1, T x2, T y2, T y3) {
    // Vertical line.
    if (x1 == x2) {
        return x1;
    }

    // y = mx + b
    double m = static_cast<double>((y2-y1))/(x2-x1);
    double b = y1 - (m*x1);

    return (y3-b)/m;
}

#endif
