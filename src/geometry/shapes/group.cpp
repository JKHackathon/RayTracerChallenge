#include "group.hpp"
#include "shapes.hpp"

Vector Group::local_normal_at(const Point local_p) const {
    throw std::runtime_error("Group local_normal_at shouold not be used!\n");
}

IntersectionRecord Group::local_intersect(const Ray local_r) const {
    IntersectionRecord xs;
    for (const auto& shape : shapes) {
        xs.append_record(shape.get()->intersect(local_r));
    }
    std::sort(xs.intersections.begin(), xs.intersections.end(), [](Intersection a, Intersection b) {
        return a.t < b.t;
        });
    return xs;
}