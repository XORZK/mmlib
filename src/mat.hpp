#ifndef MAT_HPP
#define MAT_HPP

#pragma once
#include "vec.hpp"
#include "matrix.hpp"

#include <assert.h>
#include <math.h>
#include <iostream>

// Wrapper for 2x2 Matrices
template <typename T> class mat2 {
    private:
        static const int64_t N = 2, M = 2;
		vec2<T> *v1, *v2;
    public:
        mat2();

        mat2(mat2<T>& m2);

		~mat2() {
			free(v1);
			free(v2);
		}

        void fill(T value);

        void fill(std::function<T(int64_t, int64_t)> func);

        vec2<T>& operator[](int64_t idx);

        mat2<T> operator*(const mat2<T>& m2) const;

        mat2<T> operator*(const T scalar) const;

        mat2<T> operator/(const T scalar) const;

        mat2<T> operator+(const T value) const;

        mat2<T> operator-(const T value) const;

        mat2<T> operator+(const mat2<T>& m2) const;

        mat2<T> operator-(const mat2<T>& m2) const;

        vec2<T> operator*(const vec2<T>& v2) const;

        void operator*=(const T scalar);

        void operator/=(const T scalar);

        void operator+=(const mat2<T>& m2);

        void operator-=(const mat2<T>& m2);

        void operator+=(const T value);

        void operator-=(const T value);

        bool operator==(const mat2<T>& m2) const;

        template <typename U>
        operator mat2<U>() const;

        operator matrix<T>() const;

        mat2<T> transpose() const;

        bool is_lower_triangular() const;

        bool is_upper_triangular() const;

        bool is_triangular() const;

        int64_t rows() const;

        int64_t cols() const;
};

template <typename T> class mat3 { 
    private:
        static const int64_t N = 3, M = 3;
		vec3<T> *v1, *v2, *v3;
    public:
        mat3();

        mat3(mat3<T>& m2);

		~mat3() {
			free(v1);
			free(v2);
			free(v3);
		}

        void fill(T value);

        void fill(std::function<T(int64_t, int64_t)> func);

        vec3<T>& operator[](int64_t idx);

        mat3<T> operator*(mat3<T>& m2) const;

        mat3<T> operator*(const T scalar) const;

        mat3<T> operator/(const T scalar) const;

        mat3<T> operator+(const T value) const;

        mat3<T> operator-(const T value) const;

        mat3<T> operator+(mat3<T>& m2) const;

        mat3<T> operator-(mat3<T>& m2) const;

        vec3<T> operator*(vec3<T>& v2) const;

        void operator*=(const T scalar);

        void operator/=(const T scalar);

        void operator+=(mat3<T>& m2);

        void operator-=(mat3<T>& m2);

        void operator+=(const T value);

        void operator-=(const T value);

        bool operator==(mat3<T>& m2) const;

        template <typename U>
        operator mat3<U>() const;

        operator matrix<T>() const;

        mat3<T> transpose() const;

        bool is_lower_triangular() const;

        bool is_upper_triangular() const;

        bool is_triangular() const;

        mat2<T> minor(int64_t i, int64_t j) const;

        int64_t rows() const;

        int64_t cols() const;
};

template <typename T> class mat4 { 
    private:
        static const int64_t N = 4, M = 4;
        vec4<T>* data;
    public:
        mat4();

        mat4(const mat4<T>& m2);

		~mat4() {
			free(data);
			data = NULL;
		}

        void fill(T value);

        void fill(std::function<T(int64_t, int64_t)> func);

        vec4<T>& operator[](int64_t idx) const;

        mat4<T> operator*(const mat4<T>& m2) const;

        mat4<T> operator*(const T scalar) const;

        mat4<T> operator/(const T scalar) const;

        mat4<T> operator+(const T value) const;

        mat4<T> operator-(const T value) const;

        mat4<T> operator+(const mat4<T>& m2) const;

        mat4<T> operator-(const mat4<T>& m2) const;

        vec4<T> operator*(const vec4<T>& v2) const;

        void operator*=(const T scalar);

        void operator/=(const T scalar);

        void operator+=(const mat4<T>& m2);

        void operator-=(const mat4<T>& m2);

        void operator+=(const T value);

        void operator-=(const T value);

        bool operator==(const mat4<T>& m2) const;

        template <typename U>
        operator mat4<U>() const;

        operator matrix<T>() const;

        mat4<T> transpose() const;

        bool is_lower_triangular() const;

        bool is_upper_triangular() const;

        bool is_triangular() const;

        mat3<T> minor(int64_t i, int64_t j) const;

        int64_t rows() const;

        int64_t cols() const;
};

template <typename T> 
mat2<T>::mat2() {
	v1 = new vec2<T>();
	v2 = new vec2<T>();
}

template <typename T> 
mat2<T>::mat2(mat2<T>& m2) {
	v1 = new vec2<T>(m2[0]);
	v2 = new vec2<T>(m2[1]);
}

template <typename T> 
void mat2<T>::fill(T value) {
	v1->fill(value);
	v2->fill(value);
}

template <typename T>
void mat2<T>::fill(std::function<T(int64_t, int64_t)> func) {
	(*v1)[0] = func(0, 0);
	(*v1)[1] = func(0, 1);
	(*v2)[0] = func(1, 0);
	(*v2)[1] = func(1, 1);
}

template <typename T> 
vec2<T>& mat2<T>::operator[](int64_t idx) {
    assert(("Out of Bounds Error: matrix index out of range", (idx >= 0 && idx < N)));

    return (idx == 0 ? *v1 : *v2);
}

template <typename T> 
mat2<T> mat2<T>::operator*(const mat2<T>& m2) const {
    mat2<T> P;

	P[0][0] = (*v1)[0] * m2[0][0] + (*v1)[1] * m2[1][0];
	P[0][1] = (*v1)[0] * m2[0][1] + (*v1)[1] * m2[1][1];
	P[1][0] = (*v2)[0] * m2[0][0] + (*v2)[1] * m2[1][0];
	P[1][1] = (*v2)[0] * m2[0][1] + (*v2)[1] * m2[1][1];

    return P;
}

template <typename T> 
mat2<T> mat2<T>::operator*(const T scalar) const {
    mat2<T> scaled;

    for (int64_t k = 0; k < N; k++) {
        scaled[k] = ((*this)[k] * scalar);
    }

    return scaled;
}

template <typename T> 
mat2<T> mat2<T>::operator/(const T scalar) const {
    assert(("Math Error: Cannot divide by 0", scalar != 0));
    mat2<T> scaled;

    for (int64_t i = 0; i < N; i++) {
        scaled[i] = ((*this)[i] / scalar);
    }

    return scaled;
}

template <typename T> 
mat2<T> mat2<T>::operator+(const T value) const {
    mat2<T> shifted;

    for (int64_t i = 0; i < N; i++) {
        shifted[i] = ((*this)[i] + value);
    }

    return shifted;
}

template <typename T> 
mat2<T> mat2<T>::operator-(const T value) const {
    mat2<T> shifted;

    for (int64_t i = 0; i < N; i++) {
        shifted[i] = ((*this)[i] - value);
    }

    return shifted;
}

template <typename T> 
mat2<T> mat2<T>::operator+(const mat2<T>& m2) const {
    mat2<T> sum;

    for (int64_t i = 0; i < N; i++) {
        sum[i] = ((*this)[i] + m2[i]);
    }

    return sum;
}

template <typename T> 
mat2<T> mat2<T>::operator-(const mat2<T>& m2) const {
    mat2<T> diff;

    for (int64_t i = 0; i < N; i++) {
        diff[i] = ((*this)[i] - m2[i]);
    }

    return diff;
}

template <typename T>
vec2<T> mat2<T>::operator*(const vec2<T>& v2) const {
    vec2<T> P((*this)[0] * v2,
              (*this)[1] * v2);

    return P;
}

template <typename T> 
void mat2<T>::operator*=(const T scalar) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] *= scalar;
    }
}

template <typename T> 
void mat2<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0", scalar != 0));
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] /= scalar;
    }
}

template <typename T> 
void mat2<T>::operator+=(const T value) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] += value;
    }
}

template <typename T> 
void mat2<T>::operator-=(const T value) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] -= value;
    }
}

template <typename T> 
void mat2<T>::operator+=(const mat2<T>& m2) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] += m2[i];
    }
}

template <typename T> 
void mat2<T>::operator-=(const mat2<T>& m2) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] -= m2[i];
    }
}

template <typename T> 
bool mat2<T>::operator==(const mat2<T>& m2) const {
    for (int64_t i = 0; i < N; i++) {
        if ((*this)[i] != m2[i])
            return false;
    }

    return true;
}

template <typename T> template <typename U>
mat2<T>::operator mat2<U>() const {
    mat2<U> casted;

    for (int64_t k = 0; k < N; k++)
        casted[k] = static_cast<vec2<U>>((*this)[k]);

    return casted;
}

template <typename T> 
mat2<T>::operator matrix<T>() const {
    matrix<T> casted(N, N);

    for (int64_t k = 0; k < N; k++) 
        casted[k] = static_cast<vector<T>>((*this)[k]);

    return casted;
}

template <typename T> 
mat2<T> mat2<T>::transpose() const {
    mat2<T> TP;

    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = 0; k < M; k++) {
            TP[k][i] = (*this)[i][k];
        }
    }

    return mat2(TP);
}

template <typename T> 
bool mat2<T>::is_lower_triangular() const {
    return ((*this)[0][1] == static_cast<T>(0));
}

template <typename T> 
bool mat2<T>::is_upper_triangular() const {
    return ((*this)[1][0] == static_cast<T>(0));
}

template <typename T> 
bool mat2<T>::is_triangular() const {
    return (this->is_lower_triangular() || this->is_upper_triangular());
}

template <typename T> 
int64_t mat2<T>::rows() const { return N; }

template <typename T> 
int64_t mat2<T>::cols() const { return M; }

template <typename T> 
mat3<T>::mat3() {
	v1 = new vec3<T>();
	v2 = new vec3<T>();
	v3 = new vec3<T>();
}

template <typename T> 
mat3<T>::mat3(mat3<T>& m2) {
	v1 = new vec3<T>(m2[0]);	
	v2 = new vec3<T>(m2[1]);	
	v3 = new vec3<T>(m2[2]);	
}

template <typename T> 
void mat3<T>::fill(T value) {
	v1->fill(value);
	v2->fill(value);
	v3->fill(value);
}

template <typename T>
void mat3<T>::fill(std::function<T(int64_t, int64_t)> func) {
    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = 0; k < N; k++) {
            (*this)[i][k] = func(i,k);
        }
    }
}

template <typename T> 
vec3<T>& mat3<T>::operator[](int64_t idx) {
    assert(("Out of Bounds Error: matrix index out of range", (idx >= 0 && idx < N)));

    return (idx == 0 ? *v1 : idx == 1 ? *v2 : *v3);
}

template <typename T> 
mat3<T> mat3<T>::operator*(mat3<T>& m2) const {
    mat3<T> P;

    P[0][0] = (*v1)[0] * m2[0][0] + 
              (*v1)[1] * m2[1][0] + 
              (*v1)[2] * m2[2][0];

    P[0][1] = (*v1)[0] * m2[0][1] +
              (*v1)[1] * m2[1][1] + 
              (*v1)[2] * m2[2][1];

    P[0][2] = (*v1)[0] * m2[0][2] + 
              (*v1)[1] * m2[1][2] + 
              (*v1)[2] * m2[2][2];

    P[1][0] = (*v2)[0] * m2[0][0] + 
              (*v2)[1] * m2[1][0] + 
              (*v2)[2] * m2[2][0];

    P[1][1] = (*v2)[0] * m2[0][1] +
              (*v2)[1] * m2[1][1] + 
              (*v2)[2] * m2[2][1];

    P[1][2] = (*v2)[0] * m2[0][2] + 
              (*v2)[1] * m2[1][2] + 
              (*v2)[2] * m2[2][2];

    P[2][0] = (*v3)[0] * m2[0][0] + 
              (*v3)[1] * m2[1][0] + 
              (*v3)[2] * m2[2][0];

    P[2][1] = (*v3)[0] * m2[0][1] +
              (*v3)[1] * m2[1][1] + 
              (*v3)[2] * m2[2][1];

    P[2][2] = (*v3)[0] * m2[0][2] + 
              (*v3)[1] * m2[1][2] + 
              (*v3)[2] * m2[2][2];

    return P;
}

template <typename T> 
mat3<T> mat3<T>::operator*(const T scalar) const {
    mat3<T> scaled;

    for (int64_t k = 0; k < N; k++) {
        scaled[k] = ((*this)[k] * scalar);
    }

    return scaled;
}

template <typename T> 
mat3<T> mat3<T>::operator/(const T scalar) const {
    assert(("Math Error: Cannot divide by 0", scalar != 0));
    mat3<T> scaled;

    for (int64_t i = 0; i < N; i++) {
        scaled[i] = ((*this)[i] / scalar);
    }

    return scaled;
}

template <typename T> 
mat3<T> mat3<T>::operator+(const T value) const {
    mat3<T> shifted;

    for (int64_t i = 0; i < N; i++) {
        shifted[i] = ((*this)[i] + value);
    }

    return shifted;
}

template <typename T> 
mat3<T> mat3<T>::operator-(const T value) const {
    mat3<T> shifted;

    for (int64_t i = 0; i < N; i++) {
        shifted[i] = ((*this)[i] - value);
    }

    return shifted;
}

template <typename T> 
mat3<T> mat3<T>::operator+(mat3<T>& m2) const {
    mat3<T> sum;

    for (int64_t i = 0; i < N; i++) {
        sum[i] = ((*this)[i] + m2[i]);
    }

    return sum;
}

template <typename T> 
mat3<T> mat3<T>::operator-(mat3<T>& m2) const {
    mat3<T> diff;

    for (int64_t i = 0; i < N; i++) {
        diff[i] = ((*this)[i] - m2[i]);
    }

    return diff;
}

template <typename T> 
vec3<T> mat3<T>::operator*(vec3<T>& v) const {
    vec3<T> P((*v1) * v,
              (*v2) * v,
              (*v3) * v);

    return P;
}

template <typename T> 
void mat3<T>::operator*=(const T scalar) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] *= scalar;
    }
}

template <typename T> 
void mat3<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0", scalar != 0));
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] /= scalar;
    }
}

template <typename T> 
void mat3<T>::operator+=(const T value) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] += value;
    }
}

template <typename T> 
void mat3<T>::operator-=(const T value) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] -= value;
    }
}

template <typename T> 
void mat3<T>::operator+=(mat3<T>& m2) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] += m2[i];
    }
}

template <typename T> 
void mat3<T>::operator-=(mat3<T>& m2) {
    for (int64_t i = 0; i < N; i++) {
        (*this)[i] -= m2[i];
    }
}

template <typename T> 
bool mat3<T>::operator==(mat3<T>& m2) const {
    for (int64_t i = 0; i < N; i++) {
        if ((*this)[i] != m2[i])
            return false;
    }

    return true;
}

template <typename T> template <typename U>
mat3<T>::operator mat3<U>() const {
    mat3<U> casted;

    for (int64_t k = 0; k < N; k++)
        casted[k] = static_cast<vec3<U>>((*this)[k]);

    return casted;
}

template <typename T> 
mat3<T>::operator matrix<T>() const {
    matrix<T> casted(N, N);

    for (int64_t k = 0; k < N; k++) 
        casted[k] = static_cast<vector<T>>((*this)[k]);

    return casted;
}

template <typename T> 
mat3<T> mat3<T>::transpose() const {
    mat3<T> TP;

    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = 0; k < M; k++) {
            TP[k][i] = (*this)[i][k];
        }
    }

    return mat3(TP);
}

template <typename T> 
bool mat3<T>::is_lower_triangular() const {
    T z = static_cast<T>(0);

    return ((*this)[0][1] == z && 
			(*this)[0][2] == z && 
			(*this)[1][2] == z);
}

template <typename T> 
bool mat3<T>::is_upper_triangular() const {
    T z = static_cast<T>(0);

    return ((*this)[1][0] == z && 
			(*this)[2][0] == z && 
			(*this)[2][1] == z);
}

template <typename T> 
bool mat3<T>::is_triangular() const {
    return (this->is_lower_triangular() || this->is_upper_triangular());
}

template <typename T> 
mat2<T> mat3<T>::minor(int64_t i, int64_t j) const {
    mat2<T> M;

    for (int64_t k = 0; k < N; k++) {
        for (int64_t p = 0; p < N; p++) {
            if (p == j || k == i) 
                continue;

            M[k - (k > i)][p - (p > j)] = (*this)[k][p];
        }
    }

    return M;
}

template <typename T> 
int64_t mat3<T>::rows() const { return N; }

template <typename T> 
int64_t mat3<T>::cols() const { return M; }

template <typename T> 
mat4<T>::mat4() {
    this->data = static_cast<vec4<T>*>(malloc(N * sizeof(vec4<T>)));

    for (int64_t k = 0; k < N; k++) 
        this->data[k] = vec4<T>::zero();
}

template <typename T> 
mat4<T>::mat4(const mat4<T>& m2) {
    this->data = static_cast<vec4<T>*>(malloc(N * sizeof(vec4<T>)));

    for (int64_t k = 0; k < N; k++) {
        vec4<T> copy(m2[k]);
        this->data[k] = copy;
    }
}

template <typename T> 
void mat4<T>::fill(T value) {
    for (int64_t k = 0; k < N; k++) 
        this->data[k].fill(value);
}

template <typename T>
void mat4<T>::fill(std::function<T(int64_t, int64_t)> func) {
    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = 0; k < N; k++) {
            this->data[i][k] = func(i,k);
        }
    }
}

template <typename T> 
vec4<T>& mat4<T>::operator[](int64_t idx) const {
    assert(("Out of Bounds Error: matrix index out of range", (idx >= 0 && idx < N)));

    return this->data[idx];
}

template <typename T> 
mat4<T> mat4<T>::operator*(const mat4<T>& m2) const {
    mat4<T> P;

    P[0][0] = this->data[0][0] * m2[0][0] + 
              this->data[0][1] * m2[1][0] + 
              this->data[0][2] * m2[2][0] + 
              this->data[0][3] * m2[3][0];

    P[0][1] = this->data[0][0] * m2[0][1] +
              this->data[0][1] * m2[1][1] + 
              this->data[0][2] * m2[2][1] + 
              this->data[0][3] * m2[3][1];

    P[0][2] = this->data[0][0] * m2[0][2] + 
              this->data[0][1] * m2[1][2] + 
              this->data[0][2] * m2[2][2] + 
              this->data[0][3] * m2[3][2];

    P[0][3] = this->data[0][0] * m2[0][3] + 
              this->data[0][1] * m2[1][3] + 
              this->data[0][2] * m2[2][3] + 
              this->data[0][3] * m2[3][3];

    P[1][0] = this->data[1][0] * m2[0][0] + 
              this->data[1][1] * m2[1][0] + 
              this->data[1][2] * m2[2][0] + 
              this->data[1][3] * m2[3][0];

    P[1][1] = this->data[1][0] * m2[0][1] +
              this->data[1][1] * m2[1][1] + 
              this->data[1][2] * m2[2][1] + 
              this->data[1][3] * m2[3][1];

    P[1][2] = this->data[1][0] * m2[0][2] + 
              this->data[1][1] * m2[1][2] + 
              this->data[1][2] * m2[2][2] + 
              this->data[1][3] * m2[3][2];

    P[1][3] = this->data[1][0] * m2[0][3] + 
              this->data[1][1] * m2[1][3] + 
              this->data[1][2] * m2[2][3] + 
              this->data[1][3] * m2[3][3];

    P[2][0] = this->data[2][0] * m2[0][0] + 
              this->data[2][1] * m2[1][0] + 
              this->data[2][2] * m2[2][0] + 
              this->data[2][3] * m2[3][0];

    P[2][1] = this->data[2][0] * m2[0][1] +
              this->data[2][1] * m2[1][1] + 
              this->data[2][2] * m2[2][1] + 
              this->data[2][3] * m2[3][1];

    P[2][2] = this->data[2][0] * m2[0][2] + 
              this->data[2][1] * m2[1][2] + 
              this->data[2][2] * m2[2][2] + 
              this->data[2][3] * m2[3][2];

    P[2][3] = this->data[2][0] * m2[0][3] + 
              this->data[2][1] * m2[1][3] + 
              this->data[2][2] * m2[2][3] + 
              this->data[2][3] * m2[3][3];

    P[3][0] = this->data[3][0] * m2[0][0] + 
              this->data[3][1] * m2[1][0] + 
              this->data[3][2] * m2[2][0] + 
              this->data[3][3] * m2[3][0];

    P[3][1] = this->data[3][0] * m2[0][1] +
              this->data[3][1] * m2[1][1] + 
              this->data[3][2] * m2[2][1] + 
              this->data[3][3] * m2[3][1];

    P[3][2] = this->data[3][0] * m2[0][2] + 
              this->data[3][1] * m2[1][2] + 
              this->data[3][2] * m2[2][2] + 
              this->data[3][3] * m2[3][2];

    P[3][3] = this->data[3][0] * m2[0][3] + 
              this->data[3][1] * m2[1][3] + 
              this->data[3][2] * m2[2][3] + 
              this->data[3][3] * m2[3][3];

    return P;
}

template <typename T> 
mat4<T> mat4<T>::operator*(const T scalar) const {
    mat4<T> scaled;

    for (int64_t k = 0; k < N; k++) {
        scaled[k] = (this->data[k] * scalar);
    }

    return scaled;
}

template <typename T> 
mat4<T> mat4<T>::operator/(const T scalar) const {
    assert(("Math Error: Cannot divide by 0", scalar != 0));
    mat4<T> scaled;

    for (int64_t i = 0; i < N; i++) {
        scaled[i] = (this->data[i] / scalar);
    }

    return scaled;
}

template <typename T> 
mat4<T> mat4<T>::operator+(const T value) const {
    mat4<T> shifted;

    for (int64_t i = 0; i < N; i++) {
        shifted[i] = (this->data[i] + value);
    }

    return shifted;
}

template <typename T> 
mat4<T> mat4<T>::operator-(const T value) const {
    mat4<T> shifted;

    for (int64_t i = 0; i < N; i++) {
        shifted[i] = (this->data[i] - value);
    }

    return shifted;
}

template <typename T> 
mat4<T> mat4<T>::operator+(const mat4<T>& m2) const {
    mat4<T> sum;

    for (int64_t i = 0; i < N; i++) {
        sum[i] = (this->data[i] + m2[i]);
    }

    return sum;
}

template <typename T> 
mat4<T> mat4<T>::operator-(const mat4<T>& m2) const {
    mat4<T> diff;

    for (int64_t i = 0; i < N; i++) {
        diff[i] = (this->data[i] - m2[i]);
    }

    return diff;
}

template <typename T> 
vec4<T> mat4<T>::operator*(const vec4<T>& v2) const {
    vec4<T> P(this->data[0] * v2,
              this->data[1] * v2,
              this->data[2] * v2,
              this->data[3] * v2);

    return P;
}



template <typename T> 
void mat4<T>::operator*=(const T scalar) {
    for (int64_t i = 0; i < N; i++) {
        this->data[i] *= scalar;
    }
}

template <typename T> 
void mat4<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0", scalar != 0));
    for (int64_t i = 0; i < N; i++) {
        this->data[i] /= scalar;
    }
}

template <typename T> 
void mat4<T>::operator+=(const T value) {
    for (int64_t i = 0; i < N; i++) {
        this->data[i] += value;
    }
}

template <typename T> 
void mat4<T>::operator-=(const T value) {
    for (int64_t i = 0; i < N; i++) {
        this->data[i] -= value;
    }
}

template <typename T> 
void mat4<T>::operator+=(const mat4<T>& m2) {
    for (int64_t i = 0; i < N; i++) {
        this->data[i] += m2[i];
    }
}

template <typename T> 
void mat4<T>::operator-=(const mat4<T>& m2) {
    for (int64_t i = 0; i < N; i++) {
        this->data[i] -= m2[i];
    }
}

template <typename T> 
bool mat4<T>::operator==(const mat4<T>& m2) const {
    for (int64_t i = 0; i < N; i++) {
        if (this->data[i] != m2[i])
            return false;
    }

    return true;
}

template <typename T> template <typename U>
mat4<T>::operator mat4<U>() const {
    mat4<U> casted;

    for (int64_t k = 0; k < N; k++)
        casted[k] = static_cast<vec4<U>>(this->data[k]);

    return casted;
}

template <typename T> 
mat4<T>::operator matrix<T>() const {
    matrix<T> casted(N, N);

    for (int64_t k = 0; k < N; k++) 
        casted[k] = static_cast<vector<T>>(this->data[k]);

    return casted;
}

template <typename T> 
mat4<T> mat4<T>::transpose() const {
    mat4<T> TP;

    for (int64_t i = 0; i < N; i++) {
        for (int64_t k = 0; k < M; k++) {
            TP[k][i] = this->data[i][k];
        }
    }

    return mat4(TP);
}

template <typename T> 
bool mat4<T>::is_lower_triangular() const {
    T z = static_cast<T>(0);
    
    return (this->data[0][1] == z && this->data[0][2] == z && this->data[0][3] == z &&
            this->data[1][2] == z && this->data[1][3] == z && this->data[2][3] == z);
}

template <typename T> 
bool mat4<T>::is_upper_triangular() const {
    T z = static_cast<T>(0);

    return (this->data[1][0] == z && this->data[2][0] == z && this->data[3][0] == z &&
            this->data[2][1] == z && this->data[3][1] == z && this->data[3][2] == z);
}

template <typename T> 
bool mat4<T>::is_triangular() const {
    return (this->is_lower_triangular() || this->is_upper_triangular());
}

template <typename T> 
mat3<T> mat4<T>::minor(int64_t i, int64_t j) const {
    mat3<T> M;

    for (int64_t k = 0; k < N; k++) {
        for (int64_t p = 0; p < N; p++) {
            if (p == j || k == i) 
                continue;

            M[k - (k > i)][p - (p > j)] = this->data[k][p];
        }
    }

    return M;
}

template <typename T> 
int64_t mat4<T>::rows() const { return N; }

template <typename T> 
int64_t mat4<T>::cols() const { return M; }

template <typename T> 
std::ostream& operator<<(std::ostream& out, mat2<T>& M) {
    for (int64_t i = 0; i < 2; i++) {
        out << "[";
        for (int64_t k = 0; k < 2; k++) {
            out << M[i][k] << (k == 1 ? "]" : ",");
        }

        if (i != 1) 
            out << "\n";
    }

    return out;
}

template <typename T> 
std::ostream& operator<<(std::ostream& out, mat3<T>& M) {
    for (int64_t i = 0; i < 3; i++) {
        out << "[";
        for (int64_t k = 0; k < 3; k++) {
            out << M[i][k] << (k == 2 ? "]" : ",");
        }

        if (i != 2) 
            out << "\n";
    }

    return out;
}

template <typename T> 
std::ostream& operator<<(std::ostream& out, mat4<T>& M) {
    for (int64_t i = 0; i < 4; i++) {
        out << "[";
        for (int64_t k = 0; k < 4; k++) {
            out << M[i][k] << (k == 3 ? "]" : ",");
        }

        if (i != 3) 
            out << "\n";
    }

    return out;
}

template <typename T> 
inline T det(const mat2<T>& M) {
    return (M[0][0] * M[1][1]) - (M[0][1] * M[1][0]);
}

template <typename T> 
inline T det(const mat3<T>& M) {
    matrix<T> C = static_cast<matrix<T>>(M);

    return det(C);
}

template <typename T> 
inline T det(const mat4<T>& M) {
    matrix<T> C = static_cast<matrix<T>>(M);

    return det(C);
}

template <typename T> 
inline matrix<double> invert(const mat2<T>& M) {
    matrix<T> C = static_cast<matrix<T>>(M);

    return invert(C);
}

template <typename T> 
inline matrix<double> invert(const mat3<T>& M) {
    matrix<T> C = static_cast<matrix<T>>(M);

    return invert(C);
}

template <typename T> 
inline matrix<double> invert(const mat4<T>& M) {
    matrix<T> C = static_cast<matrix<T>>(M);

    return invert(C);
}

inline mat2<double> create_2d_rotation_matrix(double theta, const bool deg = true) {
    if (deg) {
        theta *= M_PI/180;
    }

    mat2<double> R;

    R[0] = vec2(cos(theta), -sin(theta));
    R[1] = vec2(sin(theta), cos(theta));

    return R;
}

inline mat3<double> create_z_rotation(double alpha, const bool deg = true) {
    if (deg) {
        alpha *= M_PI/180;
    }

    mat3<double> Z;

    Z[0] = vec3(cos(alpha), -sin(alpha), 0.0);
    Z[1] = vec3(sin(alpha), cos(alpha), 0.0);
    Z[2] = vec3(0.0, 0.0, 1.0);

    return Z;
}

inline mat3<double> create_y_rotation(double beta, const bool deg = true) {
    if (deg) {
        beta *= M_PI/180;
    }

    mat3<double> Y;

    Y[0] = vec3(cos(beta), 0.0, sin(beta));
    Y[1] = vec3(0.0, 1.0, 0.0);
    Y[2] = vec3(-sin(beta), 0.0, cos(beta));

    return Y;
}

inline mat3<double> create_x_rotation(double gamma, const bool deg = true) {
    if (deg) {
        gamma *= M_PI/180;
    }

    mat3<double> X;

    X[0] = vec3(1.0, 0.0, 0.0);
    X[1] = vec3(0.0, cos(gamma), -sin(gamma));
    X[2] = vec3(0.0, sin(gamma), cos(gamma));

    return X;
}

inline mat3<double> create_3d_rotation_matrix(double alpha = 0,     
                                              double beta = 0, 
                                              double gamma = 0, 
                                              const bool deg = true) {
    mat3<double> Z = create_z_rotation(alpha, deg),
                 Y = create_y_rotation(beta, deg),
                 X = create_x_rotation(gamma, deg);

    return (Z * Y * X);
}

template <typename T> 
inline void mult(vec3<T>& v, mat3<T>& m) {
	T a = v.x(), b = v.y(), c = v.z();
	v[0] = (m[0][0] * a) + (m[0][1] * b) + (m[0][2] * c);
	v[1] = (m[1][0] * a) + (m[1][1] * b) + (m[1][2] * c);
	v[2] = (m[2][0] * a) + (m[2][1] * b) + (m[2][2] * c);
}

#endif
