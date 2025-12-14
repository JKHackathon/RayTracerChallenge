#include "ray.hpp"

IntersectionRecords Ray::intersect(const Sphere* s) const {
    Ray obj_space_ray = this->transform(s->transform.inverse());

    Vector sphere_to_ray = obj_space_ray.origin - s->origin;
    float a = obj_space_ray.dir.dot(obj_space_ray.dir);
    float b = 2 * obj_space_ray.dir.dot(sphere_to_ray);
    float c = sphere_to_ray.dot(sphere_to_ray) - 1;
    float discriminant = pow(b, 2) - 4 * a * c;

    if (discriminant < 0) {
        return IntersectionRecords();
    }

    float t1 = (-b - sqrt(discriminant)) / (2 * a);
    float t2 = (-b + sqrt(discriminant)) / (2 * a);

    return IntersectionRecords(Intersection(t1, s), Intersection(t2, s));
}

std::optional<Intersection> IntersectionRecords::hit() const {
    float hit_val = MAXFLOAT;
    std::optional<Intersection> hit;
    for (const auto& intersection : intersections) {
        if (intersection.t >= 0 &&
            intersection.t <
                hit_val) {  // TODO: just non-negative or only positive
            hit = intersection;
            hit_val = intersection.t;
        }
    }
    return hit;
}
