#ifndef WINDOW_H
#define WINDOW_H

#pragma once
#include "camera.h"
#include "color.h"
#include "matrix.h"
#include "triangle.h"
#include "vector.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <stdint.h>

#define RENDER_DELAY 15
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define NEAR_DISTANCE 0.1
#define FAR_DISTANCE 100

class window {
    private:
        int64_t angle = 0;
        camera *c;
        bool initialized, quit, paused;
        uint64_t delay, width, height;
        std::string title;

        SDL_Window* w;
        SDL_Renderer* r;
        SDL_Event event;

        void init_window();

        void init_camera();

        void set_render_color(color c);

        vector<double> convert_ndc(vector<double> ndc_vertex) const;

    public:
        window();

        window(const std::string t, const uint64_t w, const uint64_t h);

        window(const std::string t, const uint64_t w, const uint64_t h, const uint64_t d);

        ~window();

        bool has_quit() const;

        bool is_running() const;

        bool is_initialized() const;

        void fill_bg(color c);

        void draw_point(vector<double> p);

        void draw_point(vector<double> p, color c);

        void draw_line(vector<double> p1, vector<double> p2);

        void draw_line(vector<double> p1, vector<double> p2, color c);

        void draw_circle(vector<double> center, uint64_t radius, bool fill = false);

        void draw_circle(vector<double> center, uint64_t radius, color c, bool fill = false );

        void draw_wireframe_triangle(vector<double> p1, 
                                     vector<double> p2, 
                                     vector<double> p3);

        void draw_wireframe_triangle(vector<double> p1, 
                                     vector<double> p2, 
                                     vector<double> p3,
                                     color c);

        void draw_filled_triangle(vector<double> p1,
                                  vector<double> p2, 
                                  vector<double> p3);

        void draw_filled_triangle(vector<double> p1,
                                  vector<double> p2, 
                                  vector<double> p3,
                                  color outline,
                                  color fill);


        void draw_filled_triangle(vector<double> p1,
                                  vector<double> p2,
                                  vector<double> p3,
                                  color c);

        void draw_wireframe_triangle(triangle t);

        void draw_wireframe_triangle(triangle t, color c);

        void draw_filled_triangle(triangle t);

        void draw_filled_triangle(triangle t, color outline, color fill);

        void draw_filled_triangle(triangle t, color c);

        void draw_wireframe_polygon(vector<vector<double>> vertices);

        void draw_wireframe_polygon(vector<vector<double>> vertices, color c);

        void draw_filled_polygon(vector<vector<double>> vertices);

        void draw_filled_polygon(vector<vector<double>> vertices, color c);

        void run();

        void draw();
};

#endif
