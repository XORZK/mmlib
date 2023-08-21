#ifndef MACROS_H
#define MACROS_H

#pragma once
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdexcept>

/// @file

/** Returns the maximum between two values (x,y) */
#define MAX(x,y) ((x > y) ? x : y)

/** Returns the minimum between two values (x,y) */
#define MIN(x,y) ((x < y) ? x : y)

/** Returns the GCD between two values (a,b) using the Euclidian Algorithm. 
 * @param a The first integer argument into the GCD function.
 * @param b The second integer argument into the GCD function.
 * @return The GCD of (a,b) which is the largest positive integer N such that (a % N) = (b % N) = 0.
 * */
template <typename T>
inline T gcd(T a, T b) {
    return (b == 0 ? a : gcd(b, a % b));
}

/** Performs a linear interpolation, and computes the corresponding x-value to a y-value on a line.
 * Linear interpolation is used to fill triangles.
 * @param x1 The x-value of the first pair of coordinates (x1, y1).
 * @param y1 The y-value of the first pair of coordinates (x1, y1).
 * @param x2 The x-value of the second pair of coordinates (x2, y2).
 * @param y2 The y-value of the second pair of coordinates (x2, y2).
 * @param y3 The y-value of the third pair of coordinates (x3, y3).
 * @return The unknown x3 value corresponding to y3 on the line formed by (x1, y1) and (x2, y2).
 * */
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

/** Computes the distance between two pairs of pionts (x1, y1) and (x2, y2). 
 * The distance between \f$(x_1, y_1)\f$ and \f$(x_2, y_2)\f$ is \f$sqrt{(x_2-x_1)^2 + (y_2-y_1)^2}\f$.
 * @param x1 The x-value of the first pair of coordinates (x1, y1).
 * @param y1 The y-value of the first pair of coordinates (x1, y1).
 * @param x2 The x-value of the second pair of coordinates (x2, y2).
 * @param y2 The y-value of the second pair of coordinates (x2, y2).
 * @return The distance between the two pair of points (x1, y1) and (x2, y2).
 * */
template <typename T>
inline double distance(T x1, T y1, T x2, T y2) {
    return sqrt(pow((x1-x2), 2) + pow((y2-y1), 2));
}

/** Computes the modulus of two integers N and M.
 * If \f$N<0\f$, we add M to N until it's positive.
 * @param N The number to find the remainder of.
 * @param M The modulus.
 * @return The value of \f$N \pmod M\f$.
 * */
template <typename T>
inline T mod(T N, T M) {
    assert(M != 0);

    while (N < 0) {
        N += M;
    }
    return (N % M);
}

/** Checks whether or not three pairs of points \f$(x_1, y_1)\f$, \f$(x_2, y_2)\f$, and \f$(x_3, y_3)\f$ are collinear or not.
 * \f$(x_1, y_1), (x_2, y_2), (x_3, y_3)\f$ are collinear if the slope between \f$(x_1, y_1), (x_2, y_2)\f$ and the slope between \f$(x_2, y_2), (x_3, y_3) \f$ are the same. 
 * Therefore, we check: \f$\frac{(x_3-x_2)}{(y_3-y_2)} = \frac{(x_2-x_1)}{(y_2-y_1)}.\f$
 * @param x1 The x-value of the first pair of coordinates (x1, y1).
 * @param y1 The y-value of the first pair of coordinates (x1, y1).
 * @param x2 The x-value of the second pair of coordinates (x2, y2).
 * @param y2 The y-value of the second pair of coordinates (x2, y2).
 * @param x3 The x-value of the third pair of coordinates (x3, y3).
 * @param y3 The y-value of the third pair of coordinates (x3, y3).
 * @return A boolean value denoting whether or not the three points are collinear.
 * */
template <typename T>
inline bool collinear(T x1, T y1, T x2, T y2, T x3, T y3) {
    return ((x3-x2)*(y2-y1)) == ((x2-x1)*(y3-y2));
}

#endif
