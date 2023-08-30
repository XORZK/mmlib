#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#pragma once
#include "list.hpp"
#include "vec.hpp"
#include "mat.hpp"

class triangle {
    private:
        // vertices
        vec3<double> *A, *B, *C;
    public:
        triangle();

        triangle(vec3<double> v1, vec3<double> v2, vec3<double> v3) ;

        triangle(const triangle& copy);

		~triangle() {
			(*A).~vec3();
			(*B).~vec3();
			(*C).~vec3();
		}

        vec3<double> v1() const;

        vec3<double> v2() const;

        vec3<double> v3() const;

        void v1(vec3<double> vertex);

        void v2(vec3<double> vertex);

        void v3(vec3<double> vertex);

        double area() const;

        double perimeter() const;

        triangle operator+(const vec3<double> v);

        triangle operator-(const vec3<double> v);

        triangle operator*(const mat3<double> M);

        void operator+=(const vec3<double> v);

        void operator-=(const vec3<double> v);

        void operator*=(mat3<double> M);

        void translate(const vec3<double> v);

        void transform(mat3<double>& M);
};

std::ostream& operator<<(std::ostream& out, const triangle& t);

list<vec3<double>> sort_vertices(const list<vec3<double>>& vertices);

vec3<double> barycentric(const vec2<double> A,
                         const vec2<double> B,
                         const vec2<double> C, 
                         const vec2<double> P);

vec3<double> barycentric(const vec3<double> A,
                         const vec3<double> B,
                         const vec3<double> C, 
                         const vec3<double> P);

vec3<double> barycentric(const triangle& T, const vec3<double> P);

bool point_in_triangle(const vec2<double> A,
                       const vec2<double> B,
                       const vec2<double> C, 
                       const vec2<double> P);

bool point_in_triangle(const vec3<double> A,
                       const vec3<double> B,
                       const vec3<double> C, 
                       const vec3<double> P);

bool point_in_triangle(const triangle& T, const vec3<double> P);


list<triangle> triangulate(list<vec3<double>>& vertices,
                           list<int64_t>& reflex,
                           list<int64_t>& ears);

list<triangle> triangulate(list<vec3<double>>& vertices);

#endif
