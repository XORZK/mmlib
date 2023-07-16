#include "camera.h"

// Computes { r, l, t, b }
void camera::compute_screen_coords(const double init_n, const double init_f) {
    double scale = std::tan(this->vertical_fov * 0.5 * M_PI / 180) * init_n;

    this->n = init_n;
    this->f = init_f;
    this->r = (this->aspect_ratio * scale);
    this->l = -this->r;
    this->t = scale;
    this->b = -this->t;
}

camera::camera(uint64_t width, uint64_t height) {
    this->aspect_ratio = static_cast<double>(width)/height;
    this->reset();
}

camera::camera(uint64_t width, uint64_t height, double fov) {
    this->aspect_ratio = static_cast<double>(width)/height;
    this->vertical_fov = fov;
    this->pos = vector<int64_t>::zero(3);
}

void camera::translate(vector<int64_t> translation) {
    this->pos += translation;
}

vector<int64_t> camera::get_pos() const {
    return this->pos;
}

matrix<double> camera::get_projection_matrix() const {
    return matrix<double>::create_projection_matrix(this->b, this->t, 
                                                    this->l, this->r,
                                                    this->n, this->f);
}

void camera::reset() {
    this->vertical_fov = DEFAULT_CAMERA_FOV;
    this->pos = vector<int64_t>::zero(3);
}
