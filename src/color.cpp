#include "color.hpp"

color::color() {
    v4 = new vec4<uint8_t>(0, 0, 0, 0xFF);
}

color::color(uint8_t v) {
    v4 = new vec4<uint8_t>(v, v, v, 0xFF);
}

color::color(uint8_t R, uint8_t G, uint8_t B, uint8_t A) {
    v4 = new vec4<uint8_t>(R, G, B, A);
}

uint8_t color::R() {
    return (*v4)[0];
}

uint8_t color::G() {
    return (*v4)[1];
}

uint8_t color::B() {
    return (*v4)[2];
}

uint8_t color::A() {
    return (*v4)[3];
}

void color::R(uint8_t new_red) {
    (*v4)[0] = new_red;
}

void color::G(uint8_t new_green) {
    (*v4)[1] = new_green;
}

void color::B(uint8_t new_blue) {
    (*v4)[2] = new_blue;
}

void color::A(uint8_t new_alpha) {
    (*v4)[3] = new_alpha;
}

vec4<uint8_t> color::get_v4() const {
    return *v4;
}

color color::RED() {
    return color(0xFF, 0, 0);
}

color color::GREEN() {
    return color(0, 0xFF, 0);
}

color color::BLUE() {
    return color(0, 0, 0xFF);
}

color color::BLACK() {
    return color(0, 0, 0);
}

color color::WHITE() {
    return color(0xFF, 0xFF, 0xFF);
}

std::ostream& operator<<(std::ostream& out, color& c) {
    out << "color: " << "(" << +c.R() << "," << +c.G() << "," << +c.B() << "," << +c.A() << ")";

    return out;
}

