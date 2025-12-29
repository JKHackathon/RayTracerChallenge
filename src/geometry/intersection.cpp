#include "intersection.hpp"

#include <algorithm>

#include "shapes.hpp"

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

    comps.reflect_dir = r.dir.reflect(comps.normal).normalized();

    float eps = .001;//EPSILON * fmax(1, comps.t * 2);
    comps.over_point = comps.point + comps.normal * eps;

    return comps;
}