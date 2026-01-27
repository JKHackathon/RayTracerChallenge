#pragma once

#include "shapes.hpp"

struct Cube : public Shape {
    BoundingBox bounds_of() const override {
        return BoundingBox(Point(-1, -1, -1), Point(1, 1, 1));
    }
private:
    Vector local_normal_at(const Point local_p, Intersection i) const override;
    IntersectionRecord local_intersect(const Ray local_r) const override;
    std::pair<double, double> check_axis(double origin, double direction) const;
};