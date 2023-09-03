#ifndef CAMERA_HPP
#define CAMERA_HPP

#pragma once
#include "vec.hpp"
#include "mat.hpp"

#include <math.h>
#include <stdint.h>

#define DEFAULT_CAMERA_FOV 90
#define DEFAULT_Z_POS 3

template <typename T>
vec4<double> compute_ndc_vert(vec3<T> v3, const mat4<double>& proj);

mat4<double> create_projection_matrix(const double B, const double T,
                                      const double L, const double R,
                                      const double N, const double F);

mat4<double> camera_look_at(vec3<double> eye,
						    vec3<double> target,
							vec3<double> up = vec3(0.0,1.0,0.0));

class camera {
    private:
        double fov, ratio;
        // Near, Far, Right, Left, Top, Bottom Values
        double N, F, R, L, T, B;

		// Looks down -z.
        vec3<double> position = vec3<double>(0, 0, DEFAULT_Z_POS),
					 front_dir = vec3<double>(0, 0, -1), 
					 up_dir = vec3<double>(0, -1, 0);
    public:
        camera(int64_t W, int64_t H);

        camera(int64_t W, int64_t H, double fov);

		~camera() {}

        void compute_screen_coordinates(const double init_n, const double init_f);

        void pos(vec3<double> new_pos);

        vec3<double> pos() const;

        vec3<double> front() const;

        vec3<double> up() const;

        void translate(const vec3<double>& translate);

        mat4<double> compute_projection() const;

        template <typename U>
        vec4<double> compute_ndc(vec3<U> vert) const;

        template <typename U>
        vec4<double> compute_ndc(vec4<U> vert) const;

		mat4<double> camera_view() const;

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
