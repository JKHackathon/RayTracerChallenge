#include "intersection.hpp"

#include <algorithm>

#include "shapes/shapes.hpp"
#include <set>
#include <list>

std::optional<Intersection> IntersectionRecord::hit() const {
    double hit_val = MAXFLOAT;
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
    Intersection i, Ray r, IntersectionRecord* xs) {
    PrecomputedIntersection comps;
    // Copy intersections property
    comps.t = i.t;
    comps.object = i.object;
    // Precompute useful values // TODO: why do this?
    comps.point = r.position(comps.t);
    comps.eye = -r.dir;
    comps.normal = comps.object->normal_at(comps.point);

    // flip normal if inside object
    comps.inside = false;
    if (comps.normal.dot(comps.eye) < 0) {
        comps.inside = true;
        comps.normal = -comps.normal;
    }

    double eps = EPSILON * fmax(1, sqrt(pow(comps.point.x, 2) + pow(comps.point.y, 2) + pow(comps.point.z, 2)));//comps.t * 100);
    // double cos_theta = abs(comps.normal.dot(r.dir));
    // double offset = EPSILON / std::max(cos_theta, EPSILON);
    comps.over_point = comps.point + comps.normal * EPSILON;

    // Reflections
    comps.reflect_dir = r.dir.reflect(comps.normal).normalized();

    // Refraction
    comps.under_point = comps.point - comps.normal * EPSILON;

    if (!xs) {
        return comps;
    }

    // TODO: need ordered hash set (combination of list + unordered_map)
    std::list<const Shape*> objects_within;
    std::unordered_map<const Shape*, std::list<const Shape*>::iterator> objects_within_iterators;

    // std::set<const Shape*> objects_within; 
    bool valid_hit;

    for (auto hit : xs->intersections) {
        if (i == hit) {
            valid_hit = true;
            if (objects_within.empty()) { // if empty, that means we are entering i.object from air
                comps.n1 = 1;
                comps.n2 = i.object->material.refractive_index;
                break;
            }

            if (objects_within_iterators.contains(i.object)) { // if not empty and contains i.object, we are exiting i.object
                // n1 = last entered object, not always the object we are exiting (if overlap)
                comps.n1 = objects_within.back()->material.refractive_index;
                // comps.n1 = i.object->material.refractive_index;
                auto object_it = objects_within_iterators.at(i.object);
                objects_within.erase(object_it);
                objects_within_iterators.erase(i.object);

                if (objects_within.empty()) { // if empty, exiting to air
                    comps.n2 = 1;
                    break;
                }

                comps.n2 = objects_within.back()->material.refractive_index;
                break;
            }
            else { // if not empty and i.object not there, we are entering object from another object
                comps.n2 = i.object->material.refractive_index;
                comps.n1 = objects_within.back()->material.refractive_index;
                break;
            }
        }

        // not the hit we are computing for
        if (objects_within_iterators.contains(hit.object)) { // contains, therefore exiting object
            auto object_it = objects_within_iterators.at(hit.object);
            objects_within.erase(object_it);
            objects_within_iterators.erase(hit.object);
        }
        else { // entering object
            objects_within.push_back(hit.object);
            objects_within_iterators.emplace(hit.object, std::prev(objects_within.end()));
        }
    }

    assert(valid_hit);

    return comps;
}

double Refraction::schlick(PrecomputedIntersection comps) {
    double cos = comps.eye.dot(comps.normal); //cos of angle between eye + normal

    // TIR only if n1 > n2
    if (comps.n1 > comps.n2) {
        double n = comps.n1 / comps.n2;

        double sin2_t = pow(n, 2) * (1 - pow(cos, 2));
        if (sin2_t > 1.0) { // TIR - all light is reflected, none refracted
            return 1.0;
        }
        cos = sqrt(1 - sin2_t); // n1>n2: use cos(theta_t) instead
    }
    // TODO: look at  “Reflections and Refractions in Ray Tracing,” by Bram de Greve

    double r_theta = pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);
    return r_theta + (1 - r_theta) * pow(1 - cos, 5);
}