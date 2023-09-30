#include "mesh.hpp"
#include <regex>
#include <fstream>

void mesh::assign_faces() {
	linked_node<vec3<int64_t>> *current = this->M.front();

	for (int64_t k = 0; k < this->fc && current != nullptr; k++) {
		vec3<int64_t> indices = current->value();

		int64_t a = indices[0], b = indices[1], c = indices[2];

		vec4<double> w1 = V[a],
					 w2 = V[b],
					 w3 = V[c];

		vec3<double> v1 = vec3(w1.x()/w1.w(), w1.y()/w1.w(), w1.z()/w1.w()),
					 v2 = vec3(w2.x()/w2.w(), w2.y()/w2.w(), w2.z()/w2.w()), 
					 v3 = vec3(w3.x()/w3.w(), w3.y()/w3.w(), w3.z()/w3.w());

		triangle T = triangle(v1, v2, v3);

		this->F.push_back(T); 

		current = current->next();
	}
}

void mesh::compute_normals() {
	linked_node<triangle> *current = this->F.front();

	for (int64_t k = 0; k < this->F.size(); k++) {
		this->N.push_back(current->value().normal());
		current = current->next();
	}
}

mesh::mesh() : fc(0), vc(0) {}

mesh::mesh(vector<vec4<double>> &v, list<vec3<int64_t>> &m) {
	this->vc = v.size();
	this->fc = m.size();

	this->V = v;
	this->M = m;

	this->assign_faces();
	this->compute_normals();
}

mesh::mesh(mesh &m) : fc(m.face_count()), vc(m.vertex_count()) {
	this->V = m.vertices();
	this->F = m.faces();
}

mesh::~mesh() {}

vector<vec4<double>> mesh::vertices() const {
	return this->V;
}

list<vec3<int64_t>> mesh::mappings() const {
	return this->M;
}

list<triangle> mesh::faces() const {
	return this->F;
}

list<vec3<double>> mesh::normals() const {
	return this->N;
}

int64_t mesh::vertex_count() const {
	return this->vc;
}

int64_t mesh::face_count() const {
	return this->fc;
}

mesh mesh::parse_obj(const std::string fn) {
	vector<vec4<double>> V; 
	list<vec3<int64_t>> M;

	std::ifstream obj_file(fn);

	std::string line, type;

	while (std::getline(obj_file, line)) {
		std::istringstream stream(line);
		stream >> type;

		if (type == "v") {
			double x, y, z, w = 1.0;
			stream >> x >> y >> z >> w;

			vec4 v4 = vec4(x,y,z,w);

			V.push_back(v4);
		} else if (type == "f") {
			// Indexing starts from 1.
			int64_t A, B, C, N = V.size();

			stream >> A >> B >> C;
			
			if (A < 0) A = mod(A, N) + 1;
			if (B < 0) B = mod(B, N) + 1;
			if (C < 0) C = mod(C, N) + 1;

			vec3<int64_t> index = vec3(A-1, B-1, C-1);

			M.push_back(index);
		}
	}

	obj_file.close();

	mesh m = mesh(V, M);

	return m;
}
