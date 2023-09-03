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
#include "polynomial.hpp"

// N represents the degree of the Bezier Curve
template <int64_t N = 1>
class bezier {
	private:
		polynomial<N+1, vec2<double>> *p = nullptr;
		list<vec2<double>> *control = nullptr;	

		void compute_poly();

		void initialize_points();

		template <typename... T> 
		void initialize_points(vec2<double> p0, T... args);
	public:
		template <typename... T> 
		bezier(vec2<double> p0, T... args);

		bezier(const bezier<N>& b);

		vec2<double>& operator[](int64_t idx) const;

		vec2<double> operator()(double T) const;

		int64_t order() const;

		int64_t size() const;
};

template <int64_t N>
void bezier<N>::compute_poly() {
	this->p = new polynomial<N+1, vec2<double>>();

	linked_node<vec2<double>> *curr = control->front();

	for (int64_t k = 0; k <= N; k++) {
		vec2<double> coeff = (curr->value()) * static_cast<double>(comb(N, k));
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
void bezier<N>::initialize_points(vec2<double> p0, T... args) {
	control->push_back(p0);

	if (control->size() <= N)
		initialize_points(args...);
}

template <int64_t N> template <typename... T>
bezier<N>::bezier(vec2<double> p0, T... args) {
	control = new list<vec2<double>>();
	this->initialize_points(p0, args...);
	this->compute_poly();
}

template <int64_t N>
bezier<N>::bezier(const bezier<N>& b) {
	control = new list<vec2<double>>();

	for (int64_t k = 0; k <= N; k++) {
		control->push_back(vec2<double>(b[k]));
	}

	this->compute_poly();
}

template <int64_t N>
vec2<double>& bezier<N>::operator[](int64_t idx) const {
	assert(("Out of Bounds Errors: Bezier Curve Point index out of range.", (idx >= 0 && idx <= N)));

	vec2<double> p = (*control)[idx];

	return p;
}

template <int64_t N>
vec2<double> bezier<N>::operator()(double t) const {
	vec2<double> y = vec2(0.0, 0.0);

	for (int64_t k = 0; k <= N; k++) {
		vec2 term = (*this->p)[k] * pow(1-t, N-k) * pow(t,k);
		y += term;
	}

	return y;
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
