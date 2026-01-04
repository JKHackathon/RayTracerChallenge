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

    // TODO: i typically expect dir to be normalized, but maybe there are exceptions
    Ray(Point origin, Vector dir) : origin(origin), dir(dir.normalized()) {}

    Point position(double t) { return origin + dir * t; }

    Ray transform(Transform m) const { return Ray(m * origin, m * dir); }
};