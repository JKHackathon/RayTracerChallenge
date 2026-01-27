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

    // TODO: should this go in intersectionrecord as a factory method
    // IntersectionRecord intersect_with_uv(double t, double u, double v) const;
    BoundingBox bounds_of() const override;

private:
    Vector local_normal_at(const Point local_p, Intersection i) const override;
    IntersectionRecord local_intersect(const Ray local_r) const override;
};

struct SmoothTriangle : public Triangle { // TODO: maybe a child of Triangle
    // Point p1;
    // Point p2;
    // Point p3;
    Vector n1;
    Vector n2;
    Vector n3;

    SmoothTriangle(Point p1, Point p2, Point p3, Vector n1, Vector n2, Vector n3) :
        // p1(p1), p2(p2), p3(p3), n1(n1), n2(n2), n3(n3) {
        Triangle(p1, p2, p3), n1(n1), n2(n2), n3(n3) {
    }

private:
    Vector local_normal_at(const Point local_p, Intersection i) const override;
    // IntersectionRecord local_intersect(const Ray local_r) const override;
};