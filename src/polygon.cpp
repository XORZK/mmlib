#include "polygon.hpp"

polygon::polygon() : size(0) {
    vertices = list();
    triangulated = list();
}

polygon::polygon(const list<vec3<double>> &vert) : size(vert.size()) {
    vertices = list();

    for (int64_t k = 0; k < size; k++) 
        vertices.push_back(vec3(vert[k]));

    vertices = sort_vertices(vertices);
    triangulated = triangulate(vertices);
}

polygon::polygon(const polygon& poly) : size(poly.vertex_count()) {
    vertices = list();

    for (int64_t k = 0; k < size; k++) {
        vertices.push_back(vec3(poly[k]));
    }

    vertices = sort_vertices(vertices);
    triangulated = triangulate(vertices);
}

double polygon::area() const {
    double A = 0.0;

    for (int64_t k = 0; k < triangulated.size(); k++) {
        A += triangulated[k].area();
    }

    return A;
}

// Distances between successive vertices
double polygon::perimeter() const {
    double P = 0.0;

    for (int64_t k = 0; k < size; k++) {
        P += distance(vertices[k], vertices[mod(k+1, size)]);
    }

    return P;
}

vec3<double>& polygon::operator[](int64_t idx) const {
    assert(("Out of Bounds Error: polygon index out of range", (idx >= 0 && idx < size)));

    return vertices[idx];
}

polygon polygon::operator+(const vec3<double> v) const {
    polygon copy = polygon(*this);

    copy.translate(v);

    return copy;
}

polygon polygon::operator-(const vec3<double> v) const {
    polygon copy = polygon(*this);

    copy.translate(v * -1.0);

    return copy;
}

polygon polygon::operator*(const mat3<double> M) const {
    polygon copy = polygon(*this);

    copy.transform(M);

    return copy;
}

void polygon::translate(const vec3<double> v) {
    for (int64_t k = 0; k < size; k++) {
        vertices[k] += v;
    }

    triangulated = triangulate(vertices);
}

void polygon::transform(const mat3<double> v) {
    for (int64_t k = 0; k < size; k++) {
        vertices[k] = M * vertices[k];
    }

    vertices = sort_vertices(vertices);
    triangulated = triangulate(vertices);
}

list<vec3<double>>& polygon::vertices() const {
    return this->vertices;
}

list<triangle>& polygon::triangulated() const {
    return this->triangulated;
}

int64_t polygon::vertex_count() const {
    return this->size;
}
