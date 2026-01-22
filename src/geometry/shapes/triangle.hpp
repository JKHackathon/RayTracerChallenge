#pragma once

#include "shapes.hpp"

struct Triangle : public Shape {
public:
    Point p1;
    Point p2;
    Point p3;
    Vector e1;
    Vector e2;
    Vector normal;

    Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {
        e1 = p2 - p1;
        e2 = p3 - p1;
        normal = e2.cross(e1).normalized();
    }

private:
    Vector local_normal_at(const Point local_p) const override;
    IntersectionRecord local_intersect(const Ray local_r) const override;
};