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
    // YEP THERE ARE: why do i not expect this to always be normalized, is it because it is sometimes transformed to a diff space?
    Ray(Point origin, Vector dir) : origin(origin), dir(dir) {}

    Point position(double t) { return origin + dir * t; }

    Ray transform(Transform m) const { return Ray(m * origin, m * dir); }
};