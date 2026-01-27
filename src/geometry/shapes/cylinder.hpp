#pragma once

#include "shapes.hpp"

struct Cylinder : public Shape {
    double minimum;
    double maximum;
    bool closed;

    Cylinder() : minimum(MIN_DOUBLE), maximum(MAX_DOUBLE), closed(false) {}

    BoundingBox bounds_of() const override {
        return BoundingBox(Point(-1, minimum, -1), Point(1, maximum, 1));
    }

private:
    Vector local_normal_at(const Point local_p, Intersection i) const override;
    IntersectionRecord local_intersect(const Ray local_r) const override;

    bool check_cap(Ray r, double t) const;
    void intersect_caps(Ray r, IntersectionRecord& xs) const;
};