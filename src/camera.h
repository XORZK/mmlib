#ifndef CAMERA_H
#define CAMERA_H

#pragma once
#include "vector.h"
#include "matrix.h"
#include <iostream>
#include <math.h>
#include <stdint.h>

#define DEFAULT_CAMERA_FOV 90

class camera {
    private:
        double aspect_ratio, vertical_fov;
        double n, f, r, l, t, b;

        vector<int64_t> pos;
    public:
        camera(uint64_t width, uint64_t height);

        camera(uint64_t width, uint64_t height, double fov);

        void compute_screen_coords(const double init_n, const double init_f);

        void translate(vector<int64_t> translation);

        vector<int64_t> get_pos() const;

        matrix<double> get_projection_matrix() const;

        template <typename T>
        vector<double> get_ndc_vertex(vector<T> vertex) const;

        void reset();
};

/* Eye Coordinates --> Clip Coordinates --> NDC (Normalized Device Coordinates) */
template <typename T>
vector<double> camera::get_ndc_vertex(vector<T> v_e) const {
    if (v_e.get_size() == 3) {
        v_e = create_vec4(v_e);
    }

    vector<double> v = static_cast<vector<double>>(v_e);
    matrix<double> M = this->get_projection_matrix();

    // Clip Coordinates (Vector)
    vector<double> v_c = M * v;

    /*
    for (uint64_t i = 0; i < v_c.get_size(); i++) {
        // Vertex is not on screen.
        if (v_c[i] <= -v_c[3] || v_c[i] >= v_c[3]) {
        }
    }*/

    // Divide by w_c component (which should be -z_e)
    vector<double> ndc_vertex = (v_c / v_c[3]);

    return ndc_vertex;
}

#endif
