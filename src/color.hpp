#ifndef COLOR_HPP
#define COLOR_HPP

#pragma once
#include "vec.hpp"

#include <iostream>
#include <stdint.h>

class color {
    private:
        vec4<uint8_t> v4;
    public:
        color();

        color(uint8_t v);

        color(uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255);

        uint8_t R() const;

        uint8_t G() const;

        uint8_t B() const;

        uint8_t A() const;

        void R(uint8_t new_red);

        void G(uint8_t new_green);

        void B(uint8_t new_blue);

        void A(uint8_t new_alpha);

        vec4<uint8_t> get_v4() const;

        static color RED();

        static color BLUE();

        static color GREEN();
};

std::ostream& operator<<(std::ostream& out, const color& c);

#endif
