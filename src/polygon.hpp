#ifndef POLYGON_HPP
#define POLYGON_HPP

#pragma once
#include "triangle.hpp"
#include "list.hpp"

class polygon {
    private:
        int64_t size;
        list<vec3<double>> verts;
        list<triangle> tri;
    public:
        polygon();

        polygon(const list<vec3<double>>& vert);

        polygon(const polygon& poly);

        double area() const;

        double perimeter() const;

        vec3<double>& operator[](int64_t idx) const;

        polygon operator+(const vec3<double> v) const;

        polygon operator-(const vec3<double> v) const;

        polygon operator*(mat3<double>& M) const;

        void operator+=(const vec3<double> v);

        void operator-=(const vec3<double> v);

        void operator*=(mat3<double>& M);

        void translate(const vec3<double> v);

        void transform(mat3<double>& M);

        list<vec3<double>>& vertices();

        list<triangle>& triangulated();

        int64_t vertex_count() const;
};

#endif
