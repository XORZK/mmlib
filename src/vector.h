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

template <typename T>
class vector {
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

        vector(const vector<T>& v2);

        void fill(T value);

        T& operator[](uint64_t idx) const;

        T operator*(const vector<T> v2) const;

        vector<T> operator*(const T scalar) const;

        vector<double> operator/(const double scalar) const;

        template <typename U>
        void operator/=(const U scalar);

        vector<T> operator+(const vector<T> v2) const;

        void operator+=(const vector<T> v2);

        vector<T> operator+(const T value) const;

        vector<T> operator-(const vector<T> v2) const;

        vector<T> operator-(const T value) const;

        bool operator==(const vector<T>& v2) const;

        template <typename U>
        operator vector<U>() const;

        vector<T> cross(const vector<T> v2) const;

        double magnitude() const;

        vector<double> normalize() const;

        void remove(uint64_t idx);

        void pop();

        void push_back(const T value); 

        void swap(uint64_t i, uint64_t j);

        double cos(vector<T> v2) const;

        uint64_t get_size() const;

        uint64_t get_capacity() const;

        void set_size(uint64_t new_size);

        void set_column(bool c);

        bool is_column() const;

        bool contains(T value) const;

        int64_t index(T value) const;

        vector<T> slice(uint64_t slice_idx);

        static vector<T> zero(uint64_t size);

        static vector<T> i();

        static vector<T> j();

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
    return create_vec3(v2[0], v2[1], static_cast<T>(1));
}

template <typename T>
inline vector<T> create_vec4(T x, T y, T z) {
    vector<T> point(new T[]{x,y,z,static_cast<T>(1)}, 4);
    return point;
}

template <typename T>
inline vector<T> create_vec4(T x, T y, T z, T w) {
    vector<T> point(new T[]{x,y,z,w}, 4);
    return point;
}

template <typename T>
inline vector<T> create_vec4(const vector<T> v3) {
    vector<T> point(new T[]{v3[0], v3[1], v3[2], (v3.get_size() == 3 ? static_cast<T>(1) : v3[4])}, 4);
    return point;
}

// Find Angle of each Vertex from the Estimated Center of Points
// Sorts Points Counter Clockwise
inline vector<vector<double>> sort_vertices(const vector<vector<double>>& vertices) {
    vector<vector<double>> sorted(vertices.get_size());
    vector<double> C = (vertices[0].get_size() == 3 ? create_vec3(0,0,1) : create_vec2(0, 0));

    for (uint64_t i = 0; i < vertices.get_size(); i++) {
        C[0] += vertices[i][0];
        C[1] += vertices[i][1];
    }

    C /= vertices.get_size();

    for (uint64_t i = 0; i < vertices.get_size(); i++) {
        vector<double> n = (vertices[i] - C);
        double dist = distance(n[0], n[1], 0.0, 0.0);

        double angle = atan2(n[1], n[0]) * 180/M_PI;
        angle += (360 * (angle < 0));
        sorted.push_back(create_vec3(angle, dist, static_cast<double>(i)));
    }

    // Sort the vector
    // Insertion Sort? --> Merge Sort (TODO)
    for (uint64_t i = 1; i < sorted.get_size(); i++) {
        uint64_t k = i;
        while (k > 0 && ((sorted[k][0] < sorted[k-1][0]) || (sorted[k][0] == sorted[k-1][0] && sorted[k][1] < sorted[k-1][1]))) {
            sorted.swap(k, k-1);
            k--;
        }
    }

    for (uint64_t i = 0; i < sorted.get_size(); i++) {
        sorted[i] = vertices[sorted[i][2]];
    }

    return sorted;
}

// Pre-condition: Assume vertices are in 2D space.
inline vector<double> centroid(vector<vector<double>> vertices) {
    vertices = sort_vertices(vertices);
    int64_t N = vertices.get_size();
    // A is the polygon's signed area -> A = (1/2) sum_{i=0}{n-1} [(x_i * y_(i+1)) - (x_(i+1) * y_i)]
    double A = 0.0, Cx = 0.0, Cy = 0.0;
    for (uint64_t i = 0; i < N; i++) {
        A += (vertices[i][0] * vertices[(i+1)%N][1]) - (vertices[(i+1)%N][0] * vertices[i][1]);
    }

    A /= 2.0;

    assert(A != 0);

    for (uint64_t i = 0; i < N; i++) {
        double T = (vertices[i][0] * vertices[(i+1)%N][1] - vertices[(i+1)%N][0] * vertices[i][1]);

        Cx += (vertices[i][0] + vertices[(i+1)%N][0]) * T; 
        Cy += (vertices[i][1] + vertices[(i+1)%N][1]) * T; 
    }

    Cx /= (6*A);
    Cy /= (6*A);

    return create_vec2(Cx, Cy);
}

// Computes the Barycentric Coordinates of a Point (possibly) within a Triangle ABC
// alpha = A(PCB)/A
// beta = A(PCA)/A
// gamma = 1 - (alpha + beta)
// We use the cross product to determine the area of triangles ABC, APC, APB, BPC
// which are then used to determine the weights (alpha, beta, gamma).
inline vector<double> barycentric(vector<double> p,
                                  vector<double> a,
                                  vector<double> b,
                                  vector<double> c) {

    if (p.get_size() == 2) { p = create_vec3(p); }
    if (a.get_size() == 2) { a = create_vec3(a); }
    if (b.get_size() == 2) { b = create_vec3(b); }
    if (c.get_size() == 2) { c = create_vec3(c); }

    double A = ((b-a).cross(c-a).magnitude())/2.0,
           ACB = ((b-p).cross(c-p).magnitude())/2.0,
           ACA = ((a-p).cross(c-p).magnitude())/2.0;
    double alpha = ACB/A, beta = ACA/A, gamma = 1 - (alpha + beta);

    return create_vec3(alpha, beta, gamma);
}

inline bool is_convex_vertex(vector<double> vA, vector<double> vB, vector<double> vC) {
    return (((vB[0] - vA[0]) * (vC[1] - vB[1])) - ((vC[0] - vB[0]) * (vB[1] - vA[1]))) > 0;
}

inline bool is_convex_vertex(vector<vector<double>>& vertices, int64_t idx) {
    assert(idx >= 0 && idx < vertices.get_size());
    int64_t N = vertices.get_size();
    vector<double> vA = vertices[mod(idx-1, N)], vB = vertices[mod(idx, N)], vC = vertices[mod(idx+1, N)];
    return is_convex_vertex(vA, vB, vC);
}

inline bool is_ear_vertex(vector<vector<double>>& vertices, int64_t idx) {
    assert(idx >= 0 && idx < vertices.get_size());
    int64_t N = vertices.get_size();
    vector<double> vA = vertices[mod(idx-1, N)], vB = vertices[mod(idx, N)], vC = vertices[mod(idx+1, N)];

    for (int64_t k = 0; k < N; k++) {
        if (k == mod(idx-1, N) || k == mod(idx, N) || k == mod(idx+1, N)) {
            continue;
        }

        vector<double> bc = barycentric(vertices[k], vA, vB, vC);
        if (!(bc[0] && bc[1] && bc[2])) {
            return false;
        }
    }

    return true;
}

inline void update_polygon_vertex(vector<vector<double>>& vertices, vector<int64_t>& reflex, vector<int64_t>& ears, int64_t idx) {
    if (is_ear_vertex(vertices, idx)) {
        if (reflex.contains(idx)) {
            reflex.remove(reflex.index(idx));
        }
        ears.push_back(idx);
    } else {
        if (ears.contains(idx)) {
            ears.remove(ears.index(idx));
        }
    }
}

inline vector<vector<vector<double>>> triangulate(vector<vector<double>>& vertices, vector<int64_t>& reflex, vector<int64_t>& ears) {
    vector<vector<vector<double>>> t;
    // If the set of vertices does not form a triangle, remove an ear vertex
    // and update the status of it's adjacent vertices
    // { P1, P2, P3, P4, P5 } -> { P1, P2, P4, P5 }
    // We want to update using Angles P1-P2-P4 and P2-P4-P5
    // If we remove index N, we update vertexes (N-1, N, N+1) and (N, N+1, N+2)
    while (vertices.get_size() > 3) {
        int64_t N = vertices.get_size();
        int64_t removal = ears[0];

        vector<vector<double>> triangle = create_vec3(vertices[mod(removal-1, N)],
                                                      vertices[mod(removal, N)],
                                                      vertices[mod(removal+1, N)]);
        ears.remove(0);
        vertices.remove(removal);

        for (int64_t i = 0; i < ears.get_size(); i++) {
            if (ears[i] > removal) {
                ears[i]--;
            }
        }

        for (int64_t i = 0; i < reflex.get_size(); i++) {
            if (reflex[i] > removal) {
                reflex[i]--;
            }
        }

        update_polygon_vertex(vertices, reflex, ears, removal);

        t.push_back(triangle);
    }

    t.push_back(create_vec3(vertices[0], vertices[1], vertices[2]));

    return t;
}


inline vector<vector<vector<double>>> triangulate(vector<vector<double>> vertices) {
    vertices = sort_vertices(vertices);
    vector<int64_t> reflex, ears;
    int64_t N = vertices.get_size();

    // By cosine law: c^2 = a^2 + b^2 - 2*a*b*cos(C)
    // cos(C) = (a^2 + b^2 - c^2)/2*a*b
    for (int64_t i = 0; i < N; i++) {
        vector<double> vA = vertices[mod(i-1, N)], vB = vertices[mod(i, N)], vC = vertices[mod(i+1, N)];

        // If det(b-a, c-b) > 0 --> vertices form a left turn -> implies convex
        // (b.x - a.x) * (c.y - b.y) - (c.x - b.x) * (b.y - a.y)
        if (!is_convex_vertex(vertices, i)) {
            reflex.push_back(i);
        } else {
            if (is_ear_vertex(vertices, i)) {
                ears.push_back(i);
            }
        }
    }

    return triangulate(vertices, reflex, ears);
}

#endif
