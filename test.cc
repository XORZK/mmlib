#include "rewrite/list.hpp"
#include "rewrite/vector.hpp"
#include "rewrite/vec.hpp"
#include "rewrite/matrix.hpp"
#include "rewrite/mat.hpp"
#include "rewrite/color.hpp"
#include "rewrite/triangle.hpp"
#include "rewrite/camera.hpp"
#include "rewrite/window.hpp"

int main() {
    /*
    vec2<int> x(0,1);

    x[0] = 3;

    x *= 2;

    std::cout << x << "\n";

    mat4<int> M, N;

    for (int64_t k = 0; k < 16; k++) {
        M[k/4][k%4] = rand() % 10;
    }

    N = M.transpose();

    std::cout << M << "\n";
    std::cout << det(M) << "\n";

    matrix<int> cM = static_cast<matrix<int>>(M);

    std::cout << invert(cM) << "\n";
    std::cout << invert(M) << "\n";

    std::cout << create_3d_rotation_matrix(0,0,0,1) << "\n";

    color c(0,0,0);

    std::cout << c << "\n";

    triangle t;

    std::cout << t << "\n";

    list<vec3<double>> vertices;


    vertices.push_back(vec3<double>(173,29,1)); 
    vertices.push_back(vec3<double>(181,88,1)); 
    vertices.push_back(vec3<double>(246,133,1)); 
    vertices.push_back(vec3<double>(210,148,1)); 
    vertices.push_back(vec3<double>(240,147,1)); 
    vertices.push_back(vec3<double>(241,173,1)); 
    vertices.push_back(vec3<double>(209,192,1)); 
    vertices.push_back(vec3<double>(221,196,1)); 
    vertices.push_back(vec3<double>(205,220,1)); 
    vertices.push_back(vec3<double>(154,170,1));  
    vertices.push_back(vec3<double>(93,222,1)); 
    vertices.push_back(vec3<double>(63,223,1)); 
    vertices.push_back(vec3<double>(72,155,1)); 
    vertices.push_back(vec3<double>(109,104,1));
    vertices.push_back(vec3<double>(68,129,1)); 
    vertices.push_back(vec3<double>(59,73,1)); 
    vertices.push_back(vec3<double>(27,64,1)); 
    vertices.push_back(vec3<double>(38,39,1)); 
    vertices.push_back(vec3<double>(57,49,1)); 
    vertices.push_back(vec3<double>(118,25,1)); 
    vertices.push_back(vec3<double>(142,78,1)); 
    vertices.push_back(vec3<double>(151,50,1)); 
    vertices.push_back(vec3<double>(151,21,1));

    std::cout << sort_vertices(vertices) << "\n"; 

    triangle t;

    std::cout << t << "\n";

    std::cout << barycentric(t, vec3(2.0, 1.0, 1.0)) << "\n";

    std::cout << triangulate(vertices) << "\n";*/

    window w(500, 500);

    w.run();
}
