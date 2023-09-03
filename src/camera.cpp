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

// Assumes up direction is (0, -1, 0)
mat4<double> camera_look_at(vec3<double> eye,
						    vec3<double> target,
							vec3<double> up) {
	vec3<double> f = (eye-target).normalize();
	vec3<double> l = (up.cross(f)).normalize();
	vec3<double> u = f.cross(l);

	mat4<double> M;

	M[0] = vec4<double>(l.x(), 
						l.y(), 
						l.z(), 
						(-l.x() * eye.x()) + 
						(-l.y() * eye.y()) + 
						(-l.z() * eye.z()));

	M[1] = vec4<double>(u.x(),	
						u.y(),
						u.z(),
						(-u.x() * eye.x()) +
						(-u.y() * eye.y()) +
						(-u.z() * eye.z()));

	M[2] = vec4<double>(f.x(),	
						f.y(),
						f.z(),
						(-f.x() * eye.x()) + 
						(-f.y() * eye.y()) + 
						(-f.z() * eye.z()));

	M[3] = vec4<double>(0.0, 0.0, 0.0, 1.0);

	return M;
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
    position = vec3<double>(0.0, 0.0, DEFAULT_Z_POS);
}

void camera::pos(vec3<double> new_pos) {
    position = vec3<double>(new_pos);
}

vec3<double> camera::pos() const {
    return position;
}

vec3<double> camera::front() const {
	return front_dir;
}

vec3<double> camera::up() const {
	return up_dir;
}

void camera::translate(const vec3<double>& translation) {
    position += translation;
}

mat4<double> camera::compute_projection() const {
    return create_projection_matrix(B, T, L, R, N, F);
}

mat4<double> camera::camera_view() const {
	mat4<double> view = camera_look_at(position,
									   position + front_dir,
									   up_dir);

	return view;
}

void camera::reset() {
    fov = DEFAULT_CAMERA_FOV;
    position = vec3<double>(0.0, 0.0, DEFAULT_Z_POS);
}
