#pragma once

#include "../math/color.hpp"
#include "../math/tuples.hpp"

struct PointLight {
    Point pos;
    Color intensity;

    PointLight(Point position, Color intensity)
        : pos(position), intensity(intensity) {}
};