#ifndef COLOR_H
#define COLOR_H

#pragma once
#include "vector.h"
#include <stdint.h>

/** A color class which represents a color using a 4-D vector (R, G, B, A) */
class color {
    private:
        vector<uint8_t> v4; ///< The vec4 representing the color.
    public:
        /** A constructor which creates a solid black color: (0, 0, 0, 255). */
        color();

        /** A constructor which instantiates a color object based on a singular value. 
         * The color instantiated is has the (R,G,B,A) value of (v,v,v,255).
         * @param v The value of the color.
         * */
        color(uint8_t v);

        /** A constructor which instantiates a color object based on individual R, G, B, A values.
         * @param R The red component of the color object.
         * @param G The green component of the color object.
         * @param B The blue component of the color object.
         * @param A The alpha component of the color object.
         * */
        color(uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255);

        /** A getter method which returns the red component of the color.
         * @return The red component of the color.
         * */
        uint8_t R() const;

        /** A getter method which returns the green component of the color.
         * @return The green component of the color.
         * */
        uint8_t G() const;

        /** A getter method which returns the blue component of the color.
         * @return The blue component of the color.
         * */
        uint8_t B() const;

        /** A getter method which returns the alpha component of the color 
         * @return The alpha component of the color.
         * */
        uint8_t A() const;

        /** A setter method which redefines the red component of the color.
         * @param nR The new red component of the color.
         * */
        void R(uint8_t nR);

        /** A setter method which redefines the green component of the color. 
         * @param nG The new green component of the color.
         * */
        void G(uint8_t nG);

        /** A setter method which redefines the blue component of the color.
         * @param nB The new blue component of the color.
         * */
        void B(uint8_t nB);

        /** A setter method which redefines the alpha component of the color.
         * @param nA The new alpha component of the color.
         * */
        void A(uint8_t nA);

        /** A getter method which returns the vec4 used to collectively represent the (R, G, B, A) channels.
         * @return The vec4 used to represent the color.
         * */
        vector<uint8_t>& get_color();
};

std::ostream& operator<<(std::ostream& out, const color& c);

#endif
