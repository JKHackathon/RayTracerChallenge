#include "shapes.hpp"

// #include "intersection.hpp"

IntersectionRecord Sphere::local_intersect(const Ray local_r) const {
    // Ray obj_space_ray = r.transform(transform.inverse());

    Vector sphere_to_ray = local_r.origin - origin;
    double a = local_r.dir.dot(local_r.dir);
    double b = 2 * local_r.dir.dot(sphere_to_ray);
    double c = sphere_to_ray.dot(sphere_to_ray) - 1;
    double discriminant = pow(b, 2) - 4 * a * c;

    if (discriminant < 0) {
        return IntersectionRecord();
    }

    double t1 = (-b - sqrt(discriminant)) / (2 * a);
    double t2 = (-b + sqrt(discriminant)) / (2 * a);

    return IntersectionRecord(Intersection(t1, this), Intersection(t2, this));
}

Vector Cube::local_normal_at(const Point local_p) const {
    double maxc = std::max({ abs(local_p.x), abs(local_p.y), abs(local_p.z) });

    if (double_equal(maxc, abs(local_p.x))) {
        return Vector(local_p.x, 0, 0);
    }
    else if (double_equal(maxc, abs(local_p.y))) {
        return Vector(0, local_p.y, 0);
    }
    return Vector(0, 0, local_p.z);
}

// TODO: can optimize by not checking all if clear miss
IntersectionRecord Cube::local_intersect(const Ray local_r) const {
    auto const [xtmin, xtmax] = check_axis(local_r.origin.x, local_r.dir.x);
    auto const [ytmin, ytmax] = check_axis(local_r.origin.y, local_r.dir.y);
    auto const [ztmin, ztmax] = check_axis(local_r.origin.z, local_r.dir.z);

    double tmin = std::max({ xtmin, ytmin, ztmin });
    double tmax = std::min({ xtmax, ytmax, ztmax });

    if (tmin > tmax) { // No intersection
        return IntersectionRecord();
    }

    return IntersectionRecord(Intersection(tmin, this), Intersection(tmax, this));
}

// Identify where ray intersects planes offset by 1 from axis
std::pair<double, double> Cube::check_axis(double origin, double direction) const {
    double tmin_numerator = -1 - origin;
    double tmax_numerator = 1 - origin;

    double tmin, tmax;
    if (abs(direction) >= EPSILON) {
        tmin = tmin_numerator / direction;
        tmax = tmax_numerator / direction;
    }
    else {
        tmin = tmin_numerator * INFINITY;
        tmax = tmax_numerator * INFINITY;
    }

    if (tmin > tmax) {
        double temp = tmin;
        tmin = tmax;
        tmax = temp;
    }
    return std::make_pair(tmin, tmax);
}

Vector Cylinder::local_normal_at(const Point local_p) const {
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
    if (discriminant < 0) return IntersectionRecord(); // ray doesn't intersect

    auto t0 = (-b - sqrt(discriminant)) / (2 * a);
    auto t1 = (-b + sqrt(discriminant)) / (2 * a);
    if (t0 > t1) {
        auto temp = t0;
        t0 = t1;
        t1 = temp;
    }

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