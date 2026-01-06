#pragma once

#include <optional>

#include "../../math/transformations.hpp"
#include "../../math/tuples.hpp"
#include "../../scene/materials.hpp"
#include "../intersection.hpp"
#include "../ray.hpp"

struct Shape {
public:
    Transform transform;
    Material material;

    Shape() {}
    Shape(Transform transform, Material material)
        : transform(transform), material(material) {
    }
    virtual ~Shape() = default; // Required for children's destructor to be called

    Vector normal_at(const Point p) const {
        Vector n_o = this->local_normal_at(transform.inverse() * p);
        auto n_w = transform.inverse().transpose() * n_o;
        n_w.w = 0;
        return Vector(n_w).normalized();
    }

    IntersectionRecord intersect(const Ray r) {
        Ray obj_space_ray = r.transform(transform.inverse());
        return this->local_intersect(obj_space_ray);
    }

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