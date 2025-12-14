#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "transformations.hpp"
#include "tuples.hpp"

struct Sphere {
    Point origin;
    float radius;
    Transform transform;

    Sphere() : origin(Point(0, 0, 0)), radius(1) {}

    void set_transform(Transform t) { transform = t; }
};

#endif