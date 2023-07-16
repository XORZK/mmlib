#ifndef COLOR_H
#define COLOR_H

#pragma once
#include <stdint.h>
#include "vector.h"

class color {
    private:
        vector<uint8_t> v4;
    public:
        color();

        color(uint8_t v);

        color(uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255);

        uint8_t R() const;

        uint8_t G() const;

        uint8_t B() const;

        uint8_t A() const;

        void R(uint8_t new_r);

        void G(uint8_t new_g);

        void B(uint8_t new_b);

        void A(uint8_t new_a);

        vector<uint8_t>& get_color();
};

std::ostream& operator<<(std::ostream& out, const color& c);

#endif
