#ifndef LIGHT_HPP
#define LIGHT_HPP

#pragma once
#include <assert.h>
#include "color.hpp"
#include "vec.hpp"

class light {
	private:
		vec3<double> position;
	public:
		light();

		light(vec3<double> &p);

		light(light &l);

		~light();

		void pos(vec3<double> &p);

		vec3<double> pos() const;

		vec3<double> norm_pos() const;

		static color diffuse(const vec3<double> &L,
							 const vec3<double> &N,
							 color &c);

		static color diffuse(const vec4<double> &L,
							 const vec4<double> &N,
							 color &c);
};

#endif
