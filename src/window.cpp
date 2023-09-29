#include "window.hpp"

double relative_line_distance(const vec2<double>& A, 
                              const vec2<double>& B,
                              const vec2<double>& P) {
    double line_dist = distance(A, B),
           point_dist = distance(A, P);

    return point_dist/line_dist;
}

color interpolate_color(color& c1,
                        color& c2,
                        const double P) {
    return color(c1.R() * P + c2.R() * (1-P),
                 c1.G() * P + c2.G() * (1-P),
                 c1.B() * P + c2.B() * (1-P));
}

// Initializes SDL Window
void window::initialize_window() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL Window: %s\n", SDL_GetError());
        return;
    }

    this->w = SDL_CreateWindow("balls",
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
    this->cam = new camera(this->width, this->height);
    this->cam->compute_screen_coordinates(DEFAULT_NEAR_DISTANCE, DEFAULT_FAR_DISTANCE);
	view_mat = new mat4<double>(cam->camera_view());
}

void window::initialize_light() {
	this->l = new light();
}

void window::set_render_color(color c, bool cache) {
	if (cache)
		this->current_color = new color(c);
    SDL_SetRenderDrawColor(this->r, c.R(), c.G(), c.B(), c.A());
}

vec2<double> window::ndc_to_screen_coords(const vec4<double>& ndc_vert) const {
    vec2<double> screen_vert = vec2((ndc_vert.x() + 1) * (this->width/2.0),
                                    (ndc_vert.y() + 1) * (this->height/2.0));

    return screen_vert;
}

vec2<double> window::cartesian_to_screen_coords(const vec4<double>& vert) const {
    if (this->cam == nullptr) 
        return vec2<double>();

    vec4<double> ndc_vert = cam->compute_ndc(vert);

    return this->ndc_to_screen_coords(ndc_vert);
}

list<vec2<double>> window::cartesian_to_screen_coords(const list<vec3<double>> &points) const {
	list<vec4<double>> convert;

	int64_t N = points.size();

	linked_node<vec3<double>> *head = points.front();

	for (int64_t k = 0; k < N; k++) {
		vec3<double> P = head->value();

		convert.push_back(vec4(P, 1.0));

		head = head->next();
	}

	return cartesian_to_screen_coords(convert);
}

list<vec2<double>> window::cartesian_to_screen_coords(const list<vec4<double>> &points) const {
	list<vec2<double>> screen;

	int64_t N = points.size();

	linked_node<vec4<double>> *head = points.front();

	for (int64_t k = 0; k < N; k++) {
		vec4<double> P = head->value();

		screen.push_back(cartesian_to_screen_coords(P));

		head = head->next();
	}

	return screen;
}

window::window() {
    initialize_window();
    initialize_camera();
	initialize_light();
}

window::window(const int64_t W, const int64_t H) : width(W), height(H) {
    initialize_window();
    initialize_camera();
	initialize_light();
}

window::window(const int64_t W, const int64_t H, const int64_t D) : width(W),
                                                                    height(H),
                                                                    delay(D) {
    initialize_window();
    initialize_camera();
	initialize_light();
}

window::~window() {
	free(cam);
	free(view_mat);

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

void window::fill_background(color c) {
    this->set_render_color(c);
    SDL_RenderClear(this->r);
}

// Assume point is already in terms of screen coordinates.
void window::draw_point(const vec2<double>& point) {
    SDL_RenderDrawPoint(this->r, point.x(), point.y());
}

void window::draw_point(const vec3<double>& point) {
	vec4<double> v4 = vec4<double>(point, 1.0);

    this->draw_point(v4);
}

void window::draw_point(const vec4<double>& point) {
	vec4<double> transformed_point = (*view_mat * point);

    // Converts vertex from 3D space to NDC 
    if (ABS(transformed_point.z()) <= DEFAULT_Z_THRESH) 
        return;

	if (transformed_point.z() > 0)
		return;

	vec2<double> screen = cartesian_to_screen_coords(transformed_point);

    this->draw_point(screen);
}

void window::draw_point(const vec2<double>& point, color& c) {
    this->set_render_color(c);
    this->draw_point(point);
}

void window::draw_point(const vec3<double>& point, color& c) {
    this->set_render_color(c);
    this->draw_point(point);
}

void window::draw_point(const vec4<double>& point, color& c) {
    this->set_render_color(c);
    this->draw_point(point);
}

void window::draw_line(const vec2<double>& p1, 
                       const vec2<double>& p2) {
    SDL_RenderDrawLine(this->r, p1.x(), p1.y(), p2.x(), p2.y());
}

void window::draw_line(const vec3<double>& p1, 
                       const vec3<double>& p2) {
	vec4<double> v1 = vec4(p1, 1.0), 
				 v2 = vec4(p2, 1.0);

    this->draw_line(v1, v2);
}

void window::draw_line(const vec4<double>& p1,
                       const vec4<double>& p2) {
	vec4<double> t1 = (*view_mat * p1),	
			     t2 = (*view_mat * p2);

	if (ABS(t1.z()) <= DEFAULT_Z_THRESH || ABS(t2.z()) <= DEFAULT_Z_THRESH) 
        return;

	if (t1.z() > 0 || t2.z() > 0)
		return;

	vec2<double> first = cartesian_to_screen_coords(t1),
				 second = cartesian_to_screen_coords(t2);

    this->draw_line(first, second);
}

void window::draw_line(const vec2<double>& p1,
                       const vec2<double>& p2,
                       color& c) {
    this->set_render_color(c);
    this->draw_line(p1, p2);
}

void window::draw_line(const vec3<double>& p1,
                       const vec3<double>& p2,
                       color& c) {
    this->set_render_color(c);
    this->draw_line(p1, p2);
}

void window::draw_line(const vec4<double>& p1, 
                       const vec4<double>& p2,
                       color& c) {
    this->set_render_color(c);
    this->draw_line(p1, p2);
}

// Bresenham's Circle Drawing Algorithm
void window::draw_wireframe_circle(const vec2<double>& center,
                                   const double radius) {
    int64_t D = 3 - (2 * radius);
    int64_t x = 0, y = radius;

    do {
		vec2<double> v1 = center + vec2(x,y),
					 v2 = center + vec2(-x, y),
					 v3 = center + vec2(x, -y),
					 v4 = center + vec2(-x, -y),
					 v5 = center + vec2(y, x),
					 v6 = center + vec2(-y, x),
					 v7 = center + vec2(y, -x),
					 v8 = center + vec2(-y, -x);

        this->draw_point(v1); 
        this->draw_point(v2);
        this->draw_point(v3);
        this->draw_point(v4);
        this->draw_point(v5);
        this->draw_point(v6);
        this->draw_point(v7);
        this->draw_point(v8);  

        if (D <= 0) {
            D += 4 * x + 6;
        } else {
            D += 4 * (x-y) + 10;
            --y;
        }

        ++x;
    } while (x < y);
}

void window::draw_wireframe_circle(const vec3<double>& center,
                                   const double radius) {
	vec4<double> v4 = vec4(center, 1.0);

    this->draw_wireframe_circle(v4, radius);
}

void window::draw_wireframe_circle(const vec4<double>& center,
                                   const double radius) {
	vec4<double> tc = (*view_mat * center);

    if (ABS(tc.z()) <= DEFAULT_Z_THRESH) 
		return;

	if (tc.z() > 0)
		return;

	vec4<double> top = tc + vec4<double>(0.0, radius, 0.0, 0.0);

    vec2<double> screen_center = cartesian_to_screen_coords(tc),
                 screen_top = cartesian_to_screen_coords(top);

    this->draw_wireframe_circle(screen_center, (screen_top.y() - screen_center.y()));
}

void window::draw_wireframe_circle(const vec2<double>& center,
                                   const double radius,
                                   color& c) {
    this->set_render_color(c);
    this->draw_wireframe_circle(center, radius);
}

void window::draw_wireframe_circle(const vec3<double>& center,
                                   const double radius,
                                   color& c) {
    this->set_render_color(c);
    this->draw_wireframe_circle(center, radius);
}

void window::draw_wireframe_circle(const vec4<double>& center,
                                   const double radius,
                                   color& c) {
    this->set_render_color(c);
    this->draw_wireframe_circle(center, radius);
}

// Brensenham's Circle Drawing Algorithm (Again)
void window::draw_filled_circle(const vec2<double>& center, 
                                const double radius) {

    int64_t D = 3 - (2 * radius);
    int64_t x = 0, y = radius;

    do {
		vec2<double> v1 = center + vec2(x,y),
					 v2 = center + vec2(x,-y),
					 v3 = center + vec2(y,x),
					 v4 = center + vec2(y,-x),
					 v5 = center - vec2(x,y),
					 v6 = center - vec2(x,-y),
					 v7 = center - vec2(y,x),
					 v8 = center - vec2(y,-x);

        this->draw_line(v1, v2);
        this->draw_line(v3, v4);
        this->draw_line(v5, v6);
        this->draw_line(v7, v8);

        if (D <= 0) {
            D += 4 * x + 6;
        } else {
            D += 4 * (x-y) + 10;
            --y;
        }

        ++x;
    } while (x < y);
}

void window::draw_filled_circle(const vec3<double>& center,
                                const double radius) {
	vec4<double> v4 = vec4(center, 1.0);

    this->draw_filled_circle(v4, radius);
}

void window::draw_filled_circle(const vec4<double>& center,
                                const double radius) {
	vec4<double> tc = (*view_mat * center);

    if (ABS(tc.z()) <= DEFAULT_Z_THRESH)
        return;

	if (tc.z() > 0)
		return;

    vec4<double> top = tc + vec4<double>(0.0, radius, 0.0, 0.0);

    vec2<double> screen_center = cartesian_to_screen_coords(tc),
                 screen_top = cartesian_to_screen_coords(top);

    this->draw_wireframe_circle(screen_center, (screen_top.y() - screen_center.y()));
}

void window::draw_filled_circle(const vec2<double>& center,
                                const double radius,
                                color& c) { 
    this->set_render_color(c);
    this->draw_filled_circle(center, radius);
}

void window::draw_filled_circle(const vec3<double>& center,
                                const double radius,
                                color& c) { 
    this->set_render_color(c);
    this->draw_filled_circle(center, radius);
}

void window::draw_filled_circle(const vec4<double>& center,
                                const double radius,
                                color& c) { 
    this->set_render_color(c);
    this->draw_filled_circle(center, radius);
}

void window::draw_wireframe_triangle(const vec2<double>& v1,
                                     const vec2<double>& v2,
                                     const vec2<double>& v3) {
    this->draw_line(v1, v2);
    this->draw_line(v1, v3);
    this->draw_line(v2, v3);
}

void window::draw_wireframe_triangle(const vec3<double>& v1,
                                     const vec3<double>& v2,
                                     const vec3<double>& v3) {
	vec4<double> t1 = vec4(v1, 1.0),
				 t2 = vec4(v2, 1.0),
				 t3 = vec4(v3, 1.0);

    this->draw_wireframe_triangle(t1,
								  t2, 
								  t3);
}

void window::draw_wireframe_triangle(const vec4<double>& v1,
                                     const vec4<double>& v2,
                                     const vec4<double>& v3) {
	vec4<double> t1 = (*view_mat * v1),
				 t2 = (*view_mat * v2),
				 t3 = (*view_mat * v3);

	if (ABS(t1.z()) <= DEFAULT_Z_THRESH || ABS(t2.z()) <= DEFAULT_Z_THRESH || ABS(t3.z()) <= DEFAULT_Z_THRESH)
		return;

	if (t1.z() > 0 || t2.z() > 0 || t3.z() > 0)
		return;

    vec2<double> v1_screen = cartesian_to_screen_coords(t1),
                 v2_screen = cartesian_to_screen_coords(t2),     
                 v3_screen = cartesian_to_screen_coords(t3);

    this->draw_wireframe_triangle(v1_screen, v2_screen, v3_screen);
}

void window::draw_wireframe_triangle(const triangle& T) {
	vec3<double> v1 = T.v1(),	
				 v2 = T.v2(),
				 v3 = T.v3();

    this->draw_wireframe_triangle(v1, v2, v3);
}

void window::draw_wireframe_triangle(const vec2<double>& v1,
                                     const vec2<double>& v2,
                                     const vec2<double>& v3,
                                     color& c) {
    this->set_render_color(c);
    this->draw_wireframe_triangle(v1, v2, v3);
}


void window::draw_wireframe_triangle(const vec3<double>& v1,
                                     const vec3<double>& v2,
                                     const vec3<double>& v3,
                                     color& c) {
    this->set_render_color(c);
    this->draw_wireframe_triangle(v1, v2, v3);
}

void window::draw_wireframe_triangle(const vec4<double>& v1,
                                     const vec4<double>& v2,
                                     const vec4<double>& v3,
                                     color& c) {
    this->set_render_color(c);
    this->draw_wireframe_triangle(v1, v2, v3);
}

void window::draw_wireframe_triangle(const triangle& T,
                                     color& c) {
    this->set_render_color(c);
    this->draw_wireframe_triangle(T);
}

void window::draw_filled_triangle(vec2<double>& v1,
                                  vec2<double>& v2,
                                  vec2<double>& v3) {
    // Sort v1, v2, v3 by ascending y value
    if (v1.y() > v2.y()) 
        std::swap(v1, v2);

    if (v2.y() > v3.y())
        std::swap(v2, v3);

    if (v1.y() > v2.y())
        std::swap(v1, v2);

    double bounds = v2.y();

    for (int64_t y = v1.y(); y <= v3.y(); y++) {
        double x1 = (y <= bounds ? 
                     interp<double>(v1.x(), v1.y(), v2.x(), v2.y(), y) : 
                     interp<double>(v2.x(), v2.y(), v3.x(), v3.y(), y));

        double x2 = interp<double>(v1.x(), v1.y(), v3.x(), v3.y(), y);

		vec2<double> p1 = vec2<double>(x1, y),
					 p2 = vec2<double>(x2, y);

        this->draw_line(p1, p2);
    }
}

void window::draw_filled_triangle(vec3<double>& v1,
                                  vec3<double>& v2,
                                  vec3<double>& v3) {
	vec4<double> t1 = vec4(v1, 1.0),
			     t2 = vec4(v2, 1.0),
				 t3 = vec4(v3, 1.0);

    this->draw_filled_triangle(t1, t2, t3);
}

void window::draw_filled_triangle(vec4<double>& v1,
                                  vec4<double>& v2,
                                  vec4<double>& v3) {
	vec4<double> t1 = (*view_mat * v1),
				 t2 = (*view_mat * v2),
				 t3 = (*view_mat * v3);

	if (ABS(t1.z()) <= DEFAULT_Z_THRESH || ABS(t2.z()) <= DEFAULT_Z_THRESH || ABS(t3.z()) <= DEFAULT_Z_THRESH)
		return;

	if (t1.z() > 0 || t2.z() > 0 || t3.z() > 0)
		return;

    vec2<double> c1 = cartesian_to_screen_coords(t1),
                 c2 = cartesian_to_screen_coords(t2),
                 c3 = cartesian_to_screen_coords(t3);

    return this->draw_filled_triangle(c1, c2, c3);
}

void window::draw_filled_triangle(const triangle& T) {
	vec3<double> v1 = T.v1(),	
				 v2 = T.v2(),
				 v3 = T.v3();

    this->draw_filled_triangle(v1, v2, v3);
}

void window::draw_filled_triangle(vec2<double>& v1,
                                  vec2<double>& v2,
                                  vec2<double>& v3,
                                  color& c) {
    this->set_render_color(c);
    this->draw_filled_triangle(v1, v2, v3);
}

void window::draw_filled_triangle(vec3<double>& v1,
                                  vec3<double>& v2,
                                  vec3<double>& v3,
                                  color& c) {
    this->set_render_color(c);
    this->draw_filled_triangle(v1, v2, v3);
}

void window::draw_filled_triangle(vec4<double>& v1,
                                  vec4<double>& v2,
                                  vec4<double>& v3,
                                  color& c) {
    this->set_render_color(c);
    this->draw_filled_triangle(v1, v2, v3);
}

void window::draw_filled_triangle(const triangle& T,
                                  color& c) {
    this->set_render_color(c);
    this->draw_filled_triangle(T);
}

void window::draw_shaded_triangle(vec3<double> v1,
								  vec3<double> v2,
								  vec3<double> v3) {
	color c = *(this->current_color);
	this->draw_shaded_triangle(v1, v2, v3, c);
}

void window::draw_shaded_triangle(vec3<double> v1,
								  vec3<double> v2,
								  vec3<double> v3,
								  color &c) {
	if (compare::counter_clockwise(v1, v2)) 
		std::swap(v1, v2);

	if (compare::counter_clockwise(v1, v3))
		std::swap(v1, v3);

	if (compare::counter_clockwise(v2, v3))
		std::swap(v2, v3);

	vec3 N = ((v2 - v1).cross(v3 - v1)).normalize();

	color diffuse = light::diffuse(l->norm_pos(), N, c);

	this->set_render_color(diffuse, false);
	this->draw_filled_triangle(v1, v2, v3);
}

void window::draw_shaded_triangle(const triangle& T) {
	this->draw_shaded_triangle(T.v1(), T.v2(), T.v3());
}

void window::draw_shaded_triangle(const triangle& T,
								  color &c) {
	this->draw_shaded_triangle(T.v1(), T.v2(), T.v3(), c);
}

void window::draw_wireframe_rectangle(const vec2<double> &top_left,
									  const double length,
									  const double width) {
	// v1: top_left + (length, 0)
	// v2: top_left + (0, width)
	// v3: top_left + (length, width)
	vec2<double> v1 = top_left + vec2(length, 0.0),
				 v2 = top_left + vec2(0.0, width),
				 v3 = top_left + vec2(length, width);

	this->draw_line(top_left, v1);
	this->draw_line(top_left, v2);
	this->draw_line(v1, v3);
	this->draw_line(v2, v3);
}

void window::draw_wireframe_rectangle(const vec3<double> &top_left,
									  const double length,
									  const double width) {
	vec4<double> v4 = vec4(top_left, 1.0);
	this->draw_wireframe_rectangle(v4, length, width);
}

void window::draw_wireframe_rectangle(const vec4<double> &top_left,
									  const double length, 
									  const double width) {
	vec4<double> tc = (*view_mat * top_left);

	if (ABS(tc.z()) <= DEFAULT_Z_THRESH || tc.z() > 0)
		return;

	vec4<double> right = tc + vec4(length, 0.0, 0.0, 0.0),
			     bottom = tc + vec4(0.0, width, 0.0, 0.0);

	vec2<double> screen_left = cartesian_to_screen_coords(tc),
				 screen_right = cartesian_to_screen_coords(right),
				 screen_bottom = cartesian_to_screen_coords(bottom);

	this->draw_wireframe_rectangle(screen_left, 
								   screen_right.x() - screen_left.x(),
								   screen_bottom.y() - screen_left.y());
}

void window::draw_wireframe_rectangle(const vec2<double> &top_left,
									  const double length, 
									  const double width,
									  color &c) {
	this->set_render_color(c);
	this->draw_wireframe_rectangle(top_left, length, width);
}

void window::draw_wireframe_rectangle(const vec3<double> &top_left,
									  const double length,
									  const double width,
									  color &c) {
	this->set_render_color(c);
	this->draw_wireframe_rectangle(top_left, length, width);
}

void window::draw_wireframe_rectangle(const vec4<double> &top_left,
									  const double length,
									  const double width,
									  color &c) {
	this->set_render_color(c);
	this->draw_wireframe_rectangle(top_left, length, width);
}

void window::draw_wireframe_square(const vec2<double> &top_left,
								   const double size) {
	this->draw_wireframe_rectangle(top_left, size, size);
}

void window::draw_wireframe_square(const vec3<double> &top_left,
								   const double size) {
	this->draw_wireframe_rectangle(top_left, size, size);
}

void window::draw_wireframe_square(const vec4<double> &top_left,
								   const double size) {
	this->draw_wireframe_rectangle(top_left, size, size);
}

void window::draw_wireframe_square(const vec2<double> &top_left,
								   const double size,
								   color &c) {
	this->set_render_color(c);
	this->draw_wireframe_square(top_left, size);
}

void window::draw_wireframe_square(const vec3<double> &top_left,
								   const double size,
								   color &c) {
	this->set_render_color(c);
	this->draw_wireframe_square(top_left, size);
}

void window::draw_wireframe_square(const vec4<double> &top_left,
								   const double size,
								   color &c) {
	this->set_render_color(c);
	this->draw_wireframe_square(top_left, size);
}

void window::draw_filled_rectangle(const vec2<double> &top_left,
								   const double length,
								   const double width) {
	for (int64_t k = 0; k <= width; k++) {
		vec2<double> left = top_left + vec2(0.0, (double) k),
					 right = top_left + vec2(length, (double) k);

		this->draw_line(left, right);
	}
}

void window::draw_filled_rectangle(const vec3<double> &top_left,
								   const double length,
								   const double width) {
	vec4<double> v4 = vec4(top_left, 1.0);
	this->draw_filled_rectangle(v4, length, width);
}

void window::draw_filled_rectangle(const vec4<double> &top_left,
								   const double length,
								   const double width) {
	vec4<double> tc = (*view_mat * top_left);

	if (ABS(tc.z()) <= DEFAULT_Z_THRESH || tc.z() > 0)
		return;

	vec4<double> right = tc + vec4(length, 0.0, 0.0, 0.0),
			     bottom = tc + vec4(0.0, width, 0.0, 0.0);

	vec2<double> screen_left = cartesian_to_screen_coords(tc),
				 screen_right = cartesian_to_screen_coords(right),
				 screen_bottom = cartesian_to_screen_coords(bottom);


	this->draw_filled_rectangle(screen_left, 
								screen_right.x() - screen_left.x(),
								screen_bottom.y() - screen_left.y());
}

void window::draw_filled_rectangle(const vec2<double> &top_left,
	            				   const double length, 
	            				   const double width,
	            				   color &c) {
	this->set_render_color(c);
	this->draw_filled_rectangle(top_left, length, width);
}

void window::draw_filled_rectangle(const vec3<double> &top_left,
	            				   const double length,
	            				   const double width,
	            				   color &c) {
	this->set_render_color(c);
	this->draw_filled_rectangle(top_left, length, width);
}

void window::draw_filled_rectangle(const vec4<double> &top_left,
						           const double length,
						           const double width,
						           color &c) {
	this->set_render_color(c);
	this->draw_filled_rectangle(top_left, length, width);
}

void window::draw_filled_square(const vec2<double> &top_left,
								const double size) {
	this->draw_filled_rectangle(top_left, size, size);
}

void window::draw_filled_square(const vec3<double> &top_left,
								const double size) {
	this->draw_filled_rectangle(top_left, size, size);
}

void window::draw_filled_square(const vec4<double> &top_left,
								const double size) {
	this->draw_filled_rectangle(top_left, size, size);
}

void window::draw_filled_square(const vec2<double> &top_left,
								const double size,
								color &c) {
	this->set_render_color(c);
	this->draw_filled_square(top_left, size);
}

void window::draw_filled_square(const vec3<double> &top_left,
								const double size,
								color &c) {
	this->set_render_color(c);
	this->draw_filled_square(top_left, size);
}

void window::draw_filled_square(const vec4<double> &top_left,
								const double size,
								color &c) {
	this->set_render_color(c);
	this->draw_filled_square(top_left, size);
}

void window::run() {
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;

            if (event.type == SDL_KEYDOWN) {
				int64_t k = event.key.keysym.sym;

				if (k >= 1073741903 && k <= 1073741906) {
					int64_t N = k - 1073741903;
					bool M = ((N+1) % 2);
					vec3<double> tv = vec3<double>((N%2 == N) ? (M ? -1 : 1) : 0, (N%2 == N) ? 0 : (M ? -1 : 1), 0);

					tv *= CAMERA_SPEED;
					cam->translate(tv);
					view_mat = new mat4<double>(cam->camera_view());
				}

                switch (k) {
                    case (SDLK_SPACE): 
                        paused = 1-paused;
                        break;
                    default: 
                        break; 
                }
            }


			if (event.type == SDL_MOUSEWHEEL) {
				// inwards zoom
				vec3<double> tv = vec3<double>(0, 0, (event.wheel.y > 0 ? -CAMERA_SPEED : event.wheel.y < 0 ? CAMERA_SPEED : 0));
				cam->translate(tv);
				view_mat = new mat4<double>(cam->camera_view());
			}
        }

        if (!paused && modified) {
            this->draw();
            ++this->global_time;
        }
    }
}

// DDA Algorithm
void window::draw_colored_line(vec2<double>& v1,
                               vec2<double>& v2,
                               color& c1, 
                               color& c2) {

    if (v1.x() > v2.x())
        std::swap(v1, v2);

    double dx = (v2.x() - v1.x()),
           dy = (v2.y() - v1.y()),
           x = v1.x(),
           y = MIN(v1.y(), v2.y()),
           k = 0,
           step = MAX(ABS(dx), ABS(dy));

    dx /= step;
    dy /= step;

    while (k <= step) {
        double P = k / step;

        color point_color = interpolate_color(c1, c2, P);

		vec2<double> point = vec2(x,y);
    
        this->draw_point(point, point_color);

        x += dx;
        y += dy;
        ++k;
    }
}

void window::draw_rainbow_triangle(vec2<double>& v1,
                                   vec2<double>& v2,
                                   vec2<double>& v3) {
    if (v1.y() > v2.y())
        std::swap(v1, v2);

    if (v2.y() > v3.y())
        std::swap(v2, v3);

    if (v1.y() > v2.y())
        std::swap(v1, v2);

    double bounds = v2.y();

	vec3<double> c1 = vec3(v1, 1.0),
				 c2 = vec3(v2, 1.0),
				 c3 = vec3(v3, 1.0);

    for (int64_t y = v1.y(); y <= v3.y(); y++) {
        double x1 = (y <= bounds ? 
                     interp<double>(v1.x(), v1.y(), v2.x(), v2.y(), y) : 
                     interp<double>(v2.x(), v2.y(), v3.x(), v3.y(), y));

        double x2 = interp<double>(v1.x(), v1.y(), v3.x(), v3.y(), y);

		if (std::isinf(x1) || std::isinf(x2))
			continue;

        for (int64_t x = MIN(x1, x2); x <= MAX(x1, x2); x++) {
            vec2<double> point = vec2<double>(x, y);

            vec3<double> bary = barycentric(c1,
                                            c2,
                                            c3,
                                            vec3(point, 1.0));

            color c(0xFF * bary[0], 0xFF * bary[1], 0xFF * bary[2]);

            this->draw_point(point, c);
        }
    }
}

void window::draw_rainbow_triangle(const vec3<double>& v1,
                                   const vec3<double>& v2,
                                   const vec3<double>& v3) {
	vec4<double> t1 = vec4(v1, 1.0),
			     t2 = vec4(v2, 1.0),
				 t3 = vec4(v3, 1.0);

    this->draw_rainbow_triangle(t1, t2, t3);
}

void window::draw_rainbow_triangle(const vec4<double>& v1,
                                   const vec4<double>& v2,
                                   const vec4<double>& v3) {
	vec4<double> t1 = (*view_mat * v1),
			     t2 = (*view_mat * v2),
				 t3 = (*view_mat * v3);

	if (ABS(t1.z()) <= DEFAULT_Z_THRESH || ABS(t2.z()) <= DEFAULT_Z_THRESH || ABS(t3.z()) <= DEFAULT_Z_THRESH)
		return;

	if (t1.z() > 0 || t2.z() > 0 || t3.z() > 0)
		return;

    vec2<double> c1 = cartesian_to_screen_coords(t1),
                 c2 = cartesian_to_screen_coords(t2),
                 c3 = cartesian_to_screen_coords(t3);

    return this->draw_rainbow_triangle(c1, c2, c3);
}

void window::draw_wireframe_polygon(polygon& p) {
	list<triangle> triangles = p.triangulated();

	for (int64_t k = 0; k < triangles.size(); k++) {
		this->draw_wireframe_triangle(triangles[k]);
	}
}

void window::draw_filled_polygon(polygon& p) {
	list<triangle> triangles = p.triangulated();

	for (int64_t k = 0; k < triangles.size(); k++) {
		this->draw_filled_triangle(triangles[k]);
	}
}

void window::draw_wireframe_polygon(polygon& p,
									color& c) {
	this->set_render_color(c);
	this->draw_wireframe_polygon(p);
}

void window::draw_filled_polygon(polygon& p,
							 	 color& c) {
	this->set_render_color(c);
	this->draw_filled_polygon(p);
}

template <int64_t N>
void window::draw_bezier_curve(const bezier<N>& b, 
							   double intv) {
	for (double t = 0.0; t <= 1.0; t += intv) {
		vec3<double> p = b(t);

		this->draw_point(p);
	}
}

template <int64_t N>
void window::draw_bezier_curve(const bezier<N>& b, 
							   color& c, 
							   double intv) {
	this->set_render_color(c);

	this->draw_bezier_curve<N>(b, intv);
}

void window::draw_convex_hull(list<vec2<double>> &points,
							  color norm,
							  color highlight) {
	list<vec2<double>> hull = convex_hull::divide_conquer(points);

	int64_t N = points.size(), M = hull.size();

	// Draws the entire set of points.
	for (int64_t k = 0; k < N; k++) {
		vec2<double> P = points[k];
		this->draw_filled_circle(P, 2, norm);
	}

	// Draw convex hull.
	for (int64_t k = 0; k < M; k++) {
		vec2<double> p0 = hull[k],
					 p1 = hull[mod(k+1, M)];
		this->draw_filled_circle(p0, 4, highlight);
		this->draw_line(p0, p1, highlight);
	}
}

void window::draw_convex_hull(list<vec3<double>> &points,
							  color norm,
							  color highlight) {
	list<triangle> hull = convex_hull::brute_force(points);

	quicksort(hull, &compare::tz);

	int64_t M = hull.size();

	linked_node<triangle> *node = hull.front();

	for (int64_t k = 0; k < M; k++) {
		this->draw_shaded_triangle(node->value(), highlight);

		node = node->next();
	}
}

void window::draw() {
	this->fill_background(color::BLACK());

	mat3<double> mat = create_3d_rotation_matrix(0.0, global_time, 15.0);

	color R = color::RED(), B = color::BLUE();

	list<vec3<double>> golden = polygon::golden_icosahedron(), points;

	linked_node<vec3<double>> *node = golden.front();

	for (int64_t k = 0; k < golden.size(); k++) {
		vec3 P = node->value();
		points.push_back(mat * P);
		node = node->next();
	}

	this->draw_convex_hull(points);

	SDL_RenderPresent(this->r);
	SDL_Delay(this->delay);
}
