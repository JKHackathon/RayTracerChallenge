#pragma once

#include "shapes.hpp"

struct Cone : public Shape {
    double minimum;
    double maximum;
    bool closed;

    Cone() : minimum(std::numeric_limits<double>::lowest()), maximum(std::numeric_limits<double>::max()), closed(false) {}

private:
    Vector local_normal_at(const Point local_p, Intersection i) const override;
    IntersectionRecord local_intersect(const Ray local_r) const override;

    bool check_cap(Ray r, double t, double plane_y) const;
    void intersect_caps(Ray r, IntersectionRecord& xs) const;
};