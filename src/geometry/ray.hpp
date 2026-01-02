#pragma once

#include <cassert>
#include <iostream>
#include <vector>

#include "../math/transformations.hpp"

struct Ray {
    Point origin;
    Vector dir;

    // TODO: unclear if within textbook structure
    // std::vector<Intersection> intersections;

    Ray(Point origin, Vector dir) : origin(origin), dir(dir) {}

    Point position(double t) { return origin + dir * t; }

    Ray transform(Transform m) const { return Ray(m * origin, m * dir); }
};