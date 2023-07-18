#include "triangle.h"

triangle::triangle() {
    this->reset();
}

triangle::triangle(vector<double> p1, vector<double> p2) {
    this->v1(p1);
    this->v2(p2);
    this->v3(create_vec3(0.0, 0.0, 1.0));
}

triangle::triangle(vector<double> p1, vector<double> p2, vector<double> p3) {
    this->v1(p1);
    this->v2(p2);
    this->v3(p3);
}

vector<double> triangle::v1() const {
    return this->A;
}

vector<double> triangle::v2() const {
    return this->B;
}

vector<double> triangle::v3() const {
    return this->C;
}

void triangle::v1(vector<double> p) {
    assert(p.get_size() == 3 || p.get_size() == 2);
    this->A = create_vec3(p);
}

void triangle::v2(vector<double> p) {
    assert(p.get_size() == 3 || p.get_size() == 2);
    this->B = create_vec3(p);
}

void triangle::v3(vector<double> p) {
    assert(p.get_size() == 3 || p.get_size() == 2);
    this->C = create_vec3(p);
}

void triangle::reset() {
    this->A = create_vec3(-1, 0, 1);
    this->B = create_vec3(0, 1, 1);
    this->C = create_vec3(1, 0, 1);
}

double triangle::area() const {
    return 0.5 * ((B-A).cross(C-A).magnitude());
}

double triangle::perimeter() const {
    return distance(A[0], A[1], B[0], B[1]) +
           distance(A[0], A[1], C[0], C[1]) + 
           distance(B[0], B[1], C[0], C[1]);
}

void triangle::translate(const vector<double> translation) {
    this->A += translation;
    this->B += translation;
    this->C += translation;
}

void triangle::transform(const matrix<double> mat) {
    this->A = mat * this->A;
    this->B = mat * this->B;
    this->C = mat * this->C;
}

vector<vector<double>> triangle::vertices() const {
    return create_vec3(A, B, C);
}

std::ostream& operator<<(std::ostream& out, const triangle& t) {
    out << t.vertices();

    return out;
}
