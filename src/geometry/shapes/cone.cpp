#include "cone.hpp"

Vector Cone::local_normal_at(const Point local_p, Intersection i) const {
    auto dist = pow(local_p.x, 2) + pow(local_p.z, 2);

    // EPSILON necessary to prevent issues w/ point not being exactly on cap
    if (dist < pow(maximum, 2) && local_p.y >= maximum - EPSILON) {
        return Vector(0, 1, 0);
    }
    else if (dist < pow(minimum, 2) && local_p.y <= minimum + EPSILON) {
        return Vector(0, -1, 0);
    }

    double y = sqrt(pow(local_p.x, 2) + pow(local_p.z, 2));
    y = local_p.y > 0 ? -y : y;

    return Vector(local_p.x, y, local_p.z);
}

IntersectionRecord Cone::local_intersect(const Ray local_r) const {
    auto a = pow(local_r.dir.x, 2) - pow(local_r.dir.y, 2) + pow(local_r.dir.z, 2);
    auto b = 2 * local_r.origin.x * local_r.dir.x - 2 * local_r.origin.y * local_r.dir.y + 2 * local_r.origin.z * local_r.dir.z;
    auto c = pow(local_r.origin.x, 2) - pow(local_r.origin.y, 2) + pow(local_r.origin.z, 2);

    IntersectionRecord xs;
    intersect_caps(local_r, xs); // TODO: Should this go after first condition?

    if (double_equal(a, 0) && double_equal(b, 0)) { // TODO: what is this condition?
        return xs; //IntersectionRecord();
    }

    if (double_equal(a, 0)) { // parallel to one cone
        xs.append_record(Intersection(-c / (2 * b), this));
        return xs;
    }

    auto discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) return xs;

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

bool Cone::check_cap(Ray r, double t, double plane_y) const {
    auto x = r.origin.x + t * r.dir.x;
    auto z = r.origin.z + t * r.dir.z;
    return (pow(x, 2) + pow(z, 2)) <= pow(plane_y, 2);
}

void Cone::intersect_caps(Ray r, IntersectionRecord& xs) const {
    if (!closed || double_equal(r.dir.y, 0)) {
        return;
    }

    auto t = (minimum - r.origin.y) / r.dir.y;
    if (check_cap(r, t, minimum)) {
        xs.append_record(Intersection(t, this));
    }

    t = (maximum - r.origin.y) / r.dir.y;
    if (check_cap(r, t, maximum)) {
        xs.append_record(Intersection(t, this));
    }
}