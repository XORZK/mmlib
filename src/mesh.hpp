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
	public:	
		list<triangle> *faces = nullptr;
		list<vec3<int64_t>> *mappings = nullptr;
		list<vec4<double>> *vertices = nullptr;

		void assign_faces();

		int64_t vertex_count() const;

		int64_t face_count() const;
};

mesh* parse_obj(std::string fn);

#endif
