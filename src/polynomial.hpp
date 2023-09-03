#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#pragma once
#include <assert.h>
#include <iostream>
#include <math.h>
#include <stdint.h>

#include "MACROS.hpp"
#include "list.hpp"

template <size_t N = 0, typename T = double>
class polynomial {
	private:
		const int64_t deg = N;
		T* coefficients = nullptr;
	public:
		polynomial();

		polynomial(const list<T>& c);

		polynomial(const polynomial<N, T>& p);

		T& coeff(const int64_t idx) const;

		T& operator[](const int64_t idx) const;

		template <size_t M = 1>
		polynomial<MAX(N, M), T> operator+(const polynomial<M, T>& p) const;

		template <size_t M = 1>
		polynomial<MAX(N, M), T> operator-(const polynomial<M, T>& p) const;

		template <size_t M = 1>
		polynomial<N + M, T> operator*(const polynomial<M, T>& p) const;

		template <typename M>
		T operator()(M a) const;

		polynomial<N-1, T> derivative() const;

		int64_t degree() const;
};

template <size_t N, typename T>
polynomial<N, T>::polynomial() {
	this->coefficients = static_cast<T*>(malloc((deg+1) * sizeof(T)));

	for (int64_t k = 0; k <= deg; k++)
		this->coefficients[k] = static_cast<T>(0);
}

template <size_t N, typename T>
polynomial<N, T>::polynomial(const list<T>& c) {
	this->coefficients = static_cast<T*>(malloc((deg+1) * sizeof(T)));

	int64_t s = c.size();

	for (int64_t k = 0; k <= deg; k++) 
		this->coefficients[k] = (k >= s ? static_cast<T>(0) : c[k]);
}

template <size_t N, typename T>
polynomial<N, T>::polynomial(const polynomial<N, T>& p) {
	this->coefficients = static_cast<T*>(malloc((deg+1) * sizeof(T)));

	for (int64_t k = 0; k <= deg; k++)
		this->coefficients[k] = p[k];
}

template <size_t N, typename T>
T& polynomial<N, T>::coeff(int64_t idx) const {
	return this->coefficients[idx];
}

template <size_t N, typename T>
T& polynomial<N, T>::operator[](int64_t idx) const {
	return this->coefficients[idx];
}


template <size_t N, typename T> template <size_t M>
polynomial<MAX(N, M), T> polynomial<N, T>::operator+(const polynomial<M, T>& p) const {
	polynomial<MAX(N, M), T> sum;

	for (int64_t k = 0; k <= MAX(N, M); k++) {
		T c = (k > N ? static_cast<T>(0) : (*this)[k]) + 
			  (k > M ? static_cast<T>(0) : p[k]);

		sum[k] = c;
	}

	return sum;
}

template <size_t N, typename T> template <size_t M>
polynomial<MAX(N, M), T> polynomial<N, T>::operator-(const polynomial<M, T>& p) const {
	polynomial<MAX(N, M), T> diff;

	for (int64_t k = 0; k <= MAX(N, M); k++) {
		T c = (k > N ? static_cast<T>(0) : (*this)[k]) - 
			  (k > M ? static_cast<T>(0) : p[k]);

		diff[k] = c;
	}

	return diff;
}

// O(n^2) 
template <size_t N, typename T> template <size_t M>
polynomial<N + M, T> polynomial<N, T>::operator*(const polynomial<M, T>& p) const {
	polynomial<N + M, T> prod;

	for (int64_t k = 0; k <= this->degree(); k++) {
		for (int64_t i = 0; i <= p.degree(); i++) {
			prod[i + k] = ((*this)[k] * p[i]);
		}
	}

	return prod;
}

template <size_t N, typename T> template <typename M>
T polynomial<N, T>::operator()(M a) const {
	T x = static_cast<T>(a), y = static_cast<T>(0);

	for (int64_t k = 0; k <= deg; k++) {
		y += pow(x, k) * (*this)[k];
	}

	return y;	
}

template <size_t N, typename T>
polynomial<N-1, T> polynomial<N, T>::derivative() const {
	polynomial<N-1, T> d;

	for (int64_t k = 1; k <= deg; k++) {
		d[k-1] = (*this)[k] * k;
	}

	return d;
}

template <size_t N, typename T> 
int64_t polynomial<N, T>::degree() const {
	return this->deg;
}

#endif
