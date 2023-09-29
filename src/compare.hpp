#ifndef COMPARE_HPP
#define COMPARE_HPP

#pragma once
#include "triangle.hpp"
#include "pair.hpp"
#include "vec.hpp"

namespace compare {
	inline bool polar(pair<vec2<double>, double> v1, 
			   		  pair<vec2<double>, double> v2) {
		return (v1.value() < v2.value());
	}

	inline bool polar(pair<vec3<double>, double> v1, 
					  pair<vec3<double>, double> v2) {
		return (v1.value() < v2.value());
	}

	inline bool x(vec2<double> v1, vec2<double> v2) {
		return (v1.x() == v2.x() ? v1.y() < v2.y() : v1.x() < v2.x());
	}

	inline bool x(vec3<double> v1, vec3<double> v2) {
		return (v1.x() < v2.x());
	}

	inline bool y(vec2<double> v1, vec2<double> v2) {
		return (v1.y() < v2.y());
	}

	inline bool y(vec3<double> v1, vec3<double> v2) {
		return (v1.y() < v2.y());
	}

	inline bool z(vec3<double> v1, vec3<double> v2) {
		return (v1.z() < v2.z());
	}

	inline bool tz(triangle a, triangle b) {
		return (MIN(MIN(a.v1().z(), a.v2().z()), a.v3().z())) < 
			   (MIN(MIN(b.v1().z(), b.v2().z()), b.v3().z()));
	}

	inline bool clockwise(vec3<double> a,
				   	 	  vec3<double> b,
				   	 	  vec3<double> c) {
		if (a.x() >= c.x() && b.x() < c.x())
			return true;

		if (a.x() < c.x() && b.x() >= c.x()) 
			return false;

		if (a.x() == c.x() && b.x() == c.x())
			return ((a.y() >= c.y() || b.y() >= c.y()) ? 
					(a.y() > b.y()) : (b.y() > a.y()));

		// Determinant of a 2x2 matrix filled with vectors a, b.
		// Were calculating det([{a.x, b.x}, {a.y, b.y}])
		double d = det<double>(a - c, b - c);    

		// If determinant is negative, that means the 2-D plane is inverted.
		// That means that the vector a is to the left of b.
		return ((d < 0) ? true : (d > 0) ? false : distance(a, c) > distance(b, c));
	}

	// Assumes c = (0, 0, 0)
	inline bool clockwise(vec3<double> a,
				   		  vec3<double> b) {
		vec3<double> c = vec3(0.0, 0.0, 0.0);

		return compare::clockwise(a, b, c);
	}

	inline bool counter_clockwise(vec3<double> a,
						   		  vec3<double> b,
						   		  vec3<double> c) {
		return !compare::clockwise(a, b, c);
	}

	inline bool counter_clockwise(vec3<double> a,
						   		  vec3<double> b) {
		return !compare::clockwise(a, b);
	}

	inline bool clockwise(vec2<double> a,
						  vec2<double> b,
						  vec2<double> c) {
		double det = cross(a, b, c);

		return (det < 0 ? true : det > 0 ? false : distance(a, c) > distance(b, c));
	}

	inline bool clockwise(vec2<double> a,
						  vec2<double> b) {
		vec2<double> c = vec2(0.0, 0.0);

		return compare::clockwise(a, b, c);
	}

	inline bool counter_clockwise(vec2<double> a,
								  vec2<double> b,
								  vec2<double> c) {
		return !compare::clockwise(a, b, c);
	}

	inline bool counter_clockwise(vec2<double> a,
								  vec2<double> b) {
		return !compare::clockwise(a, b);
	}
};

#endif
