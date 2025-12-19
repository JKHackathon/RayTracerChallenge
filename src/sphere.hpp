#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "transformations.hpp"
#include "tuples.hpp"
#include "materials.hpp"

struct Sphere {
    Point origin;
    float radius;
    Transform transform;
    Material material;

    Sphere() : origin(Point(0, 0, 0)), radius(1) {}

    void set_transform(Transform t) { transform = t; }

    Vector normal_at(Point p) const {
        Vector n_o = transform.inverse() * p - origin;
        auto n_w = transform.inverse().transpose() * n_o;
        n_w.w = 0;
        return Vector(n_w).normalized();
    }
};

#endif