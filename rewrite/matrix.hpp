#ifndef MATRIX_HPP
#define MATRIX_HPP

#pragma once
#include "vector.hpp"

#include <assert.h>
#include <iostream>
#include <stdint.h>
#include <functional>

template <typename T> class matrix {
    static_assert(std::is_arithmetic<T>::value, 
                  "matrix<T> must hold arithmetic types");

    private:
        // Rows, Cols
        int64_t N, M;
        vector<T>* mat_rows;

    public:
        matrix();

        matrix(int64_t size);

        matrix(int64_t R, int64_t C);

        matrix(const matrix<T>& m2);

        matrix(const matrix<T>& m2, int64_t R, int64_t C);

        void fill(T value);

        void fill(std::function<T(int64_t, int64_t)> func);

        vector<T>& operator[](int64_t idx) const;

        matrix<T> operator*(const matrix<T>& m2) const;

        matrix<T> operator*(const T scalar) const;

        matrix<T> operator/(const T scalar) const;

        matrix<T> operator+(const T value) const;

        matrix<T> operator-(const T value) const;

        matrix<T> operator+(const matrix<T> m2) const;

        matrix<T> operator-(const matrix<T> m2) const;

        void operator*=(const T scalar);

        void operator/=(const T scalar);

        void operator+=(const matrix<T> m2);

        void operator-=(const matrix<T> m2);

        void operator+=(const T value);

        void operator-=(const T value);

        bool operator==(const matrix<T>& m2) const;

        template <typename U>
        operator matrix<U>() const;

        matrix<T> transpose() const;

        matrix<T> minor(int64_t i, int64_t j) const;

        bool is_lower_triangular() const;

        bool is_upper_triangular() const;

        bool is_triangular() const;

        bool is_square_matrix() const; 

        int64_t rows() const;

        int64_t cols() const;

        static matrix<T> I(int64_t size);

        static matrix<T> E(int64_t size, int64_t i, int64_t j, T l);

        static matrix<T> P(int64_t size, int64_t i, int64_t j);

        static matrix<T> P(int64_t size, vector<int64_t> swaps);
};

template <typename T> 
matrix<T>::matrix() : N(0), M(0), mat_rows(nullptr) {}

template <typename T> 
matrix<T>::matrix(int64_t size) : N(size), M(size) {
    this->mat_rows = static_cast<vector<T>*>(malloc(N * sizeof(vector<T>)));

    for (int64_t k = 0; k < N; k++) {
        this->mat_rows[k] = vector<T>::zero(M);
    }
}

template <typename T> 
matrix<T>::matrix(int64_t rows, int64_t cols) : N(rows), M(cols) {
    this->mat_rows = static_cast<vector<T>*>(malloc(N * sizeof(vector<T>)));

    for (int64_t k = 0; k < N; k++) {
        this->mat_rows[k] = vector<T>::zero(M);
    }
}

template <typename T> 
matrix<T>::matrix(const matrix<T>& m2) : N(m2.rows()), M(m2.cols()) {
    this->mat_rows = static_cast<vector<T>*>(malloc(N * sizeof(vector<T>)));

    for (int64_t k = 0; k < N; k++) {
        vector copy(m2[k]);

        this->mat_rows[k] = copy;
    }
}

template <typename T> 
matrix<T>::matrix(const matrix<T>& m2, int64_t R, int64_t C) : N(R), M(C) {
    this->mat_rows = static_cast<vector<T>*>(malloc(N * sizeof(vector<T>)));

    for (int64_t k = 0; k < N; k++) {
        this->mat_rows[k] = vector<T>::zero(M);
    }

    for (int64_t i = 0; ((i < N) && (i < m2.rows())); i++) {
        for (int64_t k = 0; ((k < M) && (k < m2.cols())); k++) {
            this->mat_rows[i][k] = m2[i][k];
        }
    }
}

template <typename T> 
void matrix<T>::fill(T value) {
    for (int64_t i = 0; i < N; i++) {
        this->mat_rows[i].fill(value);
    }
}

template <typename T> 
void matrix<T>::fill(std::function<T(int64_t, int64_t)> func) {
    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = 0; k < M; k++) {
            this->mat_rows[i][k] = func(i,k);
        }
    }
}

template <typename T> 
vector<T>& matrix<T>::operator[](int64_t idx) const {
    assert(("Out of Bounds Error: matrix index out of range", (idx >= 0 && idx < N)));

    return this->mat_rows[idx];
}

template <typename T> 
matrix<T> matrix<T>::operator*(const matrix<T>& m2) const {
    assert(("Rows and Columns must match to perform matrix multiplication", (M == m2.rows())));

    matrix<T> P(N, m2.rows()), TP = m2.transpose();

    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = 0; k < m2.cols(); k++) {
            P[i][k] = this->mat_rows[i] * TP[k];
        }
    }

    return P;
}

template <typename T> 
matrix<T> matrix<T>::operator*(const T scalar) const {
    matrix<T> prod(N, M);

    for (int64_t k = 0; k < N; k++) {
        prod[k] = (this->mat_rows[k] * scalar);
    }

    return prod;
}

template <typename T> 
matrix<T> matrix<T>::operator/(const T scalar) const {
    assert(("Math Error: Cannot divide by 0", scalar != 0));

    matrix<T> div(N, M);

    for (int64_t k = 0; k < N; k++) {
        div[k] = (this->mat_rows[k] / scalar);
    }

    return div;
}

template <typename T> 
matrix<T> matrix<T>::operator+(const T value) const {
    matrix<T> sum(N, M);

    for (int64_t k = 0; k < N; k++) {
        sum[k] = (this->mat_rows[k] + value);
    }

    return sum;
}

template <typename T> 
matrix<T> matrix<T>::operator-(const T value) const {
    matrix<T> diff(N, M);

    for (int64_t k = 0; k < N; k++) {
        diff[k] = (this->mat_rows[k] - value);
    }

    return diff;
}

template <typename T> 
matrix<T> matrix<T>::operator+(const matrix<T> m2) const {
    assert(("Matrices must be the same size to perform matrix addition",
            (N == m2.rows() && M == m2.cols())));

    matrix<T> sum(N, M);

    for (int64_t k = 0; k < N; k++) {
        sum[k] = (this->mat_rows[k] + m2[k]);
    }

    return sum;
}

template <typename T> 
matrix<T> matrix<T>::operator-(const matrix<T> m2) const {
    assert(("Matrices must be the same size to perform matrix subtraction",
            (N == m2.rows() && M == m2.cols())));

    matrix<T> diff(N, M);

    for (int64_t k = 0; k < N; k++) {
        diff[k] = (this->mat_rows[k] - m2[k]);
    }

    return diff;
}

template <typename T> 
void matrix<T>::operator*=(const T scalar) {
    for (int64_t k = 0; k < N; k++) {
        this->mat_rows[k] *= scalar;
    }
}

template <typename T> 
void matrix<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0", scalar != 0));

    for (int64_t k = 0; k < N; k++) {
        this->mat_rows[k] /= scalar;
    }
}

template <typename T> 
void matrix<T>::operator+=(const T value) {
    for (int64_t k = 0; k < N; k++) {
        this->mat_rows[k] += value;
    }
}

template <typename T> 
void matrix<T>::operator-=(const T value) {
    for (int64_t k = 0; k < N; k++) {
        this->mat_rows[k] -= value;
    }
}

template <typename T> 
void matrix<T>::operator+=(const matrix<T> m2) {
    assert(("Matrices must be the same size to perform matrix addition",
            (N == m2.rows() && M == m2.cols())));

    for (int64_t k = 0; k < N; k++) {
        this->mat_rows[k] += m2[k];
    }
}

template <typename T> 
void matrix<T>::operator-=(const matrix<T> m2) {
    assert(("Matrices must be the same size to perform matrix subtraction",
            (N == m2.rows() && M == m2.cols())));

    for (int64_t k = 0; k < N; k++) {
        this->mat_rows[k] -= m2[k];
    }
}

template <typename T> 
bool matrix<T>::operator==(const matrix<T>& m2) const {
    if (this->N != m2.rows() || this->M != m2.cols()) 
        return false;
    
    for (int64_t k = 0; k < N; k++) {
        if (this->mat_rows[k] != m2[k])
            return false;
    }

    return true;
}

template <typename T> template <typename U> 
matrix<T>::operator matrix<U>() const {
    matrix<U> casted(N, M);

    for (int64_t k = 0; k < N; k++) {
        casted[k] = static_cast<vector<U>>(this->mat_rows[k]);
    }

    return casted;
}

template <typename T> 
matrix<T> matrix<T>::transpose() const {
    matrix<T> TP(M, N);

    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = 0; k < M; k++) {
            TP[k][i] = this->mat_rows[i][k];
        }
    }

    return TP;
}

template <typename T> 
matrix<T> matrix<T>::minor(int64_t i, int64_t j) const {
    matrix<T> MM(N-1, M-1);
    
    int64_t px = 0, py = 0;

    for (int64_t a = 0; a < N; a++) {
        for (int64_t b = 0; b < M; b++) {
            if (a == i || b == j) 
                continue;

            MM[px][py] = this->mat_rows[a][b];

            px += (py == (M-2) ? 1 : 0);
            py = (py+1) % (M-1);
        }
    }

    return MM;
}

// Everything above diagonal is 0.
template <typename T> 
bool matrix<T>::is_lower_triangular() const {
    if (N != M) 
        return false;

    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = i+1; k < M; k++) {
            if (this->mat_rows[i][k] != static_cast<T>(0))
                return false;
        }
    }

    return true;
}

// Everything under diagonal is 0.
template <typename T> 
bool matrix<T>::is_upper_triangular() const {
    if (N != M) 
        return false;

    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = 0; k < i; k++) {
            if (this->mat_rows[i][k] != static_cast<T>(0)) 
                return false;
        }
    }

    return true;
}

template <typename T> 
bool matrix<T>::is_triangular() const {
    return (this->is_lower_triangular() || this->is_upper_triangular());
}

template <typename T> 
bool matrix<T>::is_square_matrix() const {
    return (this->M == this->N);
}

template <typename T> 
int64_t matrix<T>::rows() const { return this->N; }

template <typename T> 
int64_t matrix<T>::cols() const { return this->M; }

template <typename T> 
matrix<T> matrix<T>::I(int64_t size) {
    matrix<T> I(size, size);

    for (int64_t k = 0; k < size; k++) {
        I[k][k] = static_cast<T>(1);
    }

    return I;
}

template <typename T> 
matrix<T> matrix<T>::E(int64_t size, int64_t i, int64_t j, T l) {
    matrix E = matrix<T>::I(size);

    E[i][j] = l;

    return E;
}

template <typename T> 
matrix<T> matrix<T>::P(int64_t size, int64_t i, int64_t j) {
    matrix P(size);

    for (int64_t k = 0; k < size; k++) {
        P[k][(k == i ? j : (k == j) ? i : k)] = static_cast<T>(1);
    }

    return P;
}

template <typename T> 
matrix<T> matrix<T>::P(int64_t size, vector<int64_t> swaps) {
    matrix P(size);

    for (int64_t k = 0; k < size; k++) {
        P[k][swaps[k]] = static_cast<T>(1);
    }

    return P;
}

template <typename T> 
std::ostream& operator<<(std::ostream& out, const matrix<T>& M) {
    for (int64_t i = 0; i < M.rows(); i++) {
        out << "[";
        for (int64_t k = 0; k < M.cols(); k++) {
            out << M[i][k] << (k == M.cols() - 1 ? "]" : ",");
        }

        if (i != M.rows() - 1)
            out << "\n";
    }

    return out;
}

template <typename T> 
T det(matrix<T>& M) {
    if (!M.is_square_matrix()) 
        return 0;
   
    if (M.rows() == 1) 
        return M[0][0];

    T D = (M.is_triangular());

    if (M.is_triangular()) {
        for (int64_t k = 0; k < M.rows(); k++) 
            D *= M[k][k];
        return D;
    } else {
        static const int64_t i = 0;

        for (int64_t j = 0; j < M.rows(); j++) {
            matrix<T> minor = M.minor(i,j);
            D += M[i][j] * pow(-1, i + j) * det(minor);
        }

        return D;
    }
}

template <typename T> 
matrix<double> invert(matrix<T>& M) {
    assert(("Cannot find inverse of non-square matrix", M.is_square_matrix()));

    int64_t R = M.rows();

    matrix<double> O = static_cast<matrix<double>>(M),
                   C = static_cast<matrix<double>>(M),
                   I = matrix<double>::I(R),
                   P = matrix<double>::I(R);

    if (det(M) == static_cast<T>(0))
        return O;

    for (int64_t i = 0; i < R; i++) {
        double pivot = C[i][i];

        if (!pivot) {
            int64_t swapped = i+1;

            while (!C[swapped][i] && swapped < R) 
                swapped++;

            if (swapped == R && !C[swapped][i]) 
                return O;

            matrix<double> permute = matrix<double>::P(R, swapped, i);
            C = permute * C;
            P = permute * P;
            pivot = C[i][i];
        }

        for (int64_t k = i+1; k < R; k++) {
            matrix<double> E = matrix<double>::E(R, k, i, -1 * C[k][i]/pivot);
            C = E * C;
            I = E * I;
        }
    }

    for (int64_t i = R-1; i >= 0; i--) {
        double pivot = C[i][i];
        for (int64_t k = i-1; k >= 0; k--) {
            matrix<double> E = matrix<double>::E(R, k, i, -1 * C[k][i]/pivot);
            C = E * C;
            I = E * I;
        }
    }

    for (int64_t i =  0; i < R; i++) {
        double pivot = C[i][i];
        I[i] /= pivot;
    }

    return P * I;
}

#endif
