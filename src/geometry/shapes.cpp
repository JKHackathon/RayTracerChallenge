#include "shapes.hpp"

// #include "intersection.hpp"

IntersectionRecord Sphere::local_intersect(const Ray local_r) const {
    // Ray obj_space_ray = r.transform(transform.inverse());

    Vector sphere_to_ray = local_r.origin - origin;
    float a = local_r.dir.dot(local_r.dir);
    float b = 2 * local_r.dir.dot(sphere_to_ray);
    float c = sphere_to_ray.dot(sphere_to_ray) - 1;
    float discriminant = pow(b, 2) - 4 * a * c;

    if (discriminant < 0) {
        return IntersectionRecord();
    }

    float t1 = (-b - sqrt(discriminant)) / (2 * a);
    float t2 = (-b + sqrt(discriminant)) / (2 * a);

    return IntersectionRecord(Intersection(t1, this), Intersection(t2, this));
}