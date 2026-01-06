#pragma once

#include "shapes.hpp"

struct Cube : public Shape {
private:
    Vector local_normal_at(const Point local_p) const override;
    IntersectionRecord local_intersect(const Ray local_r) const override;
    std::pair<double, double> check_axis(double origin, double direction) const;
};