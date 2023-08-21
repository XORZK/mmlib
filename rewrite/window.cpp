#include "window.hpp"

// Initializes SDL Window
void window::initialize_window() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL Window: %s\n", SDL_GetError());
        return;
    }

    this->w = SDL_CreateWindow("SDL Window",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         this->width,
                         this->height,
                         SDL_WINDOW_SHOWN);

    if (!this->w) {
        SDL_Log("Failed to initialize SDL Window: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    this->r = SDL_CreateRenderer(this->w, -1, SDL_RENDERER_ACCELERATED);

    if (!this->r) {
        SDL_Log("Failed to initialize SDL Window: %s\n", SDL_GetError());
        SDL_DestroyWindow(w);
        SDL_Quit();
        return;
    }

    this->init = true;
}

void window::initialize_camera() {
    c = new camera(this->width, this->height);
    c->compute_screen_coordinates(DEFAULT_NEAR_DISTANCE, DEFAULT_FAR_DISTANCE);
}

void window::set_render_color(const color c) {
    SDL_SetRenderDrawColor(this->r, c.R(), c.G(), c.B(), c.A());
}

vec2<double> window::ndc_to_screen_coords(const vec4<double> ndc_vert) const {
    vec2<double> screen_vert = vec2((ndc_vert.x() + 1) * (this->width/2.0),
                                    (ndc_vert.y() + 1) * (this->height/2.0));

    return screen_vert;
}

vec2<double> window::cartesian_to_screen_coords(const vec4<double> vert) const {
    if (c == nullptr) 
        return vec2<double>();

    vec4<double> ndc_vert = c->compute_ndc(vert);

    return this->ndc_to_screen_coords(ndc_vert);
}

window::window() {
    initialize_window();
    initialize_camera();
}

window::window(const int64_t W, const int64_t H) : width(W), height(H) {
    initialize_window();
    initialize_camera();
}

window::window(const int64_t W, const int64_t H, const int64_t D) : width(W),
                                                                    height(H),
                                                                    delay(D) {
    initialize_window();
    initialize_camera();
}

window::~window() {
    if (this->r) 
        SDL_DestroyRenderer(this->r);

    if (this->w) 
        SDL_DestroyWindow(this->w);

    SDL_Quit();
}

bool window::has_quit() const {
    return quit;
}

bool window::is_running() const {
    return !paused;
}

bool window::is_initialized() const {
    return init;
}

void window::fill_background(const color c) {
    this->set_render_color(c);
    SDL_RenderClear(this->r);
}

// Assume point is already in terms of screen coordinates.
void window::draw_point(const vec2<double> point) {
    SDL_RenderDrawPoint(this->r, point.x(), point.y());
}

void window::draw_point(const vec3<double> point) {
    this->draw_point(vec4<double>(point, 1.0));
}

void window::draw_point(const vec4<double> point) {
    // Converts vertex from 3D space to NDC 
    if (point.z() == 0) 
        return;

    vec4<double> ndc_vert = c->compute_ndc(point);

    vec2<double> screen_vert = this->ndc_to_screen_coords(ndc_vert);
    this->draw_point(screen_vert);
}

void window::draw_point(const vec2<double> point, const color c) {
    this->set_render_color(c);
    this->draw_point(point);
}

void window::draw_point(const vec3<double> point, const color c) {
    this->set_render_color(c);
    this->draw_point(point);
}

void window::draw_point(const vec4<double> point, const color c) {
    this->set_render_color(c);
    this->draw_point(point);
}

void window::draw_line(const vec2<double> p1, 
                       const vec2<double> p2) {
    SDL_RenderDrawLine(this->r, p1.x(), p1.y(), p2.x(), p2.y());
}

void window::draw_line(const vec3<double> p1, 
                       const vec3<double> p2) {
    this->draw_line(vec4(p1, 1.0), vec4(p2, 1.0));
}

void window::draw_line(const vec4<double> p1,
                       const vec4<double> p2) {
    if (p1.z() == 0 || p2.z() == 0)
        return;

    vec4<double> first_ndc_vert = c->compute_ndc(p1),
                 second_ndc_vert = c->compute_ndc(p2);

    vec2<double> first_screen_vert = this->ndc_to_screen_coords(first_ndc_vert),
                 second_screen_vert = this->ndc_to_screen_coords(second_ndc_vert);

    this->draw_line(first_screen_vert, second_screen_vert);
}

void window::draw_line(const vec2<double> p1,
                       const vec2<double> p2,
                       const color c) {
    this->set_render_color(c);
    this->draw_line(p1, p2);
}

void window::draw_line(const vec3<double> p1,
                       const vec3<double> p2,
                       const color c) {
    this->set_render_color(c);
    this->draw_line(p1, p2);
}

void window::draw_line(const vec4<double> p1, 
                       const vec4<double> p2,
                       const color c) {
    this->set_render_color(c);
    this->draw_line(p1, p2);
}

// Bresenham's Circle Drawing Algorithm
void window::draw_wireframe_circle(const vec2<double> center,
                                   const int64_t radius) {
    int64_t D = 3 - (2 * radius);
    int64_t x = 0, y = radius;

    do {
        this->draw_point(center + vec2(x,y));
        this->draw_point(center + vec2(-x,y));
        this->draw_point(center + vec2(x,-y));
        this->draw_point(center + vec2(-x,-y));

        this->draw_point(center + vec2(y,x));
        this->draw_point(center + vec2(-y,x));
        this->draw_point(center + vec2(y,-x));
        this->draw_point(center + vec2(-y,-x));

        if (D <= 0) {
            D += 4 * x + 6;
        } else {
            D += 4 * (x-y) + 10;
            --y;
        }

        ++x;
    } while (x < y);
}

void window::draw_wireframe_circle(const vec3<double> center,
                                   const int64_t radius) {
    this->draw_wireframe_circle(vec4(center, 1.0), radius);
}

void window::draw_wireframe_circle(const vec4<double> center,
                                   const int64_t radius) {
    if (center.z() == 0) 
        return;

    vec4<double> top_point = center + vec4<double>(0.0, (double) radius, 0.0, 0.0);

    vec2<double> center_screen = cartesian_to_screen_coords(center),
                 top_screen = cartesian_to_screen_coords(top_point);

    this->draw_wireframe_circle(center_screen, (int64_t) (top_screen.y() - center_screen.y()));
}

void window::draw_wireframe_circle(const vec2<double> center,
                                   const int64_t radius,
                                   const color c) {
    this->set_render_color(c);
    this->draw_wireframe_circle(center, radius);
}

void window::draw_wireframe_circle(const vec3<double> center,
                                   const int64_t radius,
                                   const color c) {
    this->set_render_color(c);
    this->draw_wireframe_circle(center, radius);
}

void window::draw_wireframe_circle(const vec4<double> center,
                                   const int64_t radius,
                                   const color c) {
    this->set_render_color(c);
    this->draw_wireframe_circle(center, radius);
}

// Brensenham's Circle Drawing Algorithm (Again)
void window::draw_filled_circle(const vec2<double> center, 
                                const int64_t radius) {

    int64_t D = 3 - (2 * radius);
    int64_t x = 0, y = radius;

    do {
        this->draw_line(center + vec2(x,y), center + vec2(x,-y));
        this->draw_line(center + vec2(y,x), center + vec2(y,-x));
        this->draw_line(center - vec2(x,y), center - vec2(x,-y));
        this->draw_line(center - vec2(y,x), center - vec2(y,-x));

        if (D <= 0) {
            D += 4 * x + 6;
        } else {
            D += 4 * (x-y) + 10;
            --y;
        }

        ++x;
    } while (x < y);
}

void window::draw_filled_circle(const vec3<double> center,
                                const int64_t radius) {
    this->draw_filled_circle(vec4(center, 1.0), radius);
}

void window::draw_filled_circle(const vec4<double> center,
                                const int64_t radius) {
    if (center.z() == 0)
        return;

    vec4<double> top_point = center + vec4<double>(0.0, (double) radius, 0.0, 0.0);

    vec2<double> center_screen = cartesian_to_screen_coords(center),
                 top_screen = cartesian_to_screen_coords(top_point);

    this->draw_wireframe_circle(center_screen, (int64_t) (top_screen.y() - center_screen.y()));
}

void window::draw_filled_circle(const vec2<double> center,
                                const int64_t radius,
                                const color c) { 
    this->set_render_color(c);
    this->draw_filled_circle(center, radius);
}

void window::draw_filled_circle(const vec3<double> center,
                                const int64_t radius,
                                const color c) { 
    this->set_render_color(c);
    this->draw_filled_circle(center, radius);
}

void window::draw_filled_circle(const vec4<double> center,
                                const int64_t radius,
                                const color c) { 
    this->set_render_color(c);
    this->draw_filled_circle(center, radius);
}

void window::draw_wireframe_triangle(const vec2<double> v1,
                                     const vec2<double> v2,
                                     const vec2<double> v3) {
    this->draw_line(v1, v2);
    this->draw_line(v1, v3);
    this->draw_line(v2, v3);
}

void window::draw_wireframe_triangle(const vec3<double> v1,
                                     const vec3<double> v2,
                                     const vec3<double> v3) {
    this->draw_wireframe_triangle(vec4(v1, 1.0),    
                                  vec4(v2, 1.0),
                                  vec4(v3, 1.0));
}

void window::draw_wireframe_triangle(const vec4<double> v1,
                                     const vec4<double> v2,
                                     const vec4<double> v3) {
    vec2<double> v1_screen = cartesian_to_screen_coords(v1),
                 v2_screen = cartesian_to_screen_coords(v2),     
                 v3_screen = cartesian_to_screen_coords(v3);

    this->draw_wireframe_triangle(v1_screen, v2_screen, v3_screen);
}

void window::draw_wireframe_triangle(const triangle& T) {
    this->draw_wireframe_triangle(T.v1(), T.v2(), T.v3());
}

void window::draw_wireframe_triangle(const vec2<double> v1,
                                     const vec2<double> v2,
                                     const vec2<double> v3,
                                     const color c) {
    this->set_render_color(c);
    this->draw_wireframe_triangle(v1, v2, v3);
}


void window::draw_wireframe_triangle(const vec3<double> v1,
                                     const vec3<double> v2,
                                     const vec3<double> v3,
                                     const color c) {
    this->set_render_color(c);
    this->draw_wireframe_triangle(v1, v2, v3);
}

void window::draw_wireframe_triangle(const vec4<double> v1,
                                     const vec4<double> v2,
                                     const vec4<double> v3,
                                     const color c) {
    this->set_render_color(c);
    this->draw_wireframe_triangle(v1, v2, v3);
}

void window::draw_wireframe_triangle(const triangle& T,
                                     const color c) {
    this->set_render_color(c);
    this->draw_wireframe_triangle(T);
}

void window::draw_filled_triangle(vec2<double> v1,
                                  vec2<double> v2,
                                  vec2<double> v3) {
    // Sort v1, v2, v3 by ascending y value
    if (v1.y() > v2.y()) 
        std::swap(v1, v2);

    if (v2.y() > v3.y())
        std::swap(v2, v3);

    if (v2.y() > v1.y())
        std::swap(v1, v2);

    double bounds = v2.y();

    for (int64_t y = v1.y()+1; y < v3.y(); y++) {
        double x1 = (y <= bounds ? 
                     interp<double>(v1.x(), v1.y(), v2.x(), v2.y(), y) : 
                     interp<double>(v2.x(), v2.y(), v3.x(), v3.y(), y));

        double x2 = interp<double>(v1.x(), v1.y(), v3.x(), v3.y(), y);

        this->draw_line(vec2<double>(x1, y), vec2<double>(x2, y));
    }
}

void window::draw_filled_triangle(vec3<double> v1,
                                  vec3<double> v2,
                                  vec3<double> v3) {
    this->draw_filled_triangle(vec4(v1, 1.0), vec4(v2, 1.0), vec4(v3, 1.0));
}

void window::draw_filled_triangle(vec4<double> v1,
                                  vec4<double> v2,
                                  vec4<double> v3) {
    vec2<double> c1 = cartesian_to_screen_coords(v1),
                 c2 = cartesian_to_screen_coords(v2),
                 c3 = cartesian_to_screen_coords(v3);

    return this->draw_filled_triangle(c1, c2, c3);
}

void window::draw_filled_triangle(const triangle& T) {
    this->draw_filled_triangle(T.v1(), T.v2(), T.v3());
}

void window::draw_filled_triangle(vec2<double> v1,
                                  vec2<double> v2,
                                  vec2<double> v3,
                                  const color c) {
    this->set_render_color(c);
    this->draw_filled_triangle(v1, v2, v3);
}

void window::draw_filled_triangle(vec3<double> v1,
                                  vec3<double> v2,
                                  vec3<double> v3,
                                  const color c) {
    this->set_render_color(c);
    this->draw_filled_triangle(v1, v2, v3);
}

void window::draw_filled_triangle(vec4<double> v1,
                                  vec4<double> v2,
                                  vec4<double> v3,
                                  const color c) {
    this->set_render_color(c);
    this->draw_filled_triangle(v1, v2, v3);
}

void window::draw_filled_triangle(const triangle& T,
                                  const color c) {
    this->set_render_color(c);
    this->draw_filled_triangle(T);
}

void window::run() {
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case (SDLK_SPACE): {
                        paused = 1-paused;
                        break;
                    }

                    default: 
                        break; 
                }
            }
        }

        if (!paused)
            this->draw();
    }
}

void window::draw() {
    list<vec3<double>> vertices;

    vertices.push_back(vec3<double>(173,29,-500)); 
    vertices.push_back(vec3<double>(181,88,-500)); 
    vertices.push_back(vec3<double>(246,133,-500)); 
    vertices.push_back(vec3<double>(210,148,-500)); 
    vertices.push_back(vec3<double>(240,147,-500)); 
    vertices.push_back(vec3<double>(241,173,-500)); 
    vertices.push_back(vec3<double>(209,192,-500)); 
    vertices.push_back(vec3<double>(221,196,-500)); 
    vertices.push_back(vec3<double>(205,220,-500)); 
    vertices.push_back(vec3<double>(154,170,-500));  
    vertices.push_back(vec3<double>(93,222,-500)); 
    vertices.push_back(vec3<double>(63,223,-500)); 
    vertices.push_back(vec3<double>(72,155,-500)); 
    vertices.push_back(vec3<double>(109,104,-500));
    vertices.push_back(vec3<double>(68,129,-500)); 
    vertices.push_back(vec3<double>(59,73,-500)); 
    vertices.push_back(vec3<double>(27,64,-500)); 
    vertices.push_back(vec3<double>(38,39,-500)); 
    vertices.push_back(vec3<double>(57,49,-500)); 
    vertices.push_back(vec3<double>(118,25,-500)); 
    vertices.push_back(vec3<double>(142,78,-500)); 
    vertices.push_back(vec3<double>(151,50,-500)); 
    vertices.push_back(vec3<double>(151,21,-500));

    list<triangle> T = triangulate(vertices);

    for (int64_t k = 0; k < T.size(); k++) 
       this->draw_wireframe_triangle(T[k], color(255, 255, 0));




    SDL_RenderPresent(this->r);
    SDL_Delay(this->delay);
}