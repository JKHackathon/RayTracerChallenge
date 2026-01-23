#include "cylinder.hpp"

Vector Cylinder::local_normal_at(const Point local_p, Intersection i) const {
    auto dist = pow(local_p.x, 2) + pow(local_p.z, 2);

    // EPSILON necessary to prevent issues w/ point not being exactly on cap
    if (dist < 1 && local_p.y >= maximum - EPSILON) {
        return Vector(0, 1, 0);
    }
    else if (dist < 1 && local_p.y <= minimum + EPSILON) {
        return Vector(0, -1, 0);
    }
    return Vector(local_p.x, 0, local_p.z);
}

// TODO: look into math
IntersectionRecord Cylinder::local_intersect(const Ray local_r) const {
    IntersectionRecord xs;
    intersect_caps(local_r, xs);

    double a = pow(local_r.dir.x, 2) + pow(local_r.dir.z, 2);

    if (double_equal(a, 0)) return xs; // ray parallel to y-axis

    auto b = 2 * local_r.origin.x * local_r.dir.x + 2 * local_r.origin.z * local_r.dir.z;
    auto c = pow(local_r.origin.x, 2) + pow(local_r.origin.z, 2) - 1;

    auto discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) return xs; // ray doesn't intersect edges

    auto t0 = (-b - sqrt(discriminant)) / (2 * a);
    auto t1 = (-b + sqrt(discriminant)) / (2 * a);
    if (t0 > t1) { // TODO: what's the purpose of this????
        auto temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // Intersection between min and max values
    auto y0 = local_r.origin.y + t0 * local_r.dir.y;
    if (minimum < y0 && y0 < maximum) {
        xs.append_record(Intersection(t0, this));
    }

    auto y1 = local_r.origin.y + t1 * local_r.dir.y;
    if (minimum < y1 && y1 < maximum) {
        xs.append_record(Intersection(t1, this));
    }


    return xs;
}

// checks if intersection w/ plane at `t` is within radius of 1
bool Cylinder::check_cap(Ray r, double t) const {
    auto x = r.origin.x + t * r.dir.x;
    auto z = r.origin.z + t * r.dir.z;
    return (pow(x, 2) + pow(z, 2)) <= 1;
}

void Cylinder::intersect_caps(Ray r, IntersectionRecord& xs) const {
    if (!closed || double_equal(r.dir.y, 0)) {
        return;
    }

    auto t = (minimum - r.origin.y) / r.dir.y;
    if (check_cap(r, t)) {
        xs.append_record(Intersection(t, this));
    }

    t = (maximum - r.origin.y) / r.dir.y;
    if (check_cap(r, t)) {
        xs.append_record(Intersection(t, this));
    }
}