#ifndef MATRIX_H
#define MATRIX_H

#pragma once
#include "MACROS.h"
#include "vector.h"
#include "rational.h"
#include <functional>
#include <iostream>
#include <stdint.h>

template <typename T>
class matrix {
    private:
        uint64_t N, M;
        vector<T>* vectors;
    public:
        matrix();

        matrix(uint64_t size);

        matrix(uint64_t rows, uint64_t cols);

        matrix(const matrix<T>& mat_2);

        matrix(const matrix<T>& mat_2, uint64_t rows, uint64_t cols);

        void fill(T value);

        void fill(std::function<T(uint64_t, uint64_t)> f);

        vector<T>& operator[](uint64_t idx) const;

        matrix<double> operator*(const double scalar) const;

        matrix<T> operator+(const matrix<T>& mat_2) const;

        matrix<T> operator-(const matrix<T>& mat_2) const;

        matrix<T> operator*(const matrix<T>& mat_2) const;

        vector<T> operator*(const vector<T>& v2) const;

        template <typename U>
        operator matrix<U>() const;

        matrix<T> transpose() const;

        matrix<T> minor(uint64_t i, uint64_t j) const;

        bool is_lower_triangular() const;

        bool is_upper_triangular() const;

        bool is_triangular() const;

        bool is_square() const;

        uint64_t get_rows() const;

        uint64_t get_cols() const;

        static matrix<T> I(uint64_t size);

        static matrix<T> E(uint64_t size, uint64_t i, uint64_t j, T l);

        static matrix<T> P(uint64_t size, uint64_t i, uint64_t j);

        static matrix<T> P(uint64_t size, vector<uint64_t> swaps);

        static matrix<double> create_projection_matrix(const double b, const double t,
                                                       const double l, const double r,
                                                       const double n, const double f);

        static matrix<double> create_2d_rotation_matrix(double theta = 0, bool deg = true);

        static matrix<double> create_3d_rotation_matrix(double alpha = 0, double beta = 0, double gamma = 0, bool deg = true);

        static matrix<double> create_z_rotation(double alpha, bool deg = true);

        static matrix<double> create_y_rotation(double beta, bool deg = true);
        
        static matrix<double> create_x_rotation(double gamma, bool deg = true);
};

template <typename T> 
std::ostream& operator<<(std::ostream& out, const matrix<T>& mat);

template <typename T>
matrix<T>::matrix() : N(0), M(0), vectors(NULL) {
}

template <typename T>
matrix<T>::matrix(uint64_t size) : N(size), M(size) {
    this->vectors = static_cast<vector<T>*>(malloc(sizeof(vector<T>) * this->N));

    for (uint64_t i = 0; i < this->N; i++) {
        this->vectors[i] = vector<T>::zero(this->N);
    }
}

template <typename T>
matrix<T>::matrix(uint64_t rows, uint64_t cols) : N(rows), M(cols) {
    this->vectors = static_cast<vector<T>*>(malloc(sizeof(vector<T>) * this->N));

    for (uint64_t i = 0; i < this->N; i++) {
        this->vectors[i] = vector<T>::zero(this->M);
    }
}

template <typename T>
matrix<T>::matrix(const matrix<T>& mat_2) : N(mat_2.get_rows()), M(mat_2.get_cols()) {
    this->vectors = static_cast<vector<T>*>(malloc(sizeof(vector<T>) * this->N));

    for (uint64_t i = 0; i < this->N; i++) {
        this->vectors[i] = vector(mat_2[i]);
    }
}

template <typename T>
matrix<T>::matrix(const matrix<T>& mat_2, uint64_t rows, uint64_t cols) : N(rows), M(cols) { 
    this->vectors = static_cast<vector<T>*>(malloc(sizeof(vector<T>) * this->N));

    for (uint64_t i = 0; i < this->N; i++) {
        this->vectors[i] = vector<T>::zero(this->M);
    }

    for (uint64_t i = 0; (i < mat_2.get_rows()) && (i < this->N); i++) {
        for (uint64_t k = 0; (k < mat_2.get_cols()) && (k < this->M); k++) {
            this->vectors[i][k] = mat_2[i][k];
        }
    }
}

template <typename T>
void matrix<T>::fill(T value) {
    for (uint64_t i = 0; i < this->N; i++) {
        this->vectors[i].fill(value);
    }
}

template <typename T>
void matrix<T>::fill(std::function<T(uint64_t, uint64_t)> f) {
    for (uint64_t i = 0; i < this->N; i++) {
        for (uint64_t k = 0; k < this->M; k++) {
            this->vectors[i][k] = f(i, k);
        }
    }
}

template <typename T>
vector<T>& matrix<T>::operator[](uint64_t idx) const {
    assert(idx >= 0 && idx < this->N);

    return this->vectors[idx];
}

template <typename T>
matrix<double> matrix<T>::operator*(const double scalar) const {
    matrix<double> p(this->N, this->M);

    for (uint64_t i = 0; i < this->N; i++) {
        p[i] = (this->vectors[i] * scalar);
    }

    return p;
}

template <typename T>
matrix<T> matrix<T>::operator+(const matrix<T>& mat_2) const {
    assert(this->N == mat_2.get_rows() && 
           this->M == mat_2.get_cols());
    
    matrix<T> s(this->N, this->M);

    for (uint64_t i = 0; i < this->N; i++) {
        s[i] = (this->vectors[i] + mat_2[i]);
    }

    return s;
}

template <typename T>
matrix<T> matrix<T>::operator-(const matrix<T>& mat_2) const {
    assert(this->N == mat_2.get_rows() && 
           this->M == mat_2.get_cols());

    matrix<T> d(this->N, this->M);

    for (uint64_t i = 0; i < this->N; i++) {
        d[i] = (this->vectors[i] - mat_2[i]);
    }

    return d;
}

template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T>& mat_2) const {
    assert(this->M == mat_2.get_rows());

    matrix<T> p(this->N, mat_2.get_cols()), tp = mat_2.transpose();

    for (uint64_t i = 0; i < this->N; i++) {
        for (uint64_t k = 0; k < mat_2.get_cols(); k++) {
            p[i][k] = this->vectors[i] * tp[k];
        }
    }

    return p; 
}

template <typename T>
vector<T> matrix<T>::operator*(const vector<T>& v2) const {
    assert(v2.get_size() == this->M);

    vector<T> prod(this->N);

    for (uint64_t i = 0; i < this->N; i++) {
        prod.push_back(this->vectors[i] * v2);
    }
    
    return prod;
}

template <typename T> 
template <typename U> matrix<T>::operator matrix<U>() const {
    matrix<U> casted(this->N, this->M);

    for (uint64_t i = 0; i < this->N; i++) {
        casted[i] = static_cast<vector<U>>(this->vectors[i]);
    }

    return casted;
}

template <typename T>
matrix<T> matrix<T>::transpose() const {
    matrix<T> t(this->M, this->N);

    for (uint64_t i = 0; i < this->M; i++) {
        for (uint64_t k = 0; k < this->N; k++) {
            t[i][k] = this->vectors[k][i];
        }
    }

    return t;
}

template <typename T>
matrix<T> matrix<T>::minor(uint64_t i, uint64_t j) const {
    matrix<T> mm(this->N-1, this->M-1);

    uint64_t x = 0, y = 0;

    for (uint64_t n = 0; n < this->N; n++) {
        for (uint64_t m = 0; m < this->M; m++) {
            if (m == j || n == i) {
                continue;
            }

            mm[x][y] = this->vectors[n][m];

            x += (y == this->M-2 ? 1 : 0);
            y = (y+1)%(this->M-1);
        }
    }

    return mm;
}

// Checks for Lower Triangular Matrix (L)
template <typename T>
bool matrix<T>::is_lower_triangular() const {
    assert(this->N == this->M);

    for (uint64_t i = 0; i < this->N; i++) {
        for (uint64_t k = i + 1; k < this->N; k++) {
            if (this->vectors[i][k] != static_cast<T>(0)) {
                return false;
            }
        }
    }

    return true;
}

// Checks for Upper Triangular Matrix (U)
template <typename T>
bool matrix<T>::is_upper_triangular() const {
    assert(this->N == this->M);

    for (uint64_t i = 0; i < this->N; i++) {
        for (uint64_t k = 0; k < i; k++) {
            if (this->vectors[i][k] != static_cast<T>(0)) {
                return false;
            }
        }
    }

    return true;
}

// Checks for a triangular matrix
template <typename T>
bool matrix<T>::is_triangular() const {
    return (this->is_lower_triangular() || this->is_upper_triangular());
}

template <typename T>
bool matrix<T>::is_square() const {
    return (this->M == this->N);
}

template <typename T>
uint64_t matrix<T>::get_cols() const {
    return this->M;
}

template <typename T>
uint64_t matrix<T>::get_rows() const {
    return this->N;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const matrix<T>& mat) {
    for (int i = 0; i < mat.get_rows(); i++) {
        mat[i].set_size(mat.get_cols());
        out << mat[i] << (i < mat.get_rows() - 1 ? "\n" : "");
    }

    return out;
}
 

template <typename T>
matrix<T> matrix<T>::I(uint64_t size) {
    matrix iden(size);

    for (uint64_t i = 0; i < size; i++) {
        iden[i][i] = static_cast<T>(1);
    }

    return iden;
}

template <typename T>
matrix<T> matrix<T>::E(uint64_t size, uint64_t i, uint64_t j, T l) {
    assert(i >= 0 && i < size && j >= 0 && j < size);
    matrix elim = matrix<T>::I(size);

    elim[i][j] = l;

    return elim;
}

template <typename T>
matrix<T> matrix<T>::P(uint64_t size, uint64_t i, uint64_t j) {
    assert(i >= 0 && i < size && j >= 0 && j < size);
    matrix perm(size);

    for (uint64_t k = 0; k < size; k++) {
        perm[k][(k == i ? j : (k == j) ? i : k)] = static_cast<T>(1);
    }

    return perm;
}

template <typename T>
matrix<T> matrix<T>::P(uint64_t size, vector<uint64_t> swaps) {
    assert(swaps.get_size() == size);
    matrix perm(size);

    for (uint64_t k = 0; k < size; k++) {
        perm[k][swaps[k]] = static_cast<T>(1);
    }

    return perm;
}

// Finds det(A) using Cofactor Expansion on the 1st Row.
// Can be solved recursively.
template <typename T>
double det(matrix<T> mat) {
    assert(mat.is_square());

    double d = 1;

    if (mat.is_triangular()) {
        for (uint64_t k = 0; k < mat.get_rows(); k++) {
            d *= mat[k][k];
        }

        return d;
    }

    if (mat.get_rows() == 1) {
        return mat[0][0];
    }

    d = 0;

    uint64_t i = 0;
    
    for (uint64_t j = 0; j < mat.get_rows(); j++) {
        d += mat[i][j] * pow(-1, i + j) * det(mat.minor(i, j));
    }

    return d;
}

template <typename T>
matrix<double> invert(matrix<T> mat) {
    assert(mat.is_square());

    matrix<double> modified = static_cast<matrix<double>>(mat), I = matrix<double>::I(mat.get_rows()), P = matrix<double>::I(mat.get_rows());

    if (!det(mat)) {
        return modified;
    }

    uint64_t rows = modified.get_rows();

    // Creates zeroes under the pivot 
    for (uint64_t i = 0; i < rows; i++) {
        double pivot = modified[i][i];

        if (!pivot) {
            uint64_t row_swap = i+1;

            while (!modified[row_swap][i] && row_swap < rows) {
                row_swap++;
            }

            matrix<double> row_exchange = matrix<double>::P(rows, row_swap, i);
            modified = row_exchange * modified;
            P = row_exchange * modified;
            pivot = modified[i][i];
        }

        for (uint64_t k = i+1; k < rows; k++) {
            matrix<double> E = matrix<double>::E(rows, k, i, -1 * modified[k][i] / pivot);
            modified = E * modified;
            I = E * I;
        }
    }

    // Creates zeros over each pivot
    for (int64_t i = rows-1; i >= 0; i--) {
        double pivot = modified[i][i];
        for (int64_t k = i-1; k >= 0; k--) {
            matrix<double> E = matrix<double>::E(rows, k, i, -1 * modified[k][i] / pivot);
            modified = E * modified;
            I = E * I;
        }
    }

    // Divides each row by the pivot
    for (uint64_t i = 0; i < rows; i++) {
        double pivot = modified[i][i];
        modified[i] /= pivot;
        I[i] /= pivot;
    }

    return P * I;
}

/* Bottom, Top, Left, Right, Near, and Far Coordinates. */
template <typename T>
matrix<double> matrix<T>::create_projection_matrix(const double b, const double t,
                                                   const double l, const double r,
                                                   const double n, const double f) {
    // initializing the projection matrix
    matrix<double> P(4, 4);

    P[0] = vector<double>(new double[]{(2*n)/(r-l),0,(r+l)/(r-l),0}, 4);

    P[1] = vector<double>(new double[]{0,(2*n)/(t-b),(t+b)/(t-b),0}, 4);

    P[2] = vector<double>(new double[]{0,0,-(f+n)/(f-n),(-2*f*n)/(f-n)}, 4);

    P[3] = vector<double>(new double[]{0,0,-1,0}, 4);

    return P;
}

template <typename T>
matrix<double> matrix<T>::create_2d_rotation_matrix(double theta, bool deg) {
    if (deg) {
        theta *= M_PI/180;
    }

    matrix<double> R(2,2);

    R[0] = vector<double>(new double[]{cos(theta), -sin(theta)});
    R[1] = vector<double>(new double[]{sin(theta), cos(theta)});

    return R;
}

// yaw: z, pitch: y, roll: x
// degrees on default --> conversion is required
template <typename T>
matrix<double> matrix<T>::create_3d_rotation_matrix(double alpha, double beta, double gamma, bool deg) {
    matrix<double> Rz = matrix<T>::create_z_rotation(alpha, deg),
                   Ry = matrix<T>::create_y_rotation(beta, deg),
                   Rx = matrix<T>::create_x_rotation(gamma, deg);

    return (Rz * Ry * Rx);
}

template <typename T>
matrix<double> matrix<T>::create_z_rotation(double alpha, bool deg) {
    if (deg) {
        alpha *= M_PI/180;
    }

    matrix<double> Rz(3,3);
    Rz[0] = vector<double>(new double[]{cos(alpha), -sin(alpha), 0}, 3);
    Rz[1] = vector<double>(new double[]{sin(alpha), cos(alpha), 0}, 3);
    Rz[2] = vector<double>(new double[]{0, 0, 1}, 3);

    return Rz;
}

template <typename T>
matrix<double> matrix<T>::create_y_rotation(double beta, bool deg) {
    if (deg) {
        beta *= M_PI/180;
    }

    matrix<double> Ry(3,3);

    Ry[0] = vector<double>(new double[]{cos(beta), 0, sin(beta)}, 3);
    Ry[1] = vector<double>(new double[]{0, 1, 0}, 3);
    Ry[2] = vector<double>(new double[]{-sin(beta), 0, cos(beta)}, 3);

    return Ry;
}

template <typename T>
matrix<double> matrix<T>::create_x_rotation(double gamma, bool deg) {
    if (deg) {
        gamma *= M_PI/180;
    }

    matrix<double> Rx(3, 3);

    Rx[0] = vector<double>(new double[]{1, 0, 0}, 3);
    Rx[1] = vector<double>(new double[]{0, cos(gamma), -sin(gamma)}, 3);
    Rx[2] = vector<double>(new double[]{0, sin(gamma), cos(gamma)}, 3);

    return Rx;
}

#endif
