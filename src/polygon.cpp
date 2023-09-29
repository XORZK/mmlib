#include "polygon.hpp"

polygon::polygon() : size(0) {
    verts = list<vec3<double>>();
    tri = list<triangle>();
}

polygon::polygon(const list<vec3<double>> &vert) : size(vert.size()) {
    verts = list<vec3<double>>();

    for (int64_t k = 0; k < size; k++) 
        verts.push_back(vec3(vert[k]));

    verts = sort_vertices(verts);
    tri = triangulate(verts);
}

polygon::polygon(const polygon& poly) : size(poly.vertex_count()) {
    verts = list<vec3<double>>();

    for (int64_t k = 0; k < size; k++) {
        verts.push_back(vec3(poly[k]));
    }

    verts = sort_vertices(verts);
    tri = triangulate(verts);
}

double polygon::area() const {
    double A = 0.0;

    for (int64_t k = 0; k < tri.size(); k++) {
        A += tri[k].area();
    }

    return A;
}

// Distances between successive vertices
double polygon::perimeter() const {
    double P = 0.0;

    for (int64_t k = 0; k < size; k++) {
        P += distance(verts[k], verts[mod(k+1, size)]);
    }

    return P;
}

vec3<double>& polygon::operator[](int64_t idx) const {
    assert(("Out of Bounds Error: polygon index out of range", (idx >= 0 && idx < size)));

	vec3<double> *v = new vec3<double>(verts[idx]);

    return *v;
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

polygon polygon::operator*(mat3<double>& M) const {
    polygon copy = polygon(*this);

    copy.transform(M);

    return copy;
}

void polygon::translate(const vec3<double> v) {
    for (int64_t k = 0; k < size; k++) {
        verts[k] += v;
    }

    tri = triangulate(verts);
}

void polygon::transform(mat3<double>& M) {
    for (int64_t k = 0; k < size; k++) {
		vec3<double> v = verts[k];
        verts[k] = M * v;
    }

    verts = sort_vertices(verts);
    tri = triangulate(verts);
}

list<vec3<double>>& polygon::vertices() {
    return this->verts;
}

list<triangle>& polygon::triangulated() {
    return this->tri;
}

int64_t polygon::vertex_count() const {
    return this->size;
}

list<vec3<double>> polygon::golden_icosahedron() {
	list<vec3<double>> points;

	double golden = (1.0 + sqrt(5.0))/2.0;

	points.push_back(vec3(0.0, 1.0, golden));
	points.push_back(vec3(0.0, 1.0, -golden));
	points.push_back(vec3(0.0, -1.0, golden));
	points.push_back(vec3(0.0, -1.0, -golden));
	points.push_back(vec3(1.0, golden, 0.0));
	points.push_back(vec3(1.0, -golden, 0.0));
	points.push_back(vec3(-1.0, golden, 0.0));
	points.push_back(vec3(-1.0, -golden, 0.0));
	points.push_back(vec3(golden, 0.0, 1.0));
	points.push_back(vec3(-golden, 0.0, 1.0));
	points.push_back(vec3(golden, 0.0, -1.0));
	points.push_back(vec3(-golden, 0.0, -1.0));

	return points;
}

list<vec3<double>> polygon::icosahedron(double length) {
	assert((length != 0, "Length of an icosahedron cannot be 0."));

	list<vec3<double>> golden = golden_icosahedron(),
					   points;

	linked_node<vec3<double>> *node = golden.front();

	for (int64_t k = 0; k < golden.size(); k++) {
		points.push_back(node->value() * length);
		node = node->next();
	}

	return points;
}
