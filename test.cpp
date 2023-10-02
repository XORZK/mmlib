#include "src/window.hpp"
#include "src/compare.hpp"

int main() {
    window w(500, 500);

	color R = color::RED();
	mesh *m = parse_obj("./obj/bunny.obj");

	while (!w.has_quit()) {
		w.fill_background(color::BLACK());

		w.draw_mesh(m, R);

		w.tick();
		w.present();
	}
}
