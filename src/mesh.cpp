#include "mesh.hpp"
#include <regex>
#include <fstream>
#include <sstream>

void mesh::assign_faces() {
	vec4<double> *a = static_cast<vec4<double>*>(malloc(sizeof(vec4<double>) * vertices->size()));

	linked_node<vec4<double>> *vert_node = vertices->front();
	for (int64_t k = 0; k < vertices->size(); k++) {
		a[k] = vert_node->value();
		vert_node = vert_node->next();
	}

	linked_node<vec3<int64_t>> *map_node = mappings->front();
	for (int64_t k = 0; k < mappings->size(); k++) {
		vec3<int64_t> map = map_node->value();

		vec4<double> w1 = a[map[0]],
					 w2 = a[map[1]],
					 w3 = a[map[2]];

		vec3<double> v1 = vec3(w1.x()/w1.w(), w1.y()/w1.w(), w1.z()/w1.w()),
				     v2 = vec3(w2.x()/w2.w(), w2.y()/w2.w(), w2.z()/w2.w()),
				     v3 = vec3(w3.x()/w3.w(), w3.y()/w3.w(), w3.z()/w3.w());

		triangle T = triangle(v1, v2, v3);

		this->faces->push_back(T);

		map_node = map_node->next();
	}
}

int64_t mesh::vertex_count() const {
	return this->vertices->size();
}

int64_t mesh::face_count() const {
	return this->faces->size();
}

mesh* parse_obj(std::string fn) {
	mesh *m = new mesh();

	m->faces = new list<triangle>();
	m->vertices = new list<vec4<double>>();
	m->mappings = new list<vec3<int64_t>>();

	std::ifstream obj_file(fn);
	std::string line, type;

	while (std::getline(obj_file, line)) {
		std::istringstream stream(line);
		stream >> type;

		if (type == "v") {
			double x, y, z, w = 1.0;
			stream >> x >> y >> z >> w;

			vec4 v4 = vec4(x,y,z,w);

			m->vertices->push_back(v4);
		} else if (type == "f") {
			int64_t A, B, C;
			stream >> A >> B >> C;

			vec3<int64_t> map = vec3<int64_t>(A-1, B-1, C-1);
			m->mappings->push_back(map);
		}
	}

	obj_file.close();

	m->assign_faces();

	return m;
}
