#include "src/window.hpp"
#include "src/compare.hpp"

void test(mesh &m) {
	std::cout << m.faces() << "\n";
}

int main() {
	window w(500, 500);

	mesh i = mesh("./obj/isohedron.obj"), b = mesh("./obj/bunny.obj");

	color R = color::RED();

	while (!w.has_quit()) {
		w.fill_background(color::BLACK());

		w.tick();

		w.draw_mesh(i, R);

		w.present();
	}
}
