#ifndef VEC_HPP
#define VEC_HPP

#pragma once
#include "vector.hpp"

#include <assert.h>
#include <iostream>

template <typename T> class vec2 {
    static_assert(std::is_arithmetic<T>::value, 
                  "vec2<T> must hold arithmetic types.");
    private:
        T* dat;
        bool column_vec = false;
    public:
        vec2();

        vec2(T v);

        vec2(T x, T y);

        vec2(const vec2<T>& copy);

        ~vec2() {}

        void x(T new_x);

        T x() const;

        void y(T new_y);

        T y() const;

        void fill(T value);

        T& operator[](int64_t idx) const;

        T operator*(const vec2<T> v2) const;

        vec2<T> operator*(const T scalar) const;

        vec2<T> operator/(const T scalar) const;

        vec2<T> operator+(const T value) const;

        vec2<T> operator-(const T value) const;

        vec2<T> operator+(const vec2<T> v2) const;

        vec2<T> operator-(const vec2<T> v2) const;

        void operator*=(const T scalar);

        void operator/=(const T scalar);

        void operator+=(const vec2<T> v2);

        void operator-=(const vec2<T> v2);

        void operator+=(const T value);

        void operator-=(const T value);

        bool operator==(const vec2<T>& v2) const;

        template <typename U>
        operator vec2<U>() const;

        operator vector<T>() const;

        double magnitude() const;

        vec2<double> normalize() const;

        int64_t size() const;

        void swap();

        double cos(vec2<T>& v2) const;

        void column(bool c);

        bool column() const;

        bool contains(T value) const;

        static vec2<T> zero();

        static vec2<T> i();

        static vec2<T> j();
};

template <typename T> 
vec2<T>::vec2() {
    this->dat = static_cast<T*>(new T[]{static_cast<T>(0), static_cast<T>(0)});
}

template <typename T> 
vec2<T>::vec2(T v) {
    this->dat = static_cast<T*>(new T[]{v,v});
}

template <typename T> 
vec2<T>::vec2(T x, T y) {
    this->dat = static_cast<T*>(new T[]{x,y});
}

template <typename T> 
vec2<T>::vec2(const vec2<T>& copy) : column_vec(copy.column()) {
    this->dat = static_cast<T*>(new T[]{copy.x(), copy.y()});
}

template <typename T> 
void vec2<T>::x(T new_x) { this->dat[0] = new_x; }

template <typename T> 
T vec2<T>::x() const { return this->dat[0]; }

template <typename T> 
void vec2<T>::y(T new_y) { this->dat[1] = new_y; }

template <typename T> 
T vec2<T>::y() const { return this->dat[1]; }

template <typename T> 
void vec2<T>::fill(T value) {
    this->dat = static_cast<T*>(malloc(2 * sizeof(T)));
    std::fill(this->dat, this->dat + 2, value);
}

template <typename T> 
T& vec2<T>::operator[](int64_t idx) const {
    assert(("Out of Bounds Error: vec2 index out of range", (idx >= 0 && idx <= 1)));

    return (this->dat[idx]);
}

template <typename T> 
T vec2<T>::operator*(const vec2<T> v2) const {
    return (this->x() * v2.x() + this->y() * v2.y()); 
}

template <typename T> 
vec2<T> vec2<T>::operator*(const T scalar) const {
    vec2<T> scaled(this->x() * scalar, this->y() * scalar);
    return scaled;
}

template <typename T> 
vec2<T> vec2<T>::operator/(const T scalar) const {
    assert(("Math Error: Cannot divide by 0.", (scalar != 0)));

    vec2<T> scaled(this->x() / scalar, this->y() / scalar);

    return scaled;
}

template <typename T> 
vec2<T> vec2<T>::operator+(const T value) const {
    vec2<T> shifted(this->x() + value, this->y() + value);

    return shifted;
}

template <typename T> 
vec2<T> vec2<T>::operator-(const T value) const {
    vec2<T> shifted(this->x() - value, this->y() - value);

    return shifted;
}

template <typename T> 
vec2<T> vec2<T>::operator+(const vec2<T> v2) const {
    vec2<T> sum(this->x() + v2.x(), this->y() + v2.y());

    return sum;
}

template <typename T> 
vec2<T> vec2<T>::operator-(const vec2<T> v2) const {
    vec2<T> diff(this->x() - v2.x(), this->y() - v2.y());

    return diff;
}

template <typename T> 
void vec2<T>::operator*=(const T scalar) {
    this->dat[0] *= scalar;
    this->dat[1] *= scalar;
}

template <typename T> 
void vec2<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0.", (scalar != 0)));

    this->dat[0] /= scalar;
    this->dat[1] /= scalar;
}

template <typename T> 
void vec2<T>::operator+=(const vec2<T> v2) {
    this->dat[0] -= v2.x();
    this->dat[1] -= v2.y();
}

template <typename T> 
void vec2<T>::operator-=(const vec2<T> v2) {
    this->dat[0] -= v2.x();
    this->dat[1] -= v2.y();
}

template <typename T> 
void vec2<T>::operator+=(const T value) {
    this->dat[0] += value;
    this->dat[1] += value;
}

template <typename T> 
void vec2<T>::operator-=(const T value) {
    this->dat[0] -= value;
    this->dat[1] -= value;
}

template <typename T> 
bool vec2<T>::operator==(const vec2<T>& v2) const {
    return (this->x() == v2.x() && this->y() == v2.y());
}

template <typename T> template <typename U>
vec2<T>::operator vec2<U>() const {
    vec2<U> casted(static_cast<U>(this->x()), static_cast<U>(this->y()));
    return casted;
}

template <typename T> 
vec2<T>::operator vector<T>() const {
    vector<T> casted(static_cast<T*>(new T[]{this->x(), this->y()}), 2);
    return casted;
}

template <typename T> 
double vec2<T>::magnitude() const {
    return std::sqrt((this->x() * this->x()) + (this->y() * this->y()));
}

template <typename T> 
vec2<double> vec2<T>::normalize() const {
    double M = this->magnitude();

    assert(("Math Error: Cannot normalize a vector with a magnitude of 0.", (M != 0)));

    vec2<double> casted = static_cast<vec2<double>>(*this);

    return (casted/M);
}

template <typename T> 
void vec2<T>::swap() {
    T t = this->x();
    this->x(this->y());
    this->y(t);
}

template <typename T> 
int64_t vec2<T>::size() const { return 2; }

template <typename T> 
double vec2<T>::cos(vec2<T>& v2) const {
    return (v2.normalize() * this->normalize());
}

template <typename T> 
void vec2<T>::column(bool c) { this->column_vec = c; }

template <typename T> 
bool vec2<T>::column() const { return this->column_vec; }

template <typename T> 
bool vec2<T>::contains(T value) const {
    return (this->x() == value || this->y() == value);
}

template <typename T> 
vec2<T> vec2<T>::zero() {
    vec2<T> z(static_cast<T>(0), static_cast<T>(0));

    return z;
}

template <typename T> 
vec2<T> vec2<T>::i() {
    vec2<T> i(static_cast<T>(1), static_cast<T>(0));

    return i;
}

template <typename T> 
vec2<T> vec2<T>::j() {
    vec2<T> j(static_cast<T>(0), static_cast<T>(1));

    return j;
}

template <typename T> 
std::ostream& operator<<(std::ostream& out, const vec2<T> vec) {
    return (out << "vec2: " << '<' << vec.x() << ',' << vec.y() << '>');
}


template <typename T> class vec3 {
    static_assert(std::is_arithmetic<T>::value, 
                  "vec3<T> must hold arithmetic types.");
    private:
        T* dat;
        bool column_vec = false;
    public:
        vec3();

        vec3(T v);

        vec3(T x, T y, T z);

        vec3(const vec2<T> v2, T z);

        vec3(const vec3<T>& copy);

        ~vec3() {}

        void x(T new_x);

        T x() const;

        void y(T new_y);

        T y() const;

        void z(T new_z);

        T z() const;

        void fill(T value);

        T& operator[](int64_t idx) const;

        T operator*(const vec3<T> v2) const;

        vec3<T> operator*(const T scalar) const;

        vec3<T> operator/(const T scalar) const;

        vec3<T> operator+(const T value) const;

        vec3<T> operator-(const T value) const;

        vec3<T> operator+(const vec3<T> v2) const;

        vec3<T> operator-(const vec3<T> v2) const;

        void operator*=(const T scalar);

        void operator/=(const T scalar);

        void operator+=(const vec3<T> v2);

        void operator-=(const vec3<T> v2);

        void operator+=(const T value);

        void operator-=(const T value);

        bool operator==(const vec3<T>& v2) const;

        template <typename U>
        operator vec3<U>() const;

        operator vector<T>() const;

        vec3<T> cross(const vec3<T> v2) const;

        double magnitude() const;

        vec3<double> normalize() const;

        double cos(vec3<T>& v2) const;

        int64_t size() const;

        void column(bool c);

        bool column() const;

        bool contains(T value) const;

        static vec3<T> zero();

        static vec3<T> i();

        static vec3<T> j();

        static vec3<T> k();
};

template <typename T> 
vec3<T>::vec3() {
    this->dat = static_cast<T*>(new T[]{static_cast<T>(0),
                                        static_cast<T>(0),
                                        static_cast<T>(0)});
}

template <typename T> 
vec3<T>::vec3(T v) {
    this->dat = static_cast<T*>(new T[]{v,v,v});
}

template <typename T> 
vec3<T>::vec3(T x, T y, T z) {
    this->dat = static_cast<T*>(new T[]{x,y,z});
}

template <typename T> 
vec3<T>::vec3(const vec2<T> v2, T z) {
    this->dat = static_cast<T*>(new T[]{v2.x(),v2.y(),z});
}

template <typename T> 
vec3<T>::vec3(const vec3<T>& copy) {
    this->dat = static_cast<T*>(new T[]{copy.x(), copy.y(), copy.z()});
}

template <typename T> 
void vec3<T>::x(T new_x) { this->dat[0] = new_x; }

template <typename T>
T vec3<T>::x() const { return this->dat[0]; }

template <typename T> 
void vec3<T>::y(T new_y) { this->dat[1] = new_y; }

template <typename T> 
T vec3<T>::y() const { return this->dat[1]; }

template <typename T> 
void vec3<T>::z(T new_z) { this->dat[2] = new_z; }

template <typename T> 
T vec3<T>::z() const { return this->dat[2]; }

template <typename T> 
void vec3<T>::fill(T value) {
    this->dat = static_cast<T*>(malloc(3 * sizeof(T)));
    std::fill(this->dat, this->dat + 3, value);
}

template <typename T> 
T& vec3<T>::operator[](int64_t idx) const {
    assert(("Out of Bounds Error: vec3 index out of range", (idx >= 0 && idx <= 2)));

    return this->dat[idx];
}

template <typename T> 
T vec3<T>::operator*(const vec3<T> v2) const {
    return (this->x() * v2.x() + 
            this->y() * v2.y() +
            this->z() * v2.z());
}

template <typename T> 
vec3<T> vec3<T>::operator*(const T scalar) const {
    vec3<T> scaled(this->x() * scalar,    
                   this->y() * scalar,
                   this->z() * scalar);

    return scaled;
}

template <typename T> 
vec3<T> vec3<T>::operator/(const T scalar) const {
    assert(("Math Error: Cannot divide by 0", (scalar != 0)));

    vec3<T> scaled(this->x() / scalar,
                   this->y() / scalar,
                   this->z() / scalar);

    return scaled;
}

template <typename T> 
vec3<T> vec3<T>::operator+(const T value) const {
    vec3<T> shifted(this->x() + value,
                    this->y() + value,
                    this->z() + value);

    return shifted;
}

template <typename T> 
vec3<T> vec3<T>::operator-(const T value) const {
    vec3<T> shifted(this->x() - value,
                    this->y() - value,
                    this->z() - value);

    return shifted;
}

template <typename T> 
vec3<T> vec3<T>::operator+(const vec3<T> v2) const {
    vec3<T> sum(this->x() + v2.x(),
                this->y() + v2.y(),
                this->z() + v2.z());

    return sum;
}

template <typename T> 
vec3<T> vec3<T>::operator-(const vec3<T> v2) const {
    vec3<T> diff(this->x() - v2.x(),
                 this->y() - v2.y(),
                 this->z() - v2.z());

    return diff;
}

template <typename T> 
void vec3<T>::operator*=(const T scalar) {
    this->dat[0] *= scalar;
    this->dat[1] *= scalar;
    this->dat[2] *= scalar;
}

template <typename T> 
void vec3<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0.", (scalar != 0)));

    this->dat[0] /= scalar;
    this->dat[1] /= scalar;
    this->dat[2] /= scalar;
}

template <typename T> 
void vec3<T>::operator+=(const vec3<T> v2) {
    this->dat[0] += v2.x();
    this->dat[1] += v2.y();
    this->dat[2] += v2.z();
}

template <typename T> 
void vec3<T>::operator-=(const vec3<T> v2) {
    this->dat[0] -= v2.x();
    this->dat[1] -= v2.y();
    this->dat[2] -= v2.z();
}

template <typename T> 
void vec3<T>::operator+=(const T value) {
    this->dat[0] += value;
    this->dat[1] += value;
    this->dat[2] += value;
}

template <typename T> 
void vec3<T>::operator-=(const T value) {
    this->dat[0] -= value;
    this->dat[1] -= value;
    this->dat[2] -= value;
}

template <typename T>
bool vec3<T>::operator==(const vec3<T>& v2) const {
    return (this->x() == v2.x() && this->y() == v2.y() && this->z() == v2.z());
}

template <typename T> template <typename U>
vec3<T>::operator vec3<U>() const {
    vec3<U> casted(static_cast<U>(this->x()),
                   static_cast<U>(this->y()),
                   static_cast<U>(this->z()));
    return casted;
}

template <typename T> 
vec3<T>::operator vector<T>() const {
    vector<T> casted(static_cast<T*>(new T[]{this->x(), this->y(), this->z()}), 3);
    return casted;
}

template <typename T> 
vec3<T> vec3<T>::cross(const vec3<T> v2) const {
    vec3<T> i = vec3<T>::i(), j = vec3<T>::j(), k = vec3<T>::k();

    return (i * (this->y() * v2.z() - this->z() * v2.y())) + 
           (j * (this->z() * v2.x() - this->x() * v2.z())) + 
           (k * (this->x() * v2.y() - this->y() * v2.x()));
}

template <typename T> 
double vec3<T>::magnitude() const {
    return std::sqrt((this->x() * this->x()) + 
                     (this->y() * this->y()) + 
                     (this->z() * this->z()));
}

template <typename T> 
vec3<double> vec3<T>::normalize() const {
    double M = this->magnitude();

    assert(("Math Error: Cannot normalize a vector with a magnitude of 0.", (M != 0)));

    vec3<double> casted = static_cast<vec3<double>>(*this);

    return (casted/M);
}

template <typename T> 
double vec3<T>::cos(vec3<T>& v2) const {
    return (v2.normalize() * this->normalize());
}

template <typename T> 
int64_t vec3<T>::size() const { return 3; }

template <typename T> 
void vec3<T>::column(bool c) { this->column_vec = c; }

template <typename T> 
bool vec3<T>::column() const { return this->column_vec; }

template <typename T>
bool vec3<T>::contains(T value) const {
    return (this->x() == value || this->y() == value || this->z() == value);
}

template <typename T> 
vec3<T> vec3<T>::zero() {
    vec3<T> z(static_cast<T>(0),
              static_cast<T>(0),
              static_cast<T>(0));

    return z;
}

template <typename T> 
vec3<T> vec3<T>::i() {
    vec3<T> i(static_cast<T>(1),
              static_cast<T>(0),
              static_cast<T>(0));

    return i;
}

template <typename T> 
vec3<T> vec3<T>::j() {
    vec3<T> j(static_cast<T>(0),
              static_cast<T>(1),
              static_cast<T>(0));

    return j;
}

template <typename T> 
vec3<T> vec3<T>::k() {
    vec3<T> k(static_cast<T>(0),
              static_cast<T>(0),
              static_cast<T>(1));

    return k;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const vec3<T>& vec) {
    return (out << "vec3: " << '<' << vec.x() << ',' << vec.y() << ',' << vec.z() << '>');
}

template <typename T> class vec4 {
    static_assert(std::is_arithmetic<T>::value,
                  "vec4<T> must hold arithmetic types");

    private:
        //x, y, z, w
        T* dat;
        bool colum_vec = false;
    public:
        vec4();

        vec4(T v);

        vec4(T x, T y, T z, T w);

        vec4(vec3<T> v3);

        vec4(vec3<T> v3, T w);

        vec4(const vec4<T>& copy);

        ~vec4() {}

        void x(T new_x);

        T x() const;

        void y(T new_y);

        T y() const;

        void z(T new_z);

        T z() const;

        void w(T new_w);

        T w() const;

        void fill(T value);

        T& operator[](int64_t idx) const;

        T operator*(const vec4<T> v2) const;

        vec4<T> operator*(const T scalar) const;

        vec4<T> operator/(const T scalar) const;

        vec4<T> operator+(const T value) const;

        vec4<T> operator-(const T value) const;

        vec4<T> operator+(const vec4<T> v2) const;

        vec4<T> operator-(const vec4<T> v2) const;

        void operator*=(const T scalar);

        void operator/=(const T scalar);

        void operator+=(const vec4<T> v2);

        void operator-=(const vec4<T> v2);

        void operator+=(const T value);

        void operator-=(const T value);

        bool operator==(const vec4<T>& v2) const;

        template <typename U>
        operator vec4<U>() const;

        operator vector<T>() const;

        double magnitude() const;

        vec4<double> normalize() const;

        double cos(vec4<T>& v2) const;

        int64_t size() const;

        void column(bool c);

        bool column() const;

        bool contains(T value) const;

        static vec4<T> zero();
};

template <typename T> 
vec4<T>::vec4() {
    this->dat = static_cast<T*>(new T[]{static_cast<T>(0), 
                                        static_cast<T>(0),
                                        static_cast<T>(0),
                                        static_cast<T>(0)});
}

template <typename T> 
vec4<T>::vec4(T v) {
    this->dat = static_cast<T*>(new T[]{v, v, v, static_cast<T>(1)});
}

template <typename T> 
vec4<T>::vec4(T x, T y, T z, T w) {
    this->dat = static_cast<T*>(new T[]{x,y,z,w});
}

template <typename T> 
vec4<T>::vec4(vec3<T> v3) {
    this->dat = static_cast<T*>(new T[]{v3.x(), v3.y(), v3.z(), static_cast<T>(1)});
}

template <typename T>
vec4<T>::vec4(vec3<T> v3, T v) {
    this->dat = static_cast<T*>(new T[]{v3.x(), v3.y(), v3.z(), v});
}

template <typename T>
vec4<T>::vec4(const vec4<T>& copy) {
    this->dat = static_cast<T*>(new T[]{copy.x(), copy.y(), copy.z(), copy.w()});
}

template <typename T> 
void vec4<T>::x(T new_x) { this->dat[0] = new_x; }

template <typename T> 
T vec4<T>::x() const { return this->dat[0]; }

template <typename T> 
void vec4<T>::y(T new_y) { this->dat[1] = new_y; }

template <typename T> 
T vec4<T>::y() const { return this->dat[1]; }

template <typename T> 
void vec4<T>::z(T new_z) { this->dat[2] = new_z; }

template <typename T> 
T vec4<T>::z() const { return this->dat[2]; }

template <typename T> 
void vec4<T>::w(T new_w) { this->dat[3] = new_w; }

template <typename T> 
T vec4<T>::w() const { return this->dat[3]; }

template <typename T> 
void vec4<T>::fill(T value) { 
    this->dat = static_cast<T*>(malloc(4 * sizeof(T)));
    std::fill(this->dat, this->dat + 4, value);
}

template <typename T> 
T& vec4<T>::operator[](int64_t idx) const {
    assert(("Out of Bounds Error: vec4 index out of range", (idx >= 0 && idx <= 3)));

    return this->dat[idx];
}

template <typename T> 
T vec4<T>::operator*(const vec4<T> v2) const {
    return (this->x() * v2.x() + 
            this->y() * v2.y() + 
            this->z() * v2.z() + 
            this->w() * v2.w());
}

template <typename T> 
vec4<T> vec4<T>::operator*(const T scalar) const {
    vec4<T> scaled(this->x() * scalar,
                   this->y() * scalar,
                   this->z() * scalar,
                   this->w() * scalar);

    return scaled;
}

template <typename T> 
vec4<T> vec4<T>::operator/(const T scalar) const {
    assert(("Math Error: Cannot divide by 0.", (scalar != 0)));

    vec4<T> scaled(this->x() / scalar,
                   this->y() / scalar,
                   this->z() / scalar,
                   this->w() / scalar);

    return scaled;
}

template <typename T> 
vec4<T> vec4<T>::operator+(const T value) const {
    vec4<T> sum(this->x() + value,
                this->y() + value,
                this->z() + value,
                this->w() + value);

    return sum;
}

template <typename T>
vec4<T> vec4<T>::operator-(const T value) const {
    vec4<T> diff(this->x() - value,
                 this->y() - value,
                 this->z() - value,
                 this->w() - value);

    return diff;
}

template <typename T>
vec4<T> vec4<T>::operator+(const vec4<T> v2) const {
    vec4<T> sum(this->x() + v2.x(),
                this->y() + v2.y(),
                this->z() + v2.z(),
                this->w() + v2.w());

    return sum;
}

template <typename T>
vec4<T> vec4<T>::operator-(const vec4<T> v2) const {
    vec4<T> diff(this->x() - v2.x(),
                 this->y() - v2.y(),
                 this->z() - v2.z(),
                 this->w() - v2.w());

    return diff;
}

template <typename T> 
void vec4<T>::operator*=(const T scalar) {
    for (int64_t k = 0; k < this->size(); k++) 
        this->dat[k] *= scalar;
}

template <typename T> 
void vec4<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0.", (scalar != 0)));

    for (int64_t k = 0; k < this->size(); k++) 
        this->dat[k] /= scalar;
}

template <typename T> 
void vec4<T>::operator+=(const T value) {
    for (int64_t k = 0; k < this->size(); k++) 
        this->dat[k] += value;
}

template <typename T> 
void vec4<T>::operator-=(const T value) {
    for (int64_t k = 0; k < this->size(); k++) 
        this->dat[k] -= value;
}

template <typename T> 
void vec4<T>::operator+=(const vec4<T> v2) {
    for (int64_t k = 0; k < this->size(); k++) 
        this->dat[k] += v2[k];
}

template <typename T> 
void vec4<T>::operator-=(const vec4<T> v2) {
    for (int64_t k = 0; k < this->size(); k++) 
        this->dat[k] -= v2[k];
}

template <typename T>
bool vec4<T>::operator==(const vec4<T>& v2) const {
    return (this->x() == v2.x() && this->y() == v2.y() && 
            this->z() == v2.z() && this->w() == v2.w());
}

template <typename T> template <typename U>
vec4<T>::operator vec4<U>() const {
    vec4<U> casted(static_cast<U>(this->x()),
                   static_cast<U>(this->y()),
                   static_cast<U>(this->z()),
                   static_cast<U>(this->w()));

    return casted;
}

template <typename T> 
vec4<T>::operator vector<T>() const {
    vector<T> casted(static_cast<T*>(new T[]{this->x(), this->y(), 
                                             this->z(), this->w()}), 4);
    return casted;
}

template <typename T> 
double vec4<T>::magnitude() const {
    return std::sqrt((this->x() * this->x()) + 
                     (this->y() * this->y()) + 
                     (this->z() * this->z()) + 
                     (this->w() * this->w()));
}

template <typename T> 
vec4<double> vec4<T>::normalize() const {
    double M = this->magnitude();
    assert(("Math Error: Cannot normalize a vector with a magnitude of 0.", (M != 0)));

    vec4<double> casted = static_cast<vec4<double>>(*this);
    return (casted/M);
}

template <typename T> 
double vec4<T>::cos(vec4<T>& v2) const {
    return (this->normalize() * v2.normalize());
}

template <typename T> 
int64_t vec4<T>::size() const { return 4; }

template <typename T> 
void vec4<T>::column(bool c) { this->column_vec = c; }

template <typename T> 
bool vec4<T>::column() const { return this->column_vec; }

template <typename T> 
bool vec4<T>::contains(T value) const {
    return (this->x() == value || 
            this->y() == value ||
            this->z() == value || 
            this->w() == value);
}

template <typename T> 
vec4<T> vec4<T>::zero() {
    vec4<T> z(static_cast<T>(0),
              static_cast<T>(0),
              static_cast<T>(0),
              static_cast<T>(0));

    return z;
}

template <typename T> 
std::ostream& operator<<(std::ostream& out, const vec4<T>& vec) {
    return (out << "vec4: " << '<' << vec.x() << ',' << vec.y() << ',' << vec.z() << ',' << vec.w() << '>');
}

template <typename T>
inline double distance(vec2<T> v1, vec2<T> v2) {
    return std::sqrt(std::pow(v1.x() - v2.x(), 2) + std::pow(v1.y() - v2.y(), 2));
}

template <typename T> 
inline double distance(vec3<T> v1, vec3<T> v2) {
    return std::sqrt(std::pow(v1[0] - v2[0], 2) + std::pow(v1[1] - v2[1], 2) + std::pow(v1[2] - v2[2], 2));
}

// Computes determinant of: [v1.x v2.x] 
//                          [v1.y v2.y]
template <typename T>
T det(const vec2<T> v1, const vec2<T> v2) {
    return (v1.x() * v2.y()) - (v2.x() * v1.y());
}

template <typename T>
T det(const vec3<T> v1, const vec3<T> v2) {
    return (v1.x() * v2.y()) - (v2.x() * v1.y());
}

#endif
