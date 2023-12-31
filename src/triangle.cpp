#include "triangle.hpp"
#include "compare.hpp"

triangle::triangle() {
    A = new vec3<double>(-1.0, 0.0, 1.0);
    B = new vec3<double>(0.0, 1.0, 1.0);
    C = new vec3<double>(1.0, 0.0, 1.0);
	this->sort_ccw();
}

triangle::triangle(vec3<double> v1, 
                   vec3<double> v2, 
                   vec3<double> v3) {
	A = new vec3<double>(v1);
	B = new vec3<double>(v2);
	C = new vec3<double>(v3);
	this->sort_ccw();
}

triangle::triangle(const triangle& copy) {
    A = new vec3(copy.v1());
    B = new vec3(copy.v2());
    C = new vec3(copy.v3());
	this->sort_ccw();
}

vec3<double> triangle::v1() const {
    return *A;
}

vec3<double> triangle::v2() const {
    return *B;
}

vec3<double> triangle::v3() const {
    return *C;
}

void triangle::v1(vec3<double> vertex) {
    A = new vec3<double>(vertex);
	this->sort_ccw();
}

void triangle::v2(vec3<double> vertex) {
    B = new vec3<double>(vertex);
	this->sort_ccw();
}

void triangle::v3(vec3<double> vertex) {
    C = new vec3<double>(vertex);
	this->sort_ccw();
}

vec3<double> triangle::mid() const {
	return (*A + *B + *C) / 3.0;
}

vec3<double> triangle::normal() const {
	return (*A - *B).cross(*C - *B);
}

double triangle::area() const {
    return 0.5 * (((*B)-(*A)).cross((*C)-(*A)).magnitude());
}

double triangle::perimeter() const {
    return distance((*A), (*B)) + 
		   distance((*A), (*C)) + 
		   distance((*B), (*C));
}

triangle triangle::operator+(const vec3<double> v) {
    triangle shifted((*A) + v, (*B) + v, (*C) + v);
    
    return shifted;
}

triangle triangle::operator-(const vec3<double> v) {
    triangle shifted((*A) - v, (*B) - v, (*C) - v);
    return shifted;
}

triangle triangle::operator*(const mat3<double> M) {
    triangle transformed(M * (*A), M * (*B), M * (*C));

    return transformed;
}

void triangle::operator+=(const vec3<double> v) {
    this->translate(v);
}

void triangle::operator-=(const vec3<double> v) {
    this->translate(v * -1.0);
}

void triangle::operator*=(mat3<double> M) {
    this->transform(M);
}

void triangle::translate(const vec3<double> v) {
    *A += v;
    *B += v;
    *C += v;
	this->sort_ccw();
}

void triangle::transform(mat3<double>& M) {
    *A = M * (*A);
    *B = M * (*B);
    *C = M * (*C);
	this->sort_ccw();
}

void triangle::sort_ccw() {
	if (compare::counter_clockwise((*A), (*B))) {
		std::swap(A, B);
	} 

	if (compare::counter_clockwise((*A), (*C))) {
		std::swap(A, C);
	}

	if (compare::counter_clockwise((*B), (*C))) {
		std::swap(B, C);
	}
}

std::ostream& operator<<(std::ostream& out, const triangle& t) {
    out << "triangle: (" << t.v1() << ", " << t.v2() << ", " << t.v3() << ")";
    return out;
}

// Assumes that for each vec3, z value is 1. 
// Sorts vertices in clockwise order, starting from the top.
list<vec3<double>> sort_vertices(const list<vec3<double>>& vertices) {
    list<vec3<double>> sorted_list;

    int64_t size = vertices.size();

    vec3<double> C = vec3<double>(0.0, 0.0, 0.0);

    for (int64_t k = 0; k < size; k++) {
        C += vertices[k];
        sorted_list.push_back(vec3(vertices[k]));
    }

    C /= size;

    // Insertion sort
    for (int64_t k = 1; k < size; k++) {
        int64_t swap = k;

        while (swap > 0 && compare::clockwise(sorted_list[swap], sorted_list[swap-1], C)) {
            sorted_list.swap(swap, swap-1);
            --swap;
        }
    }

    return sorted_list;
}

vec3<double> barycentric(const vec2<double> A,
                         const vec2<double> B,
                         const vec2<double> C,
                         const vec2<double> P) {
    return barycentric(vec3(A, 1.0), vec3(B, 1.0), vec3(C, 1.0), vec3(P, 1.0));
}

vec3<double> barycentric(const vec3<double> A,
                         const vec3<double> B,
                         const vec3<double> C,
                         const vec3<double> P) {
    vec3<double> AB = B - A,
                 AC = C - A,
                 AP = P - A;

    vec3<double> nAC = vec3(A.y() - C.y(), C.x() - A.x(), 1.0),
                 nAB = vec3(A.y() - B.y(), B.x() - A.x(), 1.0);

    double beta = (AP * nAC)/(AB * nAC),
           gamma = (AP * nAB)/(AC * nAB),
           alpha = 1.0 - beta - gamma;

    return vec3(alpha, beta, gamma);
}

vec3<double> barycentric(const triangle& T, const vec3<double> P) {
    return barycentric(T.v1(), T.v2(), T.v3(), P);
}

bool point_in_triangle(const vec2<double> A,
                       const vec2<double> B,
                       const vec2<double> C, 
                       const vec2<double> P) {
    return point_in_triangle(vec3(A, 1.0), vec3(B, 1.0), vec3(C, 1.0), vec3(P, 1.0));
}


bool point_in_triangle(const vec3<double> A,
                       const vec3<double> B,
                       const vec3<double> C, 
                       const vec3<double> P) {
    vec3<double> bary = barycentric(A, B, C, P);

    return (0 <= bary[0] && 0 <= bary[1] && 0 <= bary[2] && 
            1 >= bary[0] && 1 >= bary[1] && 1 >= bary[2]);
}

bool point_in_triangle(const triangle& T, const vec3<double> P) {
    return point_in_triangle(T.v1(), T.v2(), T.v3(), P);
}

bool is_ear_vertex(const triangle& T, 
                   const list<vec3<double>>& vertices, 
                   const list<int64_t>& reflex) {

    for (int64_t k = 0; k < reflex.size(); k++) {
        vec3<double> P = vertices[reflex[k]];

        if (P == T.v1() || P == T.v2() || P == T.v3())
            continue;

        if (point_in_triangle(T, P)) {
            return false;
        }
    }

    return true;
}

int64_t find_right_vertex(list<vec3<double>>& vertices, 
                          list<int64_t>& removed,
                          int64_t vert_index) {
    int64_t size = vertices.size();
    int64_t right_index = mod(vert_index+1, size);

    while (removed.index(right_index) != -1) {
        right_index = mod(right_index+1, size);
    }

    return right_index;
}

int64_t find_left_vertex(list<vec3<double>>& vertices,
                         list<int64_t>& removed,
                         int64_t vert_index) {
    int64_t size = vertices.size();
    int64_t left_index = mod(vert_index-1, size);

    while (removed.index(left_index) != -1) {
        left_index = mod(left_index-1, size);
    }

    return left_index;
}

list<triangle> triangulate(list<vec3<double>>& vertices) {
    list<vec3<double>> sorted = sort_vertices(vertices);

    list<triangle> triangles;
    list<int64_t> convex, reflex, ears, removed;
    int64_t size = vertices.size();

    for (int64_t k = 0; k < size; k++) {
        vec3<double> A = sorted[mod(k+1,size)], 
                     B = sorted[mod(k, size)], 
                     C = sorted[mod(k-1, size)]; 

        double d = det(B - A, C - B);

        if (d < 0) {
            reflex.push_back(k);
        } else {
            convex.push_back(k);
        }
    }

    for (int64_t i = 0; i < convex.size(); i++) {
        int64_t vert_index = convex[i];
        vec3<double> A = sorted[mod(vert_index+1, size)], 
                     B = sorted[mod(vert_index, size)], 
                     C = sorted[mod(vert_index-1, size)]; 
    
        bool in_triangle = true;

        if (is_ear_vertex(triangle(A, B, C), sorted, reflex)) {
            ears.push_back(vert_index);
        }
    }

    while (triangles.size() <= size - 2) {
        int64_t vert_index = ears[0], 
                left_index = find_left_vertex(sorted, removed, vert_index),
                right_index = find_right_vertex(sorted, removed, vert_index);

        vec3<double> A = sorted[right_index],
                     B = sorted[vert_index], 
                     C = sorted[left_index];

        ears.remove(0);
        convex.remove(convex.index(vert_index));

        if (removed.index(vert_index) != -1)
            continue;

        removed.push_back(vert_index);

        triangles.push_back(triangle(A, B, C));

        vec3<double> R = sorted[find_right_vertex(sorted, removed, right_index)],
                     L = sorted[find_left_vertex(sorted, removed, left_index)];

        // Check left neighbour
        if (det(C-A, L-C) > 0) {
            reflex.remove(reflex.index(left_index));

            if (convex.index(left_index) == -1) 
                convex.push_back(left_index);

            if (is_ear_vertex(triangle(L, C, A), sorted, reflex)) {
                ears.push_back(left_index);
            }
        }

        // Checking right neighbour
        if (det(A-R, C-A) > 0) {
            reflex.remove(reflex.index(right_index));

            if (convex.index(right_index) == -1)
                convex.push_back(right_index);

            if (is_ear_vertex(triangle(R, C, A), sorted, reflex)) {
                ears.push_back(right_index);
            }
        }
    }

    return triangles;
}
