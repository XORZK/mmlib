#include "camera.hpp"

template <typename T> 
vec4<double> compute_ndc_vert(vec3<T> v3, const mat4<double>& proj) {
    vec4<double> v4 = static_cast<vec4<double>>(vec4<T>(v3));

    vec4<double> clip = proj * v4;

    clip /= clip.w();

    return clip;
}

mat4<double> create_projection_matrix(const double B, const double T,
                                      const double L, const double R,
                                      const double N, const double F) {
    mat4<double> proj;

    proj[0] = vec4<double>((2*N)/(R-L), 0.0, (R+L)/(R-L), 0.0);
    proj[1] = vec4<double>(0.0, (2*N)/(T-B), (T+B)/(T-B), 0.0);
    proj[2] = vec4<double>(0.0, 0.0, -(F+N)/(F-N), (-2*F*N)/(F-N));
    proj[3] = vec4<double>(0.0, 0.0, -1.0, 0.0);

    return proj;
}


void camera::compute_screen_coordinates(const double init_n, const double init_f) {
    double S = std::tan(fov * 0.5 * M_PI / 180) * init_n;

    N = init_n;
    F = init_f;
    R = ratio * S;
    L = -R;
    T = S;
    B = -T;
}

camera::camera(int64_t W, int64_t H) {
    ratio = static_cast<double>(W)/H;
    this->reset();
}

camera::camera(int64_t W, int64_t H, double f) {
    ratio = static_cast<double>(W)/H;
    fov = f;
    position = vec3<int64_t>::zero();
}

void camera::pos(vec3<int64_t> new_pos) {
    position = new_pos;
}

vec3<int64_t> camera::pos() const {
    return position;
}

void camera::translate(vec3<int64_t> translation) {
    position += translation;
}

mat4<double> camera::compute_projection() const {
    return create_projection_matrix(B, T, L, R, N, F);
}

void camera::reset() {
    fov = DEFAULT_CAMERA_FOV;
    position = vec3<int64_t>::zero();
}
