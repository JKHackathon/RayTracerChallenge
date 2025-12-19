#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "materials.hpp"
#include "transformations.hpp"
#include "tuples.hpp"

struct Sphere {
    Point origin;
    float radius;
    Transform transform;
    Material material;

    Sphere() : origin(Point(0, 0, 0)), radius(1) {}
    Sphere(Transform transform, Material material)
        : origin(Point(0, 0, 0)),
          transform(transform),
          material(material),
          radius(1) {}

    Vector normal_at(Point p) const {
        Vector n_o = transform.inverse() * p - origin;
        auto n_w = transform.inverse().transpose() * n_o;
        n_w.w = 0;
        return Vector(n_w).normalized();
    }
};

#endif