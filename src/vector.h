#ifndef VECTOR_H
#define VECTOR_H

#pragma once
#include "MACROS.h"
#include "rational.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <type_traits>

/** A generic vector class which can be used to hold any type. */
template <typename T>
class vector {
    private:
        uint64_t size; ///< The current size (amount of items) in the vector.
        uint64_t capacity; ///< The allocated capacity of the array representing the vector.
        T* data; ///< The array used to represent the vector.
        bool column; ///< A boolean value which denotes whether or not the vector is a column or row vector.
       
        /** Performs widening on the array containing the items of the vector.
         * @param new_capacity The new capacity of the vector.
         * */
        void widen(uint64_t new_capacity);
    public:
        /** Instantiates an empty vector with no size. */
        vector();

        /** Instantiates an empty vector with an initial capacity. 
         * @param init_capacity The initial capacity of the vector.
         * */
        vector(uint64_t init_capacity);

        /** Instantiates a vector with items from another array of type M.
         * @param d The array of items to instantiate the vector with.
         * @param data_size The size of the array.
         * */
        template <typename M>
        vector(M* d, uint64_t data_size);

        /** Instantiates a vector with items from another array of same generic type T.
         * @param d The array of items to instantiate the vector with.
         * @param data_size The size of the array.
         * */
        vector(T* d, uint64_t data_size);

        /** A copy constructor which instantiates a vector using the contents of another vector.
         * @param v2 The vector to be copied.
         * */
        vector(const vector<T>& v2);

        ~vector() {}

        /** Fills the vector using a given value.
         * @param value The value to fill the vector with.
         * */
        void fill(T value);

        /** Overrides the subscript operator to allow for vector indexing.
         * @param idx The index to get from the vector.
         * @return The entry at the specified index.
         * */
        T& operator[](uint64_t idx) const;

        /** Computes the dot product of two vectors.
         * The dot product between two vectors \f$u, v \in \mathbb{R}^n\f$ is defined as:
         * \f$ \sum_{i=1}^{n} v_i * u_i \f$
         * @param v2 The vector to find the dot product of.
         * @return The dot product between two vectors \f$u,v\f$.
         * */
        T operator*(const vector<T> v2) const;

        /** Returns a scaled vector multiplied by some constant \f$k \in \mathbb{R} \f$.
         * @param scalar The scalar to multiply the vector by.
         * @return The scaled vector multiplied by some constant.
         * */
        vector<T> operator*(const T scalar) const;

        /** Returns a scaled vector divided by some constant \f$k \in \mathbb{R}, k \ne 0 \f$ 
         * @param scalar The scalar to divide the vector by.
         * @return The scaled vector divided by some constant.
         * */
        vector<double> operator/(const double scalar) const;

        /** Modifies the current vector and divides it by some scalar.
         * @param The scalar to divide the vector by.
         * */
        template <typename U>
        void operator/=(const U scalar);

        /** Returns the sum of two vectors.
         * The sum of two vectors \f$u, v \in \mathbb{R}^n\f$ is defined as:
         * \f$(u + v)_i = u_i + v_i,  \forall 1 \le i \le n \f$
         * @param v2 The vector to add to.
         * @return The sum of the two vectors.
         * */
        vector<T> operator+(const vector<T> v2) const;

        /** Modifies the current vector and adds it by another vector.
         * @param v2 The vector to add to.
         * */
        void operator+=(const vector<T> v2);

        /** Returns the current vector with every entry shifted by some constant \f$k \in \mathbb{R}\f$
         * @param value The value to shift every entry in the vector in the positive direction by.
         * @return The vector shifted in the positive direction.
         * */
        vector<T> operator+(const T value) const;

        /** Returns the difference between two vectors.
         * The difference between two vectors \f$u, v \in \mathbb{R}^n \f$ is defined as:
         * \f$(u-v)_i = u_i - v_i, \forall 1 \le i \le n \f$
         * @param v2 The vector to subtract by.
         * @return The difference between the two vectors.
         * */
        vector<T> operator-(const vector<T> v2) const;

        /** Returns the current vector with every entry shifted by some constant \f$k \in \mathbb{R}\f$
         * @param value The value to shift every entry in the vector in the negative direction by.
         * @return The vector shifted in the negative direction.
         * */
        vector<T> operator-(const T value) const;

        /** Checks whether or not two vectors are equal.
         * @param v2 The vector of which equality is checked.
         * @return A boolean value denoting whether or not the two vectors are equal.
         * */
        bool operator==(const vector<T>& v2) const;

        /** Typecasts the vector from type T to type U. 
         * @return The typecasted vector.
         * */
        template <typename U>
        operator vector<U>() const;

        /** Computes the dot product between two 3-D vectors. 
         * The dot product of two vectors \f$u, v \in \mathbb{R}^3\f$ has 
         * a magnitude equivalent to the area of the parallelogram formed by 
         * vectors $u,v$. The orientation of $u \times v$ is determined
         * by the Right Hand Rule.
         * @param v2 The vector to compute the dot product with.
         * @return The dot product of the two vectors.
         * */
        vector<T> cross(const vector<T> v2) const;

        /** Computes the magnitude of the vector.
         * The magnitude of the vector \f$v \in \mathbb{R}^n\f$is defined to be:
         * \f$\|v\| = \sqrt{\sum_{i=1}^{n} v_i^2}\f$
         * @return The magnitude of the vector.
         * */
        double magnitude() const;

        /** Normalizes the vector so that the magnitude of the vector is 1.
         * The normalized vector is defined as: \f$\frac{v}{\|v\|}\f$
         * @return The normalized form of the current vector, with magnitude 1.
         * */
        vector<double> normalize() const;

        /** Removes the entry at a specific index within the vector.
         * @param idx The index of the entry to remove.
         * */
        void remove(uint64_t idx);

        /** Removes the last entry (tail) of the vector. */
        void pop();

        /** Appends an entry to the back (tail) of the vector.
         * @param value The value to push to the back of the vector.
         * */
        void push_back(const T value); 

        /** Swaps two indices within the vector.
         * Likewise, this can also be done using a permutation matrix.
         * @param i The first index of the entry to be swapped.
         * @param j The second index of the entry to be swapped.
         * */
        void swap(uint64_t i, uint64_t j);

        /** Computes the cosine of the angle between two vectors.
         * This is done using the definition of the dot product:
         * For any two vectors, \f$u,v \in \mathbb{R}^n\f$:
         * \f$ cos(\theta) = \frac{u \cdot v}{\|u\| \times \|v\|} \f$
         * Where \f$\theta\f$ is the angle between \f$u, v\f$
         * */
        double cos(vector<T> v2) const;

        /** Returns the size of the vector (amount of items currently held).
         * @return The size of the vector.
         * */
        uint64_t get_size() const;

        /** Returns the capacity of the vector (amount of items which can be held without reallocation). 
         * @return The capacity of the vector.
         * */
        uint64_t get_capacity() const;

        /** Redefines the size of the vector,
         * @param new_size The new size of the vector.
         * */
        void set_size(uint64_t new_size);

        /** A setter method which determines whether or not the vector is a column vector.
         * @param c A boolean value denoting whether or not the vector is a column vector.
         * */
        void set_column(bool c);

        /** A getter method which denotes whether or not the vector is a column vector.
         * @return A boolean value denoting whether or not the vector is a column vector.
         * */
        bool is_column() const;

        /** Performs a linear search to determine whether or not the vector 
         * contains a specific value.
         * @param value The value to search for within the vector.
         * @return A boolean value determining whether or not the vector 
         *         contains a specific value.
         * */
        bool contains(T value) const;

        /** Performs a linear search and returns the first instance of a value 
         * in the vector. If the value does not exist within the vector,
         * the function returns -1.
         * @param value The value to search for within the vector.
         * @return The index of the first instance of a value in the vector.
         *         If the value is not contained within the vector, -1 is
         *         returned.
         * */
        int64_t index(T value) const;

        /** Returns a slice (sub-vector) of the vector.
         * The function slices from the 0th index upto, but not including,
         * the final specificed index.
         * @param slice_idx The index to slice upto.
         * @return A subvector which contains the values from the 
         *         0th index upto, but not including, the value
         *         at the slice index specified.
         * */
        vector<T> slice(uint64_t slice_idx);

        /** A static method which returns a zero vector of any specific size and type.
         * @param The size of the zero vector.
         * @return A zero vector of some specified size and type.
         * */
        static vector<T> zero(uint64_t size);

        /** Returns the 3-D x-axis oriented unit vector, 
         * \f$i = \begin{bmatrix} 1 & 0 & 0 \end{bmatrix} \f$. 
         * */
        static vector<T> i();

        /** Returns the 3-D y-axis oriented unit vector.
         * \f$j = \begin{bmatrix} 0 & 1 & 0 \end{bmatrix} \f$.
         * */
        static vector<T> j();

        /** Returns the 3-D z-axis oriented unit vector.
         * \f$ k = \begin{bmatrix} 0 & 0 & 1 \end{bmatrix} \f$*/
        static vector<T> k();
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const vector<T>& vec);

template <typename T>
void vector<T>::widen(uint64_t new_capacity) {
    assert(new_capacity >= this->size);

    T* copy = static_cast<T*>(malloc(new_capacity * sizeof(T)));

    if (this->size != 0) {
        for (uint64_t i = 0; i < this->size; i++) {
            copy[i] = this->data[i];
        }
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
vector<T>::vector(T* dat, uint64_t dat_size): size(dat_size), capacity(MAX(dat_size, 10)), data(static_cast<T*>(dat)) {}

template <typename T>
vector<T>::vector(const vector<T>& v2) {
    this->data = static_cast<T*>(malloc(v2.get_size() * sizeof(T)));
    this->size = v2.get_size();

    for (uint64_t i = 0; i < this->size; i++) {
        this->data[i] = v2[i];
    }
}

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
vector<T> vector<T>::operator*(const T scalar) const {
    vector<T> p(this->size);

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
void vector<T>::operator+=(const vector<T> v2) {
    assert(this->size == v2.get_size());

    for (uint64_t i = 0; i < this->size; i++) {
        this->data[i] += v2[i];
    }
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
bool vector<T>::operator==(const vector<T>& v2) const {
    if (this->size != v2.get_size()) {
        return false;
    }

    for (uint64_t i = 0; i < this->size; i++) {
        if (this->data[i] != v2[i]) {
            return false;
        }
    }

    return true;
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
vector<T> vector<T>::cross(const vector<T> v2) const {
    assert(this->size == 3 && v2.get_size() == 3);
    vector<T> i = vector<T>::i(), j = vector<T>::j(), k = vector<T>::k();

    return (i * (this->data[1]*v2[2] - this->data[2]*v2[1])) + 
           (j * (this->data[2]*v2[0] - this->data[0]*v2[2])) +
           (k * (this->data[0]*v2[1] - this->data[1]*v2[0]));
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
void vector<T>::remove(uint64_t idx) {
    assert(idx >= 0 && idx < this->size);

    T* copy = static_cast<T*>(malloc(sizeof(T) * (this->capacity)));

    for (uint64_t i = 0; i < this->size; i++) {
        if (i == idx) continue;
        copy[i - (i > idx)] = this->data[i];
    }

    this->size--;
    this->data = copy;
}

template <typename T>
void vector<T>::pop() {
    this->size = MAX(this->size-1, 0);
}

template <typename T>
void vector<T>::push_back(const T value) {
    if (this->size >= this->capacity) {
        this->widen(this->size + 2);
    }

    this->data[this->size++] = value;
}

template <typename T>
void vector<T>::swap(uint64_t i, uint64_t j) {
    assert(i >= 0 && i < this->size && j >= 0 && j < this->size);
    T tmp = this->data[i];
    this->data[i] = this->data[j];
    this->data[j] = tmp;
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
bool vector<T>::contains(T value) const {
    for (uint64_t i = 0; i < this->size; i++) {
        if (this->data[i] == value) {
            return true;
        }
    }

    return false;
}

template <typename T>
int64_t vector<T>::index(T value) const {
    for (uint64_t i = 0; i < this->size; i++) {
        if (this->data[i] == value) {
            return i;
        }
    }

    return -1;
}

// Slices upto (but not including) slice_idx
template <typename T>
vector<T> vector<T>::slice(uint64_t slice_idx) {
    assert(this->size >= slice_idx);
    vector<T> sliced(slice_idx);

    for (uint64_t i = 0; i < slice_idx; i++) {
        sliced.push_back(this->data[i]);
    }

    return sliced;
}

template <typename T>
vector<T> vector<T>::zero(uint64_t s) {
    vector<T> z(s);

    z.set_size(s);
    z.fill(0);

    return z;
}

template <typename T>
vector<T> vector<T>::i() {
    T one = static_cast<T>(1), zero = static_cast<T>(0);
    return vector<T>(new T[]{one, zero, zero}, 3);
}

template <typename T>
vector<T> vector<T>::j() {
    T one = static_cast<T>(1), zero = static_cast<T>(0);
    return vector<T>(new T[]{zero, one, zero}, 3);
}

template <typename T>
vector<T> vector<T>::k() {
    T one = static_cast<T>(1), zero = static_cast<T>(0);
    return vector<T>(new T[]{zero, zero, one}, 3);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const vector<T>& vec) {
    out << "<";
    for (uint64_t i = 0; i < vec.get_size(); i++) {
        out << vec[i] << (i < vec.get_size() - 1 ? "," : ">");
    }

    return out;
}

template <typename T>
inline vector<T> create_vec2(T x, T y) {
    vector<T> point(new T[]{x,y}, 2);
    return point;
}

template <typename T>
inline vector<T> create_vec3(T x, T y, T z) {
    vector<T> point(new T[]{x,y,z}, 3);
    return point;
}

template <typename T>
inline vector<T> create_vec3(const vector<T> v2) {
    return create_vec3(v2[0], v2[1], (v2.get_size() == 2 ? static_cast<T>(1) : v2[2]));
}

template <typename T>
inline vector<T> create_vec4(T x, T y, T z, T w) {
    vector<T> point(new T[]{x,y,z,w}, 4);
    return point;
}

template <typename T>
inline vector<T> create_vec4(T x, T y, T z) {
    return create_vec4(x, y, z, static_cast<T>(1));
}

template <typename T>
inline vector<T> create_vec4(const vector<T> v3) {
    return create_vec4(v3[0], v3[1], v3[2], (v3.get_size() == 3 ? static_cast<T>(1) : v3[3]));
}

#endif
