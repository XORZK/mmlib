#ifndef WINDOW_H
#define WINDOW_H

#pragma once
#include "camera.h"
#include "color.h"
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

        void draw_point(vector<int64_t> p);

        void draw_point(vector<int64_t> p, color c);

        void draw_line(vector<int64_t> p1, vector<int64_t> p2);

        void draw_line(vector<int64_t> p1, vector<int64_t> p2, color c);

        void draw_circle(vector<int64_t> center, uint64_t radius);

        void draw_circle(vector<int64_t> center, uint64_t radius, color c);

        void run();

        void draw();
};

#endif
