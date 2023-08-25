#ifndef POLYGON_HPP
#define POLYGON_HPP

#pragma once
#include "triangle.hpp"
#include "list.hpp"

class polygon {
    private:
        int64_t size;
        list<vec3<double>> vertices;
        list<triangle> triangulated;
    public:
        polygon();

        polygon(const list<vec3<double>>& vert);

        polygon(const polygon& poly);

        double area() const;

        double perimeter() const;

        vec3<double>& operator[](int64_t idx) const;

        polygon operator+(const vec3<double> v) const;

        polygon operator-(const vec3<double> v) const;

        polygon operator*(const mat3<double> M) const;

        void operator+=(const vec3<double> v);

        void operator-=(const vec3<double> v);

        void operator*=(const mat3<double> M);

        void translate(const vec3<double> v);

        void transform(const mat3<double> M);

        list<vec3<double>>& vertices() const;

        list<triangle>& triangulated() const;

        int64_t vertex_count() const;
};

#endif
