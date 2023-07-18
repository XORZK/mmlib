#include <SDL2/SDL.h>
#include <functional>
#include "src/window.h"

int main() {
    window w(static_cast<std::string>("balls"), 500, 500, 15);
    w.run();

    /*
    camera *c = new camera(400, 400);
    c->compute_screen_coords(NEAR_DISTANCE, FAR_DISTANCE);

    std::cout << c->get_ndc_vector(vertex) << "\n";

    vector<double> P1 = create_vec2(0,0),
                   P2 = create_vec2(1,0),
                   P3 = create_vec2(4,2),
                   P4 = create_vec2(5,4),
                   P5 = create_vec2(4,6),
                   P6 = create_vec2(3,5),
                   P7 = create_vec2(2,8),
                   P8 = create_vec2(-1,4);

    vector<vector<double>> points = vector<vector<double>>(new vector<double>[]{P5, P4, P6, P2, P1, P3, P8, P7}, 8);

    vector<vector<vector<double>>> t = triangulate(points);

    for (uint64_t i = 0; i < t.get_size(); i++) {
        std::cout << t[i] << "\n";
    }*/
}
