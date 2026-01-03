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