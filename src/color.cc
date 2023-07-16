#include "color.h"

color::color() : v4(vector<uint8_t>::zero(4)) {
    v4[3] = 0xFF;
}

color::color(uint8_t v) : v4(vector<uint8_t >::zero(4)) {
    this->v4.fill(v);
}

color::color(uint8_t R, 
             uint8_t G, 
             uint8_t B, 
             uint8_t A) {
    this->v4 = vector<uint8_t>(new uint8_t[]{R, G, B, A}, 4);
}

uint8_t color::R() const {
    return this->v4[0];
}

uint8_t color::G() const {
    return this->v4[1];
}

uint8_t color::B() const {
    return this->v4[2];
}

uint8_t color::A() const {
    return this->v4[3];
}

void color::R(uint8_t new_r) {
    this->v4[0] = new_r;
}

void color::G(uint8_t new_g) {
    this->v4[1] = new_g;
}

void color::B(uint8_t new_b) {
    this->v4[2] = new_b;
}

void color::A(uint8_t new_a) {
    this->v4[3] = new_a;
}


vector<uint8_t>& color::get_color() {
    return this->v4;
}

std::ostream& operator<<(std::ostream& out, const color& c) {
    out << "(" << static_cast<int64_t>(c.R()) << "," << 
                  static_cast<int64_t>(c.G()) << "," << 
                  static_cast<int64_t>(c.B()) << "," << 
                  static_cast<int64_t>(c.A()) << ")";
    return out;
}
