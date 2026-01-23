#include "shapes.hpp"
#include "group.hpp"

Vector Shape::normal_at(const Point world_p, const Intersection i) const {
    Point local_p = this->world_to_object(world_p);
    Vector local_n = this->local_normal_at(local_p, i);
    return this->normal_to_world(local_n);
}

IntersectionRecord Shape::intersect(const Ray r) const {
    Ray obj_space_ray = r.transform(transform.inverse());
    return this->local_intersect(obj_space_ray);
}

Point Shape::world_to_object(Point p) const {
    if (parent.has_value()) {
        p = parent.value()->world_to_object(p);
    }
    return transform.inverse() * p;
}

Vector Shape::normal_to_world(Vector normal) const {
    auto temp_normal = transform.inverse().transpose() * normal;
    temp_normal.w = 0;
    normal = Vector(temp_normal).normalized();
    if (parent.has_value()) {
        normal = parent.value()->normal_to_world(normal);
    }
    return normal;
}