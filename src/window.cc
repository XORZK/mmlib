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

void window::draw_point(vector<double> p) {
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

void window::draw_point(vector<double> p, color c) {
    this->set_render_color(c);
    this->draw_point(p);
}

void window::draw_line(vector<double> p1, vector<double> p2) {
    if (p1.get_size() == 3 || p1.get_size() == 4) {
        p1 = this->convert_ndc(c->get_ndc_vertex(p1));
    }

    if (p2.get_size() == 3 || p2.get_size() == 4) {
        p2 = this->convert_ndc(c->get_ndc_vertex(p2));
    }

    SDL_RenderDrawLine(this->r, p1[0], p1[1], p2[0], p2[1]);
}

void window::draw_line(vector<double> p1, vector<double> p2, color c) {
    this->set_render_color(c);
    this->draw_line(p1, p2);
}

// Implements Bresenham's Circle Drawing Algorithm
void window::draw_circle(vector<double> center, uint64_t radius, bool fill) {
    // From { x, y } => { x, y, 1 }
    // Adjusting from 3D points to 2D points
    if (center.get_size() == 3 || center.get_size() == 4) {
        vector<double> adjusted_center = this->convert_ndc(c->get_ndc_vertex(center));
        vector<double> top_point = this->convert_ndc(c->get_ndc_vertex(vector<double>::create_vec3(center[0], center[1] + radius, center[2])));

        radius = static_cast<uint64_t>(top_point[1] - adjusted_center[1]);
        center = adjusted_center.slice(2);
    }

    int64_t d = 3 - (2 * radius);
    int64_t x = 0, y = static_cast<int64_t>(radius);

    do {
        // Fill from (x,y) to (x,-y), (y,x) to (y,-x),
        // (-x,-y) to (-x,y), (-y,-x) to (-y,x)
        if (fill) {
            this->draw_line(center + vector<double>::create_vec2(x,y),
                            center + vector<double>::create_vec2(x, -y));
            this->draw_line(center + vector<double>::create_vec2(y,x),
                            center + vector<double>::create_vec2(y, -x));
            this->draw_line(center - vector<double>::create_vec2(x,y),
                            center - vector<double>::create_vec2(x, -y));
            this->draw_line(center - vector<double>::create_vec2(y,x),
                            center - vector<double>::create_vec2(y, -x));
        } else {
            this->draw_point(center + vector<double>::create_vec2(x, y));
            this->draw_point(center + vector<double>::create_vec2(-x, y));
            this->draw_point(center + vector<double>::create_vec2(x, -y));
            this->draw_point(center + vector<double>::create_vec2(-x, -y));

            this->draw_point(center + vector<double>::create_vec2(y, x));
            this->draw_point(center + vector<double>::create_vec2(-y, x));
            this->draw_point(center + vector<double>::create_vec2(y, -x));
            this->draw_point(center + vector<double>::create_vec2(-y, -x));
        }

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

void window::draw_circle(vector<double> center, uint64_t radius, color c, bool fill) {
    this->set_render_color(c);
    this->draw_circle(center, radius, fill);
}

void window::draw_wireframe_triangle(vector<double> p1,
                                     vector<double> p2,
                                     vector<double> p3) {
    this->draw_line(p1, p2);
    this->draw_line(p1, p3);
}

void window::draw_wireframe_triangle(vector<double> p1,
                                     vector<double> p2,
                                     vector<double> p3,
                                     color c) {
    this->set_render_color(c);
    this->draw_line(p1, p2);
    this->draw_line(p1, p3);
    this->draw_line(p2, p3);
}

// Assume p1, p2, and p3 are 2D vertices NDC
void window::draw_filled_triangle(vector<double> p1, 
                                  vector<double> p2,
                                  vector<double> p3) {
    if (p1.get_size() == 3 || p1.get_size() == 4) { p1 = this->convert_ndc(c->get_ndc_vertex(p1)); }
    if (p2.get_size() == 3 || p2.get_size() == 4) { p2 = this->convert_ndc(c->get_ndc_vertex(p2)); }
    if (p3.get_size() == 3 || p3.get_size() == 4) { p3 = this->convert_ndc(c->get_ndc_vertex(p3)); }

    // Sort vertexes by ascending y value (p1, p2, p3) such that p1.y < p2.y < p3.y
    if (p2[1] < p1[1]) { std::swap(p2, p1); }
    if (p3[1] < p1[1]) { std::swap(p3, p1); }
    if (p3[1] < p2[1] || (p2[1] == p3[1] && p3[0] == p1[0])) { std::swap(p3, p2); } 

    double boundary = p2[1];

    // increment y at every step, and draw a line between x1 and x2.
    for (uint64_t y = p1[1]+1; y < p3[1]; y++) {
        // figure out which edges in the triangle the y point lies within.
        // if y < boundary: slopes 1->2, 1->3, otherwise slopes: 2->3, 1->3
        double x1 = (y <= boundary ? interp<double>(p1[0], p1[1], p2[0], p2[1], y) : 
                                     interp<double>(p2[0], p2[1], p3[0], p3[1], y)); 

        double x2 = interp<double>(p1[0], p1[1], p3[0], p3[1], y);

        this->draw_line(vector<double>::create_vec2(x1, y),
                        vector<double>::create_vec2(x2, y));
    }
}

void window::draw_filled_triangle(vector<double> p1,
                                  vector<double> p2,
                                  vector<double> p3,
                                  color outline,
                                  color fill) {
    this->set_render_color(fill);
    this->draw_filled_triangle(p1, p2, p3);
    this->draw_wireframe_triangle(p1, p2, p3, outline);
}

void window::draw_filled_triangle(vector<double> p1,
                                  vector<double> p2,
                                  vector<double> p3,
                                  color c) {
    this->draw_filled_triangle(p1, p2, p3, c, c);
}

void window::run() {
    while (!this->quit) {
        while (SDL_PollEvent(&this->event)) {
            if (this->event.type == SDL_QUIT) {
                this->quit = true;
            }

            if (this->event.type == SDL_KEYDOWN) {
                switch (this->event.key.keysym.sym) {
                    case (SDLK_SPACE): {
                        this->paused = 1-this->paused;
                        break;
                    }

                    default: break;
                }
            }
        }

        if (!this->paused) {
            this->draw();
        }
    }
}

void window::draw() {
    this->fill_bg(color(0, 0, 0));

    matrix<double> R = matrix<double>::create_x_rotation(this->angle++, true);

    color g(0, 255, 0);
    color r(255,0,0);

    this->draw_filled_triangle(R * vector<double>::create_vec3(-0.25, 0, -3),
                               R * vector<double>::create_vec3(-0.25, 0.25, -3),
                               R * vector<double>::create_vec3(0.25, 0, -3),
                               g, r);

    SDL_RenderPresent(this->r);
    SDL_Delay(this->delay);
}
