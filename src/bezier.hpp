#ifndef BEZIER_HPP
#define BEZIER_HPP

#pragma once
#include <assert.h>
#include <cstdarg>
#include <iostream>
#include <functional>

#include "MACROS.hpp"
#include "list.hpp"
#include "vec.hpp"
#include "mat.hpp"
#include "polynomial.hpp"

// N represents the degree of the Bezier Curve (N+1 control points)
template <int64_t N = 1>
class bezier {
	private:
		polynomial<N+1, vec3<double>> *p = nullptr;
		list<vec3<double>> *control = nullptr;	

		void compute_poly();

		template <typename... T> 
		void initialize_points(vec3<double> p0, T... args);

		void initialize_points();
	public:
		template <typename... T> 
		bezier(vec3<double> p0, T... args);

		bezier(const bezier<N>& b);

		vec3<double>& operator[](int64_t idx) const;

		vec3<double> operator()(double T) const;

		bezier<N> operator+(vec3<double> tv) const;

		bezier<N> operator*(mat3<double> mat) const;

		void operator+=(vec3<double> tv);

		void operator*=(mat3<double> mat);

		void transform(mat3<double> mat);

		void translate(vec3<double> tv);

		int64_t order() const;

		int64_t size() const;
};

template <int64_t N>
void bezier<N>::compute_poly() {
	this->p = new polynomial<N+1, vec3<double>>();

	std::cout << *control << "\n";

	linked_node<vec3<double>> *curr = control->front();

	for (int64_t k = 0; k <= N; k++) {
		vec3<double> coeff = (curr->value()) * static_cast<double>(comb(N, k));
		(*this->p)[k] = coeff;
		curr = curr->next();
	}
}

template <int64_t N>
void bezier<N>::initialize_points() {
	if (control->size() != N+1) {
		throw std::runtime_error("Bezier Curve cannot be formed: Amount of control points is insufficient");
	}
}

template <int64_t N> template <typename... T>
void bezier<N>::initialize_points(vec3<double> p0, T... args) {
	control->push_back(p0);

	if (control->size() <= N)
		initialize_points(args...);
}

template <int64_t N> template <typename... T>
bezier<N>::bezier(vec3<double> p0, T... args) {
	control = new list<vec3<double>>();
	this->initialize_points(p0, args...);
	this->compute_poly();
}

template <int64_t N>
bezier<N>::bezier(const bezier<N>& b) {
	control = new list<vec3<double>>();

	for (int64_t k = 0; k <= N; k++) {
		control->push_back(vec3<double>(b[k]));
	}

	this->compute_poly();
}

template <int64_t N>
vec3<double>& bezier<N>::operator[](int64_t idx) const {
	assert(("Out of Bounds Errors: Bezier Curve Point index out of range.", (idx >= 0 && idx <= N)));

	vec3<double> p = (*control)[idx];

	return p;
}

template <int64_t N>
vec3<double> bezier<N>::operator()(double t) const {
	vec3<double> y = vec3(0.0, 0.0, 0.0);

	for (int64_t k = 0; k <= N; k++) {
		vec3 term = (*this->p)[k] * pow(1-t, N-k) * pow(t,k);
		y += term;
	}

	return y;
}

template <int64_t N>
bezier<N> bezier<N>::operator+(vec3<double> tv) const {
	bezier<N> copy = *this;
	copy.translate(tv);

	return copy;
}

template <int64_t N>
bezier<N> bezier<N>::operator*(mat3<double> mat) const {
	bezier<N> copy = *this;
	copy.transform(mat);

	return copy;
}

template <int64_t N>
void bezier<N>::operator+=(vec3<double> tv) {
	this->transform(tv);
}

template <int64_t N>
void bezier<N>::operator*=(mat3<double> mat) {
	this->transform(mat);
}

template <int64_t N>
void bezier<N>::transform(mat3<double> mat) {
	linked_node<vec3<double>>* curr = control->front();

	for (int64_t k = 0; k <= N; k++) {
		vec3<double> value = (*control)[k];

		curr->value(mat*value);
		curr = curr->next();
	}

	this->compute_poly();
}

template <int64_t N>
void bezier<N>::translate(vec3<double> tv) {
	linked_node<vec3<double>>* curr = control->front();

	for (int64_t k = 0; k <= N; k++) {
		vec3<double> value = (*control)[k];

		curr->value(value + tv);
		curr = curr->next();
	}

	this->compute_poly();
}

template <int64_t N>
int64_t bezier<N>::order() const {
	return N;
}

template <int64_t N>
int64_t bezier<N>::size() const {
	return N+1;
}

#endif
