#ifndef WINDOW_HPP
#define WINDOW_HPP

#pragma once
#include "color.hpp"
#include "camera.hpp"
#include "triangle.hpp"
#include "mat.hpp"
#include "vec.hpp"

#include <SDL2/SDL.h>

#define RENDERER_DELAY 15
#define DEFAULT_WINDOW_WIDTH 500
#define DEFAULT_WINDOW_HEIGHT 500
#define DEFAULT_NEAR_DISTANCE 0.1
#define DEFAULT_FAR_DISTANCE 100

double relative_line_distance(const vec2<double> A, 
                              const vec2<double> B,
                              const vec2<double> P);

color interpolate_color(const color c1,
                        const color c2,
                        const double P);

class window {
    private:
        camera *c = nullptr;

        bool init = false, quit = false, paused = false, modified = true;

        int64_t width = DEFAULT_WINDOW_WIDTH, 
                height = DEFAULT_WINDOW_HEIGHT, 
                delay = RENDERER_DELAY;

        int64_t global_time = 0;

        SDL_Window *w;
        SDL_Renderer *r;
        SDL_Event event;

        void initialize_window();

        void initialize_camera();

        void set_render_color(const color c);

        vec2<double> ndc_to_screen_coords(const vec4<double> ndc_vert) const;

        vec2<double> cartesian_to_screen_coords(const vec4<double> vert) const;
    public:
        window();

        window(const int64_t W, const int64_t H);

        window(const int64_t W, const int64_t H, const int64_t D);

        ~window();

        bool has_quit() const;

        bool is_initialized() const;

        bool is_running() const;

        void fill_background(const color c);

        void draw_point(const vec2<double> point);

        void draw_point(const vec3<double> point);

        void draw_point(const vec4<double> point);

        void draw_point(const vec2<double> point,   
                        const color c);

        void draw_point(const vec3<double> point, 
                        const color c);

        void draw_point(const vec4<double> point, 
                        const color c);

        void draw_line(const vec2<double> p1, 
                       const vec2<double> p2);

        void draw_line(const vec3<double> p1, 
                       const vec3<double> p2);

        void draw_line(const vec4<double> p1, 
                       const vec4<double> p2);

        void draw_line(const vec2<double> p1, 
                       const vec2<double> p2, 
                       const color c);

        void draw_line(const vec3<double> p1, 
                       const vec3<double> p2, 
                       const color c);

        void draw_line(const vec4<double> p1, 
                       const vec4<double> p2, 
                       const color c);

        void draw_wireframe_circle(const vec2<double> center, 
                                   const int64_t radius);

        void draw_wireframe_circle(const vec3<double> center,
                                   const int64_t radius);

        void draw_wireframe_circle(const vec4<double> center,
                                   const int64_t radius);

        void draw_wireframe_circle(const vec2<double> center, 
                                   const int64_t radius, 
                                   const color c);

        void draw_wireframe_circle(const vec4<double> center,
                                   const int64_t radius,
                                   const color c);

        void draw_wireframe_circle(const vec3<double> center,
                                   const int64_t radius,
                                   const color c);

        void draw_filled_circle(const vec2<double> center, 
                                const int64_t radius);

        void draw_filled_circle(const vec3<double> center,
                                const int64_t radius);

        void draw_filled_circle(const vec4<double> center,
                                const int64_t radius);

        void draw_filled_circle(const vec2<double> center, 
                                const int64_t radius, 
                                const color c);

        void draw_filled_circle(const vec3<double> center,
                                const int64_t radius,
                                const color c);

        void draw_filled_circle(const vec4<double> center,
                                const int64_t radius,
                                const color c);

        void draw_wireframe_triangle(const vec2<double> v1,
                                     const vec2<double> v2,
                                     const vec2<double> v3);

        void draw_wireframe_triangle(const vec3<double> v1,
                                     const vec3<double> v2, 
                                     const vec3<double> v3);

        void draw_wireframe_triangle(const vec4<double> v1, 
                                     const vec4<double> v2,
                                     const vec4<double> v3);

        void draw_wireframe_triangle(const triangle& T);

        void draw_wireframe_triangle(const vec2<double> v1,
                                     const vec2<double> v2,
                                     const vec2<double> v3,
                                     const color c);

        void draw_wireframe_triangle(const vec3<double> v1,
                                     const vec3<double> v2, 
                                     const vec3<double> v3,
                                     const color c);

        void draw_wireframe_triangle(const vec4<double> v1, 
                                     const vec4<double> v2,
                                     const vec4<double> v3,
                                     const color c);

        void draw_wireframe_triangle(const triangle& T, 
                                     const color c);

        void draw_filled_triangle(vec2<double> v1,
                                  vec2<double> v2,
                                  vec2<double> v3);

        void draw_filled_triangle(vec3<double> v1,
                                  vec3<double> v2, 
                                  vec3<double> v3);

        void draw_filled_triangle(vec4<double> v1, 
                                  vec4<double> v2,
                                  vec4<double> v3);

        void draw_filled_triangle(const triangle& T);

        void draw_filled_triangle(vec2<double> v1,
                                  vec2<double> v2,
                                  vec2<double> v3,
                                  const color c);

        void draw_filled_triangle(vec3<double> v1,
                                  vec3<double> v2, 
                                  vec3<double> v3,
                                  const color c);

        void draw_filled_triangle(vec4<double> v1, 
                                  vec4<double> v2,
                                  vec4<double> v3,
                                  const color c);

        void draw_filled_triangle(const triangle& T, 
                                  const color c);

        void draw_colored_line(vec2<double> v1,
                               vec2<double> v2,
                               const color c1,
                               const color c2);

        void draw_rainbow_triangle(vec2<double> v1,
                                   vec2<double> v2,
                                   vec2<double> v3);

        void draw_rainbow_triangle(const vec3<double> v1,
                                   const vec3<double> v2,
                                   const vec3<double> v3);

        void draw_rainbow_triangle(const vec4<double> v1,
                                   const vec4<double> v2,
                                   const vec4<double> v3);

        void run();
        
        void draw(); 
};

#endif
