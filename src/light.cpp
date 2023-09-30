#include "light.hpp"

light::light() {
	this->position = vec3(0.0, 0.0, 1.0);
}

light::light(vec3<double> &p) {
	this->position = p;
}

light::light(light &l) {
	this->position = l.pos();
}

light::~light() {}

void light::pos(vec3<double> &p) {
	this->position = p;
}

vec3<double> light::pos() const {
	return this->position;
}

vec3<double> light::norm_pos() const {
	return this->position.normalize();
}

color light::diffuse(const vec3<double> &L,
					 const vec3<double> &N,
					 color &c) {
	double cos = L * N;
	return color(c.R() * cos, c.G() * cos, c.B() * cos, c.A());
}

color light::diffuse(const vec4<double> &L, 
					 const vec4<double> &N,
					 color &c) {
	assert(("Math Error: Division by Zero", N.w() != 0.0 && L.w() != 0.0));

	vec3 v1 = vec3(L.x() / L.w(), L.y() / L.w(), L.z() / L.w()),
		 v2 = vec3(N.x() / N.w(), N.y() / N.w(), N.z() / N.w());

	return light::diffuse(v1, v2, c);
}
