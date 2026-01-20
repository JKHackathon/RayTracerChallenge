#pragma once

#include <optional>

#include "../../math/transformations.hpp"
#include "../../math/tuples.hpp"
#include "../../scene/materials.hpp"
#include "../intersection.hpp"
#include "../ray.hpp"

struct Group;

struct Shape {
public:
    Transform transform;
    Material material;
    std::optional<Group*> parent; // Does not own group, therefore

    Shape() {}
    Shape(Transform transform, Material material)
        : transform(transform), material(material) {
    }
    virtual ~Shape() = default; // Required for children's destructor to be called

    Vector normal_at(const Point p) const;
    IntersectionRecord intersect(const Ray r) const;
    Point world_to_object(Point p) const;
    Vector normal_to_world(Vector normal) const;
private:
    virtual IntersectionRecord local_intersect(const Ray local_r) const = 0;
    virtual Vector local_normal_at(const Point local_p) const = 0;
};

struct TestShape : public Shape {
public:
    mutable std::optional<Ray> saved_ray;

private:
    Vector local_normal_at(const Point p) const override {
        return Vector(p.x, p.y, p.z);
    }
    IntersectionRecord local_intersect(const Ray local_r) const override {
        saved_ray = local_r;
        return IntersectionRecord();
    }
};