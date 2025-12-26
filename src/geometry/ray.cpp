#include "ray.hpp"

#include <algorithm>

IntersectionRecord Ray::intersect(const Sphere* s) const {
    Ray obj_space_ray = this->transform(s->transform.inverse());

    Vector sphere_to_ray = obj_space_ray.origin - s->origin;
    float a = obj_space_ray.dir.dot(obj_space_ray.dir);
    float b = 2 * obj_space_ray.dir.dot(sphere_to_ray);
    float c = sphere_to_ray.dot(sphere_to_ray) - 1;
    float discriminant = pow(b, 2) - 4 * a * c;

    if (discriminant < 0) {
        return IntersectionRecord();
    }

    float t1 = (-b - sqrt(discriminant)) / (2 * a);
    float t2 = (-b + sqrt(discriminant)) / (2 * a);

    return IntersectionRecord(Intersection(t1, s), Intersection(t2, s));
}

std::optional<Intersection> IntersectionRecord::hit() const {
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

IntersectionRecord Ray::intersect_world(const World* w) const {
    IntersectionRecord xs;
    for (auto& [key, object] : w->objects) {
        xs.append_record(this->intersect(object.get()));
    }

    std::sort(xs.intersections.begin(), xs.intersections.end(),
              [](Intersection a, Intersection b) { return a.t < b.t; });
    return xs;
}

PrecomputedIntersection PrecomputedIntersection::prepare_computations(
    Intersection i, Ray r) {
    PrecomputedIntersection comps;
    // Copy intersections property
    comps.t = i.t;
    comps.object = i.object;
    // Precompute useful values // TODO: why do this?
    comps.point = r.position(comps.t);
    comps.eye = -r.dir;
    comps.normal = comps.object->normal_at(comps.point);

    comps.inside = false;
    if (comps.normal.dot(comps.eye) < 0) {
        comps.inside = true;
        comps.normal = -comps.normal;
    }

    float eps = EPSILON * fmax(1, comps.t * 2);
    comps.over_point = comps.point + comps.normal * eps;

    return comps;
}