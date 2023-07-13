#include "MACROS.h"
#include "rational.h"
#include <iostream>
#include <numeric>
#include <stdint.h>
#include <assert.h>

void rat::simplify() {
    uint64_t f = gcd(this->P, static_cast<int64_t>(this->Q));
    this->P /= f;
    this->Q /= f;
}

rat::rat(const int64_t p, const uint64_t q) : P(p), Q(q) {
    assert(this->Q != 0);
    this->simplify();
}

rat::rat(const rat& v): P(v.get_P()), Q(v.get_Q()) {
    assert(this->Q != 0);
    this->simplify();
}

rat rat::operator+(rat& r) const {
    return rat(
        r.get_P() * this->get_Q() + 
        r.get_Q() * this->get_P(),
        r.get_Q() * this->get_Q()
    );
}

rat rat::operator-(rat& r) const {
    return rat(
        this->get_P() * r.get_Q() -
        this->get_Q() * r.get_P(),
        this->get_Q() * r.get_Q()
    );
}

rat rat::operator/(rat& r) const {
    assert(r.get_P() != 0);
    return rat(
        this->get_P() * r.get_Q(),
        this->get_Q() * r.get_P()
    );
}

int64_t rat::operator%(rat& r) const {
    if (!(this->is_integral() && r.is_integral())) {
        return 0;
    }

    return (this->P % r.get_P());
}

rat& rat::operator++() {
    this->P += this->Q;
    return *this;
}

rat& rat::operator--() {
    this->P -= this->Q;
    return *this;
}

rat rat::operator++(int) {
    rat tmp(*this);
    this->P += this->Q;
    return tmp;
}

rat rat::operator--(int) {
    rat tmp(*this);
    this->P -= this->Q;
    return tmp;
}

int64_t rat::get_P() const {
    return this->P;
}

uint64_t rat::get_Q() const {
    return this->Q;
}

bool rat::is_integral() const {
    return (this->Q == 1);
}

double rat::value() const {
    return (static_cast<double>(this->P) / this->Q);
}

std::ostream& operator<<(std::ostream& out, const rat& r) {
    out << r.get_P() << "/" << r.get_Q();
    return out;
}