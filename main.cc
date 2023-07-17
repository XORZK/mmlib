#include <SDL2/SDL.h>
#include <functional>
#include "src/camera.h"
#include "src/matrix.h"
#include "src/window.h"

int main() {
    /*
    window w(static_cast<std::string>("balls"), 500, 500, 15);
    w.run();

    camera *c = new camera(400, 400);
    c->compute_screen_coords(NEAR_DISTANCE, FAR_DISTANCE);

    std::cout << c->get_ndc_vector(vertex) << "\n";*/
    vector<double> v(new double[]{1,2,3}, 3);

    std::cout << v << "\n";

    v.remove(1);

    std::cout << v << "\n";
}
