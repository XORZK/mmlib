#ifndef MESH_HPP
#define MESH_HPP

#pragma once
#include <iostream>
#include <stdint.h>
#include <string>

#include "triangle.hpp"
#include "vec.hpp"
#include "vector.hpp"

// Mesh class for defining and storing meshes.
class mesh {
	private:
		int64_t fc, vc;

		vector<vec4<double>> V; // Vertices
		list<vec3<int64_t>> M; // Mappings
		list<triangle> F; // Faces
		list<vec3<double>> N; // Normals

		void compute_normals();
		void assign_faces();
	public:
		mesh();

		mesh(vector<vec4<double>> &v, list<vec3<int64_t>> &m);

		mesh(mesh &m);

		~mesh();

		vector<vec4<double>> vertices() const;

		list<vec3<int64_t>> mappings() const;

		list<triangle> faces() const;

		list<vec3<double>> normals() const;

		int64_t vertex_count() const;

		int64_t face_count() const;

		static mesh parse_obj(const std::string fn);
};

std::ostream& operator<<(std::ostream& out, const mesh &m);

#endif
