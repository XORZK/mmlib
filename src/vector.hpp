#ifndef VECTOR_HPP
#define VECTOR_HPP

#pragma once
#include "MACROS.hpp"

#include <iostream>
#include <assert.h>
#include <math.h>
#include <stdint.h>

template <typename T> class vector {
    private:
        int64_t vec_size, vec_capacity;
        T* data;
        bool column_vec;

        void widen(int64_t new_capacity);
    public:
        vector();

        vector(int64_t init_capacity);

        template <typename M> 
        vector(M* dat, int64_t dat_size);

        vector(T* dat, int64_t dat_size);

        vector(const vector<T>& v2);

		~vector() {}

        void fill(T value); 

        T& operator[](int64_t idx) const;

        T operator*(const vector<T> v2) const;

        vector<T> operator*(const T scalar) const;

        vector<T> operator/(const T scalar) const;

        vector<T> operator+(const T value) const;
        
        vector<T> operator-(const T value) const;

        vector<T> operator+(const vector<T> v2) const;

        vector<T> operator-(const vector<T> v2) const;

        void operator*=(const T scalar);

        void operator/=(const T scalar);

        void operator+=(const vector<T> v2);

        void operator-=(const vector<T> v2);

        void operator+=(const T value);
        
        void operator-=(const T value);

        bool operator==(const vector<T>& v2) const;

        template <typename U> 
        operator vector<U>() const;

        double magnitude() const;

        vector<double> normalize() const;

        void remove(int64_t idx);

        void pop();

        void push_back(const T value);

        void swap(int64_t i, int64_t j);

        double cos(vector<T>& v2) const;

        int64_t size() const;

        void size(int64_t new_size);

        int64_t capacity() const;

        void column(bool c);

        bool column() const;

        bool contains(T value) const;

        int64_t index(T value) const;

        vector<T> slice(int64_t slice_idx) const;

        vector<T> slice(int64_t slice_begin, int64_t slice_end) const;

        static vector<T> zero(int64_t size);

        template <typename U> 
        friend std::ostream& operator<<(std::ostream& out, const vector<U>& vec);
};

template <typename T> 
void vector<T>::widen(int64_t new_capacity) {
    if (new_capacity < this->vec_size) {
        std::length_error("Cannot widen vector to a smaller capacity.");
    } else {
        T* copy = static_cast<T*>(malloc(new_capacity * sizeof(T)));
        if (this->vec_size != 0) {
            for (int64_t k = 0; k < this->vec_size; k++) 
                copy[k] = this->data[k];
        }

        this->data = copy;
        this->vec_capacity = new_capacity;
    }
}

template <typename T> 
vector<T>::vector() : vec_size(0), vec_capacity(0), column_vec(false) {
    this->data = static_cast<T*>(malloc(this->vec_capacity * sizeof(T)));
}

template <typename T> 
vector<T>::vector(int64_t init_capacity) : vec_size(0), vec_capacity(init_capacity), column_vec(false) {
    this->data = static_cast<T*>(malloc(this->vec_capacity * sizeof(T)));
}

template <typename T> template <typename M>
vector<T>::vector(M* dat, int64_t dat_size) : vec_size(dat_size), vec_capacity(MAX(10, dat_size)), column_vec(false) {
    this->data = static_cast<T*>(dat);
}

template <typename T> 
vector<T>::vector(T* dat, int64_t dat_size) : vec_size(dat_size), vec_capacity(MAX(10, dat_size)), column_vec(false) {
    this->data = static_cast<T*>(dat);
}

template <typename T> 
vector<T>::vector(const vector<T>& v2) {
    this->data = static_cast<T*>(malloc(v2.size() * sizeof(T)));
    this->vec_size = v2.size();

    for (int64_t k = 0; k < this->vec_size; k++) {
        this->data[k] = v2[k];
    }
}

template <typename T> 
void vector<T>::fill(T value) {
    this->data = static_cast<T*>(malloc(this->vec_size * sizeof(T)));
    std::fill(this->data, this->data + this->vec_size, value);
    this->vec_capacity = this->vec_size;
}

template <typename T> 
T& vector<T>::operator[](int64_t idx) const {
    assert(("Out of Bounds Error: vector index out of range", (idx >= 0 && idx < this->vec_size)));
    
    return this->data[idx];
}

template <typename T>
T vector<T>::operator*(const vector<T> v2) const {
    assert(("Vector sizes must match to compute the dot product", (this->vec_size == v2.size())));

    T dot = 0;

    for (int64_t i = 0; i < this->vec_size; i++) 
        dot += this->data[i] * v2[i];

    return dot;
}

template <typename T> 
vector<T> vector<T>::operator*(const T scalar) const {
    vector<T> scaled(this->vec_size);

    for (int64_t i = 0; i < this->vec_size; i++) 
        scaled.push_back(scalar * this->data[i]);

    return scaled;
}

template <typename T> 
vector<T> vector<T>::operator/(const T scalar) const {
    assert(("Math Error: Cannot divide by 0", (scalar != 0)));

    vector<T> scaled(this->vec_size);

    for (int64_t i = 0; i < this->vec_size; i++) 
        scaled.push_back(this->data[i] / scalar);

    return scaled;
}

template <typename T> 
vector<T> vector<T>::operator+(const T value) const {
    vector<T> shifted(this->vec_size);

    for (int64_t i = 0; i < this->vec_size; i++) 
        shifted.push_back(this->data[i] + value);

    return shifted;
}

template <typename T> 
vector<T> vector<T>::operator-(const T value) const {
    vector<T> shifted(this->vec_size);

    for (int64_t i = 0; i < this->vec_size; i++)
        shifted.push_back(this->data[i] - value);

    return shifted;
}

template <typename T> 
vector<T> vector<T>::operator+(const vector<T> v2) const {
    assert(("Vector sizes must match to perform vector addition", (this->vec_size == v2.size())));

    vector<T> sum(this->vec_size);

    for (int64_t i = 0; i < this->vec_size; i++) 
        sum.push_back(this->data[i] + v2[i]);

    return sum;
}

template <typename T> 
vector<T> vector<T>::operator-(const vector<T> v2) const {
    assert(("Vector sizes must match to perform vector subtraction", (this->vec_size == v2.size())));

    vector<T> diff(this->vec_size);

    for (int64_t i = 0; i < this->vec_size; i++) 
        diff.push_back(this->data[i] - v2[i]);

    return diff;
}

template <typename T> 
void vector<T>::operator*=(const T scalar) {
    for (int64_t i = 0; i < this->vec_size; i++) 
        this->data[i] = (this->data[i] * scalar);
}

template <typename T> 
void vector<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0", (scalar != 0)));

    for (int64_t i = 0; i < this->vec_size; i++) 
        this->data[i] = (this->data[i] / scalar);
}

template <typename T> 
void vector<T>::operator+=(const vector<T> v2) {
    assert(("Vector sizes must match to perform vector addition", (this->vec_size == v2.size())));

    for (int64_t i = 0; i < this->vec_size; i++) 
        this->data[i] += v2[i];
}

template <typename T> 
void vector<T>::operator-=(const vector<T> v2) {
    assert(("Vector sizes must match to perform vector subtraction", (this->vec_size == v2.size())));

    for (int64_t i = 0; i < this->vec_size; i++) 
        this->data[i] -= v2[i];
}

template <typename T> 
void vector<T>::operator+=(const T value) {
    for (int64_t i = 0; i < this->vec_size; i++) 
        this->data[i] += value;
}

template <typename T> 
void vector<T>::operator-=(const T value) {
    for (int64_t i = 0; i < this->vec_size; i++) 
        this->data[i] -= value;
}

template <typename T> 
bool vector<T>::operator==(const vector<T>& v2) const {
    if (this->vec_size != v2.size()) return false;

    for (int64_t i = 0; i < this->vec_size; i++) {
        if (this->data[i] != v2[i]) 
            return false;
    }

    return true;
}

template <typename T> template <typename U> 
vector<T>::operator vector<U>() const {
    vector<U> casted(this->vec_size);

    for (int64_t i = 0; i < this->vec_size; i++)
        casted.push_back(static_cast<U>(this->data[i]));

    return casted;
}

template <typename T> 
double vector<T>::magnitude() const {
    double M = 0.0;

    for (int64_t i = 0; i < this->vec_siez; i++) 
        M += (this->data[i] * this->data[i]);

    return std::sqrt(M);
}

template <typename T> 
vector<double> vector<T>::normalize() const {
    double M = this->magnitude();

    assert(("Math Error: Cannot normalize a vector with a magnitude of 0.", (M != 0)));

    vector<double> casted = static_cast<vector<double>>(*this);

    return (casted/M);
}

template <typename T> 
void vector<T>::remove(int64_t idx) {
    assert(("Out of Bounds Error: Vector remove index out of range.", (idx >= 0 && idx < this->vec_size)));

    T* copy = static_cast<T*>(malloc(sizeof(T) * (this->vec_capacity)));

    for (int64_t i = 0; i < this->vec_size; i++) {
        if (i == idx)
            continue;
        copy[i - (i > idx)] = this->data[i];
    }

    --this->vec_size;
    this->data = copy;
}

template <typename T> 
void vector<T>::pop() {
    this->vec_size = MAX(this->vec_size - 1, 0);
}

template <typename T> 
void vector<T>::push_back(const T value) {
    if (this->vec_size >= this->vec_capacity) 
        this->widen(this->vec_size + 10);

    this->data[this->vec_size++] = value;
}

template <typename T> 
void vector<T>::swap(int64_t i, int64_t j) {
    assert(("Out of Bounds Error: Vector index out of range (swap)", (i >= 0 && i < this->vec_size && j >= 0 && j < this->vec_size)));


    T t = this->data[i];
    this->data[i] = this->data[j];
    this->data[j] = t;
}

template <typename T> 
double vector<T>::cos(vector<T>& v2) const {
    assert(("Vector sizes must match to compute dot product", (this->vec_size == v2.size()))); 

    return (v2.normalize() * this->normalize());
}

template <typename T> 
int64_t vector<T>::size() const {
    return this->vec_size;
}

template <typename T> 
void vector<T>::size(int64_t new_size) {
    this->vec_size = new_size;
}

template <typename T> 
int64_t vector<T>::capacity() const {
    return this->vec_capacity;
}

template <typename T> 
void vector<T>::column(bool c) {
    this->column_vec = c;
}

template <typename T> 
bool vector<T>::column() const {
    return this->column_vec;
}

template <typename T> 
bool vector<T>::contains(T value) const {
    for (int64_t i = 0; i < this->vec_size; i++) {
        if (this->data[i] == value)
            return true;
    }

    return false;
}

template <typename T> 
int64_t vector<T>::index(T value) const {
    for (int64_t i = 0; i < this->vec_size; i++) {
        if (this->data[i] == value) 
            return i;
    }

    return -1;
}

template <typename T> 
vector<T> vector<T>::slice(int64_t slice_idx) const {
    assert(("Slice index cannot be greater than vector size.", (this->vec_size >= slice_idx)));
    assert(("Slice index cannot be less than 0.", (this->slice_idx >= 0)));

    vector<T> sliced_vec(slice_idx);

    for (int64_t i = 0; i < slice_idx; i++)
        sliced_vec.push_back(this->data[i]);

    return sliced_vec;
}

template <typename T> 
vector<T> vector<T>::slice(const int64_t slice_begin, const int64_t slice_end) const {
    assert(("Slice indices must be greater than 0", (slice_begin >= 0 && slice_end >= 0)));

    assert(("Slice indices cannot be greater than vector size", (this->vec_size >= slice_begin && this->vec_size >= slice_end)));

    assert(("Slice end index must be greater than slice begin index", (slice_end >= slice_begin)));

    vector<T> sliced_vec(slice_end - slice_begin);

    for (int64_t i = slice_begin; i < slice_end; i++) 
        sliced_vec.push_back(this->data[i]);

    return sliced_vec;
}

template <typename T> 
vector<T> vector<T>::zero(int64_t size) {
    vector<T> z(size);

    z.size(size);
    z.fill(static_cast<T>(0));

    return z;
}

template <typename T> 
std::ostream& operator<<(std::ostream& out, const vector<T>& vec) {
    out << '<';

    for (int64_t i = 0; i < vec.size(); i++) {
        out << vec[i] << (i < vec.size() - 1 ? ',' : '>');
    }

    return out;
}

template <typename T>
inline double distance(vector<T> v1, vector<T> v2) {
    assert(("Vectors must be the same size to compute distances", 
           (v1.size() == v2.size())));

    double D = 0;

    for (int64_t k = 0; k < v1.size(); k++) 
        D += (v2[k] - v1[k]) * (v2[k] - v1[k]);

    return std::sqrt(D);
}

#endif
