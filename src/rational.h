#ifndef RATIONAL_H
#define RATIONAL_H

#pragma once
#include <iostream>
#include <stdint.h>

class rat {
    private:
        int64_t P;
        uint64_t Q;

        void simplify();
    public:
        rat(const int64_t p = 0, const uint64_t q = 1);

        rat(const rat& v);

        rat operator+(rat& r) const;

        rat operator-(rat& r) const;

        rat operator*(rat& r) const;

        rat operator/(rat& r) const;

        int64_t operator%(rat& r) const;

        rat& operator++();

        rat& operator--(); 

        rat operator++(int);

        rat operator--(int);

        template <typename U>
        operator U() const;

        bool operator==(const rat& r) const;

        int64_t get_P() const;

        uint64_t get_Q() const;

        double value() const;

        bool is_integral() const;
};

std::ostream& operator<<(std::ostream& out, const rat& r);

template <typename U>
rat::operator U() const {
    return static_cast<U>(this->value());
}

#endif
