#ifndef COLOR_HPP
#define COLOR_HPP

#include "util.hpp"

struct Color {
    float r;
    float g;
    float b;

    Color(float r, float g, float b) : r(r), g(g), b(b) {}
    // Book says don't clamp yet as will go through transformations
    // Tuple(std::clamp(r, 0.f, 1.f), std::clamp(g, 0.f, 1.f), std::clamp(b,
    // 0.f, 1.f)) {}

    bool operator==(const Color& other) const {
        return float_equal(r, other.r) && float_equal(g, other.g) &&
               float_equal(b, other.b);
    }

    Color operator+(const Color& other) const {
        return Color(r + other.r, g + other.g, b + other.b);
    }

    Color operator-(const Color& other) const {
        return Color(r - other.r, g - other.g, b - other.b);
    }

    Color operator*(float c) const { return Color(r * c, g * c, b * c); }

    Color operator*(const Color& other) const {
        return Color(r * other.r, g * other.g, b * other.b);
    }
};

#endif