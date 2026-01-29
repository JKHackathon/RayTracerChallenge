#pragma once

#include <optional>

#include "../../math/transformations.hpp"
#include "../../math/tuples.hpp"
#include "../../scene/materials.hpp"
#include "../intersection.hpp"
#include "../ray.hpp"
#include "../../accel/bounding_box.hpp"

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

    Vector normal_at(const Point p, const Intersection i = Intersection()) const;
    IntersectionRecord intersect(const Ray r) const;
    Point world_to_object(Point p) const;
    Vector normal_to_world(Vector normal) const;
    virtual BoundingBox bounds_of() const = 0;

    BoundingBox parent_space_bounds_of() const {
        return bounds_of().transform(transform);
    }

    virtual void divide(int min_children) {}

private:
    virtual IntersectionRecord local_intersect(const Ray local_r) const = 0;
    virtual Vector local_normal_at(const Point local_p, Intersection i) const = 0;
};

struct TestShape : public Shape {
public:
    mutable std::optional<Ray> saved_ray;

    BoundingBox bounds_of() const override {
        return BoundingBox(Point(-1, -1, -1), Point(1, 1, 1));
    }
private:
    Vector local_normal_at(const Point p, const Intersection i) const override {
        return Vector(p.x, p.y, p.z);
    }
    IntersectionRecord local_intersect(const Ray local_r) const override {
        saved_ray = local_r;
        return IntersectionRecord();
    }
};