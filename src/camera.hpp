#ifndef CAMERA_HPP
#define CAMERA_HPP

#pragma once
#include "vec.hpp"
#include "mat.hpp"

#include <math.h>
#include <stdint.h>

#define DEFAULT_CAMERA_FOV 90

template <typename T>
vec4<double> compute_ndc_vert(vec3<T> v3, const mat4<double>& proj);

mat4<double> create_projection_matrix(const double B, const double T,
                                      const double L, const double R,
                                      const double N, const double F);

class camera {
    private:
        double fov, ratio;
        // Near, Far, Right, Left, Top, Bottom Values
        double N, F, R, L, T, B;

        vec3<int64_t>* position;
    public:
        camera(int64_t W, int64_t H);

        camera(int64_t W, int64_t H, double fov);

		~camera() {
			(*position).~vec3();
		}

        void compute_screen_coordinates(const double init_n, const double init_f);

        void pos(vec3<int64_t> new_pos);

        vec3<int64_t> pos() const;

        void translate(vec3<int64_t> translate);

        mat4<double> compute_projection() const;

        template <typename U>
        vec4<double> compute_ndc(vec3<U> vert) const;

        template <typename U>
        vec4<double> compute_ndc(vec4<U> vert) const;

        void reset();
};

template <typename U> 
vec4<double> camera::compute_ndc(vec3<U> vert) const {
    vec4<double> v4 = static_cast<vec4<double>>(vec4<U>(vert));
    mat4<double> M = this->compute_projection();

    vec4<double> clip = M * v4;

    clip /= clip.w();

    return clip;
}

template <typename U>
vec4<double> camera::compute_ndc(vec4<U> vert) const {
    mat4<double> M = this->compute_projection();

    vec4<double> clip = M * vert;

    clip /= clip.w();

    return clip;
}

#endif
