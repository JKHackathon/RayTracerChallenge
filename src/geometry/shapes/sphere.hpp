#pragma once

#include "shapes.hpp"

struct Sphere : public Shape {
public:
    Point origin;
    double radius;
    // Transform transform;
    // Material material;

    Sphere() : origin(Point(0, 0, 0)), radius(1) {}
    Sphere(Transform transform, Material material)
        : Shape(transform, material), origin(Point(0, 0, 0)), radius(1) {
    }

private:
    Vector local_normal_at(const Point local_p) const override;

    // TODO: look into math of it
    IntersectionRecord local_intersect(const Ray local_r) const override;
};

struct GlassSphere : public Sphere {
    GlassSphere() {
        material.transparency = 1;
        material.refractive_index = 1.5;
    }
};