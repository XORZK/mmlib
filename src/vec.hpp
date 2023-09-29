#ifndef VEC_HPP
#define VEC_HPP

#pragma once
#include "list.hpp"
#include "vector.hpp"

#include <assert.h>
#include <iostream>

template <typename T = double> class vec2 {
    static_assert(std::is_arithmetic<T>::value, 
                  "vec2<T> must hold arithmetic types.");
    private:
		T a, b;
        bool column_vec = false;
    public:
        vec2();

        vec2(T v);

        vec2(T x, T y);

        vec2(const vec2<T>& copy);

        ~vec2();

        void x(T new_x);

        T x() const;

        void y(T new_y);

        T y() const;

        void fill(T value);

        T& operator[](int64_t idx);

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

        bool operator!=(const vec2<T>& v2) const;

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
	a = b = static_cast<T>(0);
}

template <typename T> 
vec2<T>::vec2(T v) {
	a = b = v;
}

template <typename T> 
vec2<T>::vec2(T x, T y) {
	this->a = x;
	this->b = y;
}

template <typename T> 
vec2<T>::vec2(const vec2<T>& copy) : column_vec(copy.column()) {
	this->a = copy.x();
	this->b = copy.y();
}

template <typename T> 
vec2<T>::~vec2() {}

template <typename T> 
void vec2<T>::x(T new_x) { this->a = new_x; }

template <typename T> 
T vec2<T>::x() const { return this->a; }

template <typename T> 
void vec2<T>::y(T new_y) { this->b = new_y; }

template <typename T> 
T vec2<T>::y() const { return this->b; }

template <typename T> 
void vec2<T>::fill(T value) {
	this->a = this->b = value;
}

template <typename T> 
T& vec2<T>::operator[](int64_t idx) {
    assert(("Out of Bounds Error: vec2 index out of range", (idx >= 0 && idx <= 1)));

	return (idx == 0 ? a : b);
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
	this->a *= scalar;
	this->b *= scalar;
}

template <typename T> 
void vec2<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0.", (scalar != 0)));

    this->a /= scalar;
    this->b /= scalar;
}

template <typename T> 
void vec2<T>::operator+=(const vec2<T> v2) {
	this->a += v2.x();
	this->b += v2.y();
}

template <typename T> 
void vec2<T>::operator-=(const vec2<T> v2) {
    this->a -= v2.x();
    this->b -= v2.y();
}

template <typename T> 
void vec2<T>::operator+=(const T value) {
    this->a += value;
    this->b += value;
}

template <typename T> 
void vec2<T>::operator-=(const T value) {
	this->a -= value;
	this->b -= value;
}

template <typename T> 
bool vec2<T>::operator==(const vec2<T>& v2) const {
    return (this->x() == v2.x() && this->y() == v2.y());
}

template <typename T> 
bool vec2<T>::operator!=(const vec2<T>& v2) const {
    return (this->x() != v2.x() || this->y() != v2.y());
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


template <typename T = double> class vec3 {
    static_assert(std::is_arithmetic<T>::value, 
                  "vec3<T> must hold arithmetic types.");
    private:
		T a, b, c;
        bool column_vec = false;
    public:
        vec3();

        vec3(T v);

        vec3(T x, T y, T z);

        vec3(const vec2<T> v2, T z);

        vec3(const vec3<T>& copy);

        ~vec3();

        void x(T new_x);

        T x() const;

        void y(T new_y);

        T y() const;

        void z(T new_z);

        T z() const;

        void fill(T value);

        T& operator[](int64_t idx);

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

        bool operator!=(const vec3<T>& v2) const;

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
	a = b = c = 0;
}

template <typename T> 
vec3<T>::vec3(T v) {
	a = b = c = v;
}

template <typename T> 
vec3<T>::vec3(T x, T y, T z) {
	a = x;
	b = y;
	c = z;
}

template <typename T> 
vec3<T>::vec3(const vec2<T> v2, T z) {
	a = v2.x();
	b = v2.y();
	c = z;
}

template <typename T> 
vec3<T>::vec3(const vec3<T>& copy) {
	a = copy.x();
	b = copy.y();
	c = copy.z();
}

template <typename T> 
vec3<T>::~vec3() {
}

template <typename T> 
void vec3<T>::x(T new_x) { a = new_x; }

template <typename T>
T vec3<T>::x() const { return a; }

template <typename T> 
void vec3<T>::y(T new_y) { b = new_y; }

template <typename T> 
T vec3<T>::y() const { return b; }

template <typename T> 
void vec3<T>::z(T new_z) { c = new_z; }

template <typename T> 
T vec3<T>::z() const { return c; }

template <typename T> 
void vec3<T>::fill(T value) {
	a = b = c = value;
}

template <typename T> 
T& vec3<T>::operator[](int64_t idx) {
    assert(("Out of Bounds Error: vec3 index out of range", (idx >= 0 && idx <= 2)));

	return (idx == 0 ? a : (idx == 1) ? b : c);
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
	a *= scalar;
	b *= scalar;
	c *= scalar;
}

template <typename T> 
void vec3<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0.", (scalar != 0)));

	a /= scalar;
	b /= scalar;
	c /= scalar;
}

template <typename T> 
void vec3<T>::operator+=(const vec3<T> v2) {
	a += v2.x();
	b += v2.y();
	c += v2.z();
}

template <typename T> 
void vec3<T>::operator-=(const vec3<T> v2) {
	a -= v2.x();
	b -= v2.y();
	c -= v2.z();
}

template <typename T> 
void vec3<T>::operator+=(const T value) {
	a += value;
	b += value;
	c += value;
}

template <typename T> 
void vec3<T>::operator-=(const T value) {
	a -= value;
	b -= value;
	c -= value;
}

template <typename T>
bool vec3<T>::operator==(const vec3<T>& v2) const {
    return (this->x() == v2.x() && this->y() == v2.y() && this->z() == v2.z());
}

template <typename T>
bool vec3<T>::operator!=(const vec3<T>& v2) const {
    return (this->x() != v2.x() || this->y() != v2.y() || this->z() != v2.z());
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

template <typename T = double> class vec4 {
    static_assert(std::is_arithmetic<T>::value,
                  "vec4<T> must hold arithmetic types");

    private:
        //x, y, z, w
   		T a, b, c, d;     
        bool colum_vec = false;
    public:
        vec4();

        vec4(T v);

        vec4(T x, T y, T z, T w);

        vec4(vec3<T> v3);

        vec4(vec3<T> v3, T w);

        vec4(const vec4<T>& copy);

        ~vec4();

        void x(T new_x);

        T x() const;

        void y(T new_y);

        T y() const;

        void z(T new_z);

        T z() const;

        void w(T new_w);

        T w() const;

        void fill(T value);

        T& operator[](int64_t idx);

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

        bool operator!=(const vec4<T>& v2) const;

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
	a = b = c = d = 0;
}

template <typename T> 
vec4<T>::vec4(T v) {
	a = b = c = v;
	d = static_cast<T>(1);
}

template <typename T> 
vec4<T>::vec4(T x, T y, T z, T w) {
	a = x;
	b = y;
	c = z;
	d = w;
}

template <typename T> 
vec4<T>::vec4(vec3<T> v3) {
	a = v3.x();
	b = v3.y();
	c = v3.z();
	d = static_cast<T>(1);
}

template <typename T>
vec4<T>::vec4(vec3<T> v3, T v) {
	a = v3.x();
	b = v3.y();
	c = v3.z();
	d = v;
}

template <typename T>
vec4<T>::vec4(const vec4<T>& copy) {
	a = copy.x();
	b = copy.y();
	c = copy.z();
	d = copy.w();
}

template <typename T> 
vec4<T>::~vec4() {
}

template <typename T> 
void vec4<T>::x(T new_x) { a = new_x; }

template <typename T> 
T vec4<T>::x() const { return a; }

template <typename T> 
void vec4<T>::y(T new_y) { b = new_y; }

template <typename T> 
T vec4<T>::y() const { return b; }

template <typename T> 
void vec4<T>::z(T new_z) { c = new_z; }

template <typename T> 
T vec4<T>::z() const { return c; }

template <typename T> 
void vec4<T>::w(T new_w) { d = new_w; }

template <typename T> 
T vec4<T>::w() const { return d; }

template <typename T> 
void vec4<T>::fill(T value) { 
	a = b = c = d = value;
}

template <typename T> 
T& vec4<T>::operator[](int64_t idx) {
    assert(("Out of Bounds Error: vec4 index out of range", (idx >= 0 && idx <= 3)));

	return (idx == 0 ? a : idx == 1 ? b : idx == 2 ? c : d);
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
		(*this)[k] *= scalar;
}

template <typename T> 
void vec4<T>::operator/=(const T scalar) {
    assert(("Math Error: Cannot divide by 0.", (scalar != 0)));

    for (int64_t k = 0; k < this->size(); k++) 
		(*this)[k] /= scalar;
}

template <typename T> 
void vec4<T>::operator+=(const T value) {
    for (int64_t k = 0; k < this->size(); k++) 
        (*this)[k] += value;
}

template <typename T> 
void vec4<T>::operator-=(const T value) {
    for (int64_t k = 0; k < this->size(); k++) 
        (*this)[k] -= value;
}

template <typename T> 
void vec4<T>::operator+=(const vec4<T> v2) {
    for (int64_t k = 0; k < this->size(); k++) 
		(*this)[k] += v2[k];
}

template <typename T> 
void vec4<T>::operator-=(const vec4<T> v2) {
    for (int64_t k = 0; k < this->size(); k++) 
        (*this)[k] -= v2[k];
}

template <typename T>
bool vec4<T>::operator==(const vec4<T>& v2) const {
    return (this->x() == v2.x() && this->y() == v2.y() && 
            this->z() == v2.z() && this->w() == v2.w());
}

template <typename T>
bool vec4<T>::operator!=(const vec4<T>& v2) const {
    return (this->x() != v2.x() && this->y() != v2.y() && 
            this->z() != v2.z() && this->w() != v2.w());
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

inline double cross(const vec2<double> &a,
				    const vec2<double> &b,
				    const vec2<double> &c) {
	// (x2 - x1)(y3 - y1) - (y2 - y1)(x3 - x1)
	return ((b.x() - a.x()) * (c.y() - a.y())) - 
		   ((b.y() - a.y()) * (c.x() - a.x()));
}

// Checks if point c is to the left of the line segment formed by points a,b.
inline int64_t direction(const vec2<double> &a,	
						 const vec2<double> &b,
						 const vec2<double> &c) {
	vec2 control = vec2(a.x()-1, a.y()); 

	// Control is to the left of the segment a,b.
	// Thus, if fc and sc have the same sign, then 
	// they are both to the left of a,b. Otherwise, 
	// fc is to the right of a,b.
	double fc = cross(a, b, c), sc = cross(a, b, control);

	// 1 if left.
	// -1 if right.
	return (sc == 0 ? (c.y() > a.y() ? 1 : -1) : (fc > 0) == (sc > 0) ? 1 : -1);
}

inline int64_t direction(const vec3<double> &a,
					     const vec3<double> &b,
						 const vec3<double> &c,
						 const vec3<double> &d) {
	vec3<double> q = (b - a).cross(c-a);

	int64_t D = (d - a) * q;

	return (D > 0 ? 1 : D == 0 ? 0 : -1);
}

template <typename T> 
inline T centroid(list<T> &l) {
	T c = l[0];

	linked_node<T> *node = l.front()->next();

	for (int64_t k = 1; k < l.size(); k++) {
		c += node->value();
		node = node->next();
	}

	return c;
}

#endif
