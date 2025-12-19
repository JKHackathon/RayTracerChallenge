#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include "color.hpp"
#include "tuples.hpp"

struct PointLight {
    Point pos;
    Color intensity;

    PointLight(Point position, Color intensity)
        : pos(position), intensity(intensity) {}
};

#endif