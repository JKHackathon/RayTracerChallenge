#include "triangle.hpp"

Vector Triangle::local_normal_at(const Point local_p, Intersection i) const {
    return normal;
}

IntersectionRecord Triangle::local_intersect(const Ray local_r) const {
    auto dir_cross_e2 = local_r.dir.cross(e2);
    double determinant = e1.dot(dir_cross_e2);
    if (abs(determinant) < EPSILON) {
        return IntersectionRecord();
    }
    double f = 1 / determinant;
    Vector p1_to_origin = local_r.origin - p1;
    auto u = f * p1_to_origin.dot(dir_cross_e2);
    if (u < 0 || u > 1) { // ray misses p1-p3 edge
        return IntersectionRecord();
    }

    Vector origin_cross_e1 = p1_to_origin.cross(e1);
    auto v = f * local_r.dir.dot(origin_cross_e1);
    if (v < 0 || u + v > 1) {
        return IntersectionRecord();
    }

    auto t = f * e2.dot(origin_cross_e1);
    return Intersection(t, this, u, v);
}

// Interpolated normal
Vector SmoothTriangle::local_normal_at(const Point local_p, Intersection i) const {
    return n2 * i.u + n3 * i.v + n1 * (1 - i.u - i.v);
}

// IntersectionRecord SmoothTriangle::local_intersect(const Ray local_r) const {
//     // TODO: same as triangle local_intersect but create intersection w/ uv params
// }