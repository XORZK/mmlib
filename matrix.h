#ifndef MATRIX_H
#define MATRIX_H

#pragma once
#include "vector.h"
#include "rational.h"
#include <iostream>
#include <stdint.h>

template <typename T> 
class matrix {
    static_assert((std::is_arithmetic<T>::value || std::is_same<T, rat>::value),
                  "Matrix must contain arithmetic types.");
    private:
        uint64_t N, M;
        vector<T>* vectors;

    public:
        matrix();

        matrix(uint64_t size);

        matrix(uint64_t rows, uint64_t cols);

        void fill(T value);

        vector<T>& operator[](uint64_t idx) const;

        matrix<double> operator*(const double scalar) const;

        matrix<T> operator+(const matrix<T>& mat_2) const;

        matrix<T> operator-(const matrix<T>& mat_2) const;

        matrix<T> operator*(const matrix<T>& mat_2) const;

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
} ;

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
void matrix<T>::fill(T value) {
    for (uint64_t i = 0; i < this->N; i++) {
        this->vectors[i].fill(value);
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

    matrix<double> modified = static_cast<matrix<double>>(mat), I = matrix<double>::I(mat.get_rows());

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

            matrix<double> P = matrix<double>::P(rows, row_swap, i);
            modified = P * modified;
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

    return I;
}

#endif
