#include "window.h"

void window::init_window() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
        return;
    }

    this->w = SDL_CreateWindow((!this->title.empty() ? this->title.c_str() : "window"), 
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    static_cast<int>(this->width),
                                    static_cast<int>(this->height),
                                    SDL_WINDOW_SHOWN);

    if (!this->w) {
        SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    this->r= SDL_CreateRenderer(this->w, -1, SDL_RENDERER_ACCELERATED);

    if (!this->r) {
        SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
        SDL_DestroyWindow(this->w);
        SDL_Quit();
        return;
    }

    this->initialized = true;
}

void window::init_camera() {
    this->c = new camera(this->width, this->height);
    this->c->compute_screen_coords(NEAR_DISTANCE, FAR_DISTANCE);
}

void window::set_render_color(color c) {
    SDL_SetRenderDrawColor(this->r, c.R(), c.G(), c.B(), c.A());
}

vector<double> window::convert_ndc(vector<double> ndc_vertex) const {
    vector<double> screen_vector = ndc_vertex;

    screen_vector[0] = (screen_vector[0] + 1) * (static_cast<double>(this->width)/2);
    screen_vector[1] = (screen_vector[1] + 1) * (static_cast<double>(this->height)/2);

    return screen_vector;
}

window::window() : title("window"), width(WINDOW_WIDTH), height(WINDOW_HEIGHT), delay(RENDER_DELAY), 
                   initialized(0), quit(0), paused(0) {
    this->init_window();
    this->init_camera();
}

window::window(const std::string t, 
               const uint64_t w, 
               const uint64_t h) : title(t), width(w), height(h), delay(RENDER_DELAY),
                                   initialized(0), quit(0), paused(0) {
    this->init_window();
    this->init_camera();
}

window::window(const std::string t, 
               const uint64_t w, 
               const uint64_t h, 
               const uint64_t d) : title(t), width(w), height(h), delay(d),
                                   initialized(0), quit(0), paused(0) {
    this->init_window();
    this->init_camera();
}

window::~window() {
    if (this->r) {
        SDL_DestroyRenderer(this->r);
    }

    if (this->w) {
        SDL_DestroyWindow(this->w);
    }

    SDL_Quit();
}

bool window::has_quit() const {
    return (this->quit);
}

bool window::is_running() const {
    return (!this->paused);
}

bool window::is_initialized() const {
    return (this->initialized);
}

void window::fill_bg(color c) {
    this->set_render_color(c);
    SDL_RenderClear(this->r);
}

void window::draw_point(vector<int64_t> p) {
    if (p.get_size() == 3) {
        p = vector<double>::create_vec4(p);
    }

    if (p.get_size() == 2) {
        SDL_RenderDrawPoint(this->r, p[0], p[1]);
    } else if (p.get_size() == 4 && p[2] != 0) {
        // draws (x,y,z) vertex to screen
        vector<double> screen = this->convert_ndc(c->get_ndc_vertex(p));
        this->draw_point(vector<double>::create_vec2(screen[0], screen[1]));
    } 
}

void window::draw_point(vector<int64_t> p, color c) {
    this->set_render_color(c);
    this->draw_point(p);
}

void window::draw_line(vector<int64_t> p1, vector<int64_t> p2) {
    if (p1.get_size() == 3 || p1.get_size() == 4) {
        p1 = this->convert_ndc(c->get_ndc_vertex(p1));
    }

    if (p2.get_size() == 3 || p2.get_size() == 4) {
        p2 = this->convert_ndc(c->get_ndc_vertex(p2));
    }

    SDL_RenderDrawLine(this->r, p1[0], p1[1], p2[0], p2[1]);
}

void window::draw_line(vector<int64_t> p1, vector<int64_t> p2, color c) {
    this->set_render_color(c);
    this->draw_line(p1, p2);
}

// Implements Bresenham's Circle Drawing Algorithm
void window::draw_circle(vector<int64_t> center, uint64_t radius) {
    // From { x, y } => { x, y, 1 }
    // Adjusting from 3D points to 2D points
    if (center.get_size() == 3 || center.get_size() == 4) {
        vector<double> adjusted_center = this->convert_ndc(c->get_ndc_vertex(center));
        vector<double> top_point = this->convert_ndc(c->get_ndc_vertex(vector<int64_t>::create_vec3(center[0], center[1] + radius, center[2])));

        radius = static_cast<uint64_t>(top_point[1] - adjusted_center[1]);
        center = adjusted_center.slice(2);
    }

    int64_t d = 3 - (2 * radius);
    int64_t x = 0, y = static_cast<int64_t>(radius);

    do {
        this->draw_point(center + vector<int64_t>::create_vec2(x, y));
        this->draw_point(center + vector<int64_t>::create_vec2(-x, y));
        this->draw_point(center + vector<int64_t>::create_vec2(x, -y));
        this->draw_point(center + vector<int64_t>::create_vec2(-x, -y));

        this->draw_point(center + vector<int64_t>::create_vec2(y, x));
        this->draw_point(center + vector<int64_t>::create_vec2(-y, x));
        this->draw_point(center + vector<int64_t>::create_vec2(y, -x));
        this->draw_point(center + vector<int64_t>::create_vec2(-y, -x));

        // y is inside circle
        if (d <= 0) {
            d += (4*x) + 6;
        } else {
            d += (4*(x-y)) + 10;
            y--;
        }

        x++;
    } while (x < y);
}

void window::draw_circle(vector<int64_t> center, uint64_t radius, color c) {
    this->set_render_color(c);
    this->draw_circle(center, radius);
}

void window::run() {
    while (!this->quit) {
        while (SDL_PollEvent(&this->event)) {
            if (this->event.type == SDL_QUIT) {
                this->quit = true;
            }
        }

        this->draw();
    }
}

void window::draw() {
    /*uint64_t length = 8;

    vector<double>* vertices = new vector<double>[]{
        vector<double>::create_vec4(-5, -5, -10),
        vector<double>::create_vec4(-5, -5, -9),
        vector<double>::create_vec4(-5, 5, -10),
        vector<double>::create_vec4(-5, 5, -9),
        vector<double>::create_vec4(5, -5, -10),
        vector<double>::create_vec4(5, -5, -9),
        vector<double>::create_vec4(5, 5, -10),
        vector<double>::create_vec4(5, 5, -9),
    };

    for (uint64_t i = 0; i < length; i++) {
        this->draw_point(vertices[i]);
    }*/


    color c(255,0,0);
    this->draw_circle(vector<double>::create_vec3(0, 0, -100), 30, c);

    SDL_RenderPresent(this->r);
}
