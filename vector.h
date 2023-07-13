#ifndef VECTOR_H
#define VECTOR_H

#pragma once
#include "MACROS.h"
#include "rational.h"
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <assert.h>
#include <math.h>
#include <stdint.h>

template <typename T>
class vector {
    static_assert((std::is_arithmetic<T>::value || std::is_same<T, rat>::value),
                   "Vector must contain arithmetic types.");
    private:
        uint64_t size, capacity;
        T* data;
        bool column;

        void widen(uint64_t new_capacity);
    public:
        vector();

        vector(uint64_t init_capacity);

        template <typename M>
        vector(M* d, uint64_t data_size);

        vector(T* d, uint64_t data_size);

        void fill(T value);

        T& operator[](uint64_t idx) const;

        T operator*(const vector<T> v2) const;

        vector<double> operator*(const double scalar) const;

        vector<double> operator/(const double scalar) const;

        template <typename U>
        void operator/=(const U scalar);

        vector<T> operator+(const vector<T> v2) const;

        vector<T> operator+(const T value) const;

        vector<T> operator-(const vector<T> v2) const;

        vector<T> operator-(const T value) const;

        template <typename U>
        operator vector<U>() const;

        double magnitude() const;

        vector<double> normalize() const;

        void push_back(const T value); 

        double cos(vector<T> v2) const;

        uint64_t get_size() const;

        uint64_t get_capacity() const;

        void set_size(uint64_t new_size);

        void set_column(bool c);

        bool is_column() const;

        static vector<T> zero(uint64_t size);
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const vector<T>& vec);

template <typename T>
void vector<T>::widen(uint64_t new_capacity) {
    assert(new_capacity >= this->size);

    T* copy = static_cast<T*>(malloc(new_capacity * sizeof(T)));

    if (this->size != 0) {
        std::copy(this->data, this->data + this->size, copy);
    }

    this->data = copy;
    this->capacity = new_capacity;
}

template <typename T>
vector<T>::vector() : size(0), capacity(0), column(false) {
    this->data = static_cast<T*>(malloc(this->capacity * sizeof(T)));
}

template <typename T>
vector<T>::vector(uint64_t init): size(0), capacity(MAX(init, 0)), column(false) {
    this->data = static_cast<T*>(malloc(this->capacity * sizeof(T)));
}

template <typename T>
template <typename M> vector<T>::vector(M* dat, uint64_t dat_size): size(dat_size), capacity(MAX(dat_size, 10)), data(static_cast<T*>(dat)), column(false) {}

template <typename T>
vector<T>::vector(T* dat, uint64_t dat_size): size(dat_size), capacity(MAX(dat_size, 10)), data(dat) {}


template <typename T> 
void vector<T>::fill(T value) {
    this->data = static_cast<T*>(malloc(this->size * sizeof(T)));

    std::fill(this->data, this->data + this->size, value);

    this->capacity = this->size;
}

template <typename T>
T& vector<T>::operator[](uint64_t idx) const {
    assert(idx >= 0 && idx < this->size);
    return this->data[idx];
}

template <typename T>
vector<double> vector<T>::operator/(const double scalar) const {
    assert(scalar != 0);

    vector<double> q(this->size);

    for (uint64_t i = 0; i < this->size; i++) {
        q.push_back(this->data[i] * 1/scalar);
    }

    return q;
}

template <typename T>
template <typename U> void vector<T>::operator/=(const U scalar) {
    assert(scalar != 0);

    for (uint64_t i = 0; i < this->size; i++) {
        this->data[i] = static_cast<T>(this->data[i]/scalar);
    }
}

template <typename T>
T vector<T>::operator*(const vector<T> v2) const {
    assert(this->size == v2.get_size());

    T dot = 0;

    for (uint64_t i = 0; i < this->size; i++) {
        dot += this->data[i] * v2[i];
    }

    return dot;
}

template <typename T>
vector<double> vector<T>::operator*(const double scalar) const {
    vector<double> p(this->size);

    for (uint64_t i = 0; i < this->size; i++) {
        p.push_back(scalar * this->data[i]);
    }

    return p;
}

template <typename T>
vector<T> vector<T>::operator+(const vector<T> v2) const {
    assert(this->size == v2.get_size());

    vector<T> s(this->size);

    for (uint64_t i = 0; i < this->size; i++) {
        s.push_back(this->data[i] + v2[i]);
    }

    return s;
}

template <typename T>
vector<T> vector<T>::operator+(const T value) const {
    vector<T> s(this->size);

    for (uint64_t i = 0; i < this->size; i++) {
        s.push_back(this->data[i] + value);
    }

    return s;
}

template <typename T>
vector<T> vector<T>::operator-(const vector<T> v2) const {
    assert(this->size == v2.get_size());

    vector<T> d(this->size);

    for (uint64_t i = 0; i < this->size; i++) {
        d.push_back(this->data[i] - v2[i]);
    }

    return d;
}

template <typename T>
vector<T> vector<T>::operator-(const T value) const {
    vector<T> d(this->size);

    for (uint64_t i = 0; i < this->size; i++) {
        d.push_back(this->data[i] - value);
    }

    return d;
}

template <typename T>
template <typename U> vector<T>::operator vector<U>() const {
    vector<U> casted(this->size);

    for (uint64_t i = 0; i < this->size; i++) {
        casted.push_back(static_cast<U>(this->data[i]));
    }

    return casted;
}

template <typename T>
double vector<T>::magnitude() const {
    double m = 0.0;

    for (uint64_t i = 0; i < this->size; i++) {
        m += std::pow(this->data[i], 2);
    }

    return std::sqrt(m);
}

template <typename T>
vector<double> vector<T>::normalize() const {
    double m = this->magnitude();
    assert(m != 0);

    return (*this/m);
}

template <typename T>
void vector<T>::push_back(const T value) {
    if (this->size >= this->capacity) {
        this->widen(this->size + 2);
    }

    this->data[this->size++] = value;
}

template <typename T>
double vector<T>::cos(vector<T> v2) const {
    assert(v2.get_size() == this->size);

    return (v2.normalize() * this->normalize());
}

template <typename T>
uint64_t vector<T>::get_size() const { 
    return this->size;
}

template <typename T>
uint64_t vector<T>::get_capacity() const {
    return this->capacity;
}

template <typename T>
void vector<T>::set_column(bool c) {
    this->column = c;
}

template <typename T>
bool vector<T>::is_column() const {
    return this->column;
}

template <typename T>
void vector<T>::set_size(uint64_t new_size) {
    this->size = new_size;
}

template <typename T>
vector<T> vector<T>::zero(uint64_t s) {
    vector<T> z(s);

    z.set_size(s);
    z.fill(0);

    return z;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const vector<T>& vec) {
    for (uint64_t i = 0; i < vec.get_size(); i++) {
        out << "[" << vec[i] << "]" << (vec.is_column() && (i < vec.get_size()-1) ? "\n" : "");
    }

    return out;
}

#endif