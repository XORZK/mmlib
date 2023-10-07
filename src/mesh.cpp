#include "mesh.hpp"
#include <regex>
#include <fstream>

void mesh::assign_faces() {
	vec4<double> *quick = static_cast<vec4<double>*>(malloc(sizeof(vec4<double>) * this->V.size()));

	linked_node<vec4<double>> *copy = this->V.front();
	linked_node<vec3<int64_t>> *map_node = this->M.front();

	for (int64_t k = 0; k < this->V.size(); k++) { quick[k] = copy->value(); copy = copy->next(); }

	for (int64_t k = 0; k < this->M.size(); k++) {
		vec3<int64_t> map = map_node->value();

		vec4<double> w1 = quick[map[0]],
					 w2 = quick[map[1]],
					 w3 = quick[map[2]];

		vec3<double> v1 = vec3(w1.x() / w1.w(), w1.y() / w1.w(), w1.z() / w1.w()),
					 v2 = vec3(w2.x() / w2.w(), w2.y() / w2.w(), w2.z() / w2.w()),
					 v3 = vec3(w3.x() / w3.w(), w3.y() / w3.w(), w3.z() / w3.w());

		triangle T = triangle(v1, v2, v3);

		this->F.push_back(T);

		map_node = map_node->next();
	}
}

mesh::mesh() {}

mesh::mesh(mesh &m) {
	this->F = list(m.faces());
	this->M = list(m.mappings());
	this->V = list(m.vertices());
}

mesh::mesh(std::string fn) {
	this->F = list<triangle>();
	this->M = list<vec3<int64_t>>();
	this->V = list<vec4<double>>();

	std::ifstream in(fn);
	std::string line, type;

	if (!in.is_open())
		return;

	while (std::getline(in, line)) {
		std::istringstream stream(line);
		stream >> type;

		if (type == "v") {
			double x, y, z, w = 1.0;
			stream >> x >> y >> z >> w;
			vec4 v4 = vec4(x,y,z,w);
			this->V.push_back(v4);
		} else if (type == "f") {
			int64_t A, B, C, N = this->V.size();
			stream >> A >> B >> C;

			if (A < 0) A = mod(A, N) + 1;
			if (B < 0) B = mod(B, N) + 1;
			if (C < 0) C = mod(C, N) + 1;

			vec3<int64_t> map = vec3<int64_t>(A-1, B-1, C-1);
			this->M.push_back(map);
		}
	}

	in.close();

	this->assign_faces();
}

mesh::~mesh() {
}

list<vec4<double>>& mesh::vertices() {
	return (this->V);
}

list<triangle>& mesh::faces() {
	return (this->F);
}

list<vec3<int64_t>>& mesh::mappings() {
	return (this->M);
}

int64_t mesh::vertex_count() const {
	return (this->V.size());
}

int64_t mesh::face_count() const {
	return (this->F.size());
}
