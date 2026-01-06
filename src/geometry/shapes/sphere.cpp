#include "sphere.hpp"

Vector Sphere::local_normal_at(const Point local_p) const {
    return local_p - origin;
}

// TODO: look into math of it
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