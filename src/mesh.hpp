#ifndef MESH_HPP
#define MESH_HPP

#pragma once
#include <iostream>
#include <stdint.h>
#include <string>

#include "triangle.hpp"
#include "vec.hpp"

class mesh {
	private:
		list<triangle> F;
		list<vec3<int64_t>> M;
		list<vec4<double>> V;

		void assign_faces();
	public:
		mesh();

		mesh(mesh &m);

		mesh(std::string fn);

		~mesh();

		list<vec4<double>>& vertices();

		list<triangle>& faces();

		list<vec3<int64_t>>& mappings();

		int64_t vertex_count() const;

		int64_t face_count() const;
};

#endif
