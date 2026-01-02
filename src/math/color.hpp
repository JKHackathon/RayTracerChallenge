#ifndef COLOR_HPP
#define COLOR_HPP

#include "util.hpp"

struct Color {
    double r;
    double g;
    double b;

    Color(double r, double g, double b) : r(r), g(g), b(b) {}
    // Book says don't clamp yet as will go through transformations
    // Tuple(std::clamp(r, 0.f, 1.f), std::clamp(g, 0.f, 1.f), std::clamp(b,
    // 0.f, 1.f)) {}

    bool operator==(const Color& other) const {
        return double_equal(r, other.r) && double_equal(g, other.g) &&
            double_equal(b, other.b);
    }

    Color operator+(const Color& other) const {
        return Color(r + other.r, g + other.g, b + other.b);
    }

    Color operator-(const Color& other) const {
        return Color(r - other.r, g - other.g, b - other.b);
    }

    Color operator*(double c) const { return Color(r * c, g * c, b * c); }

    Color operator*(const Color& other) const {
        return Color(r * other.r, g * other.g, b * other.b);
    }
};

#endif