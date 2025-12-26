#pragma once

#include "../math/transformations.hpp"
#include "../math/tuples.hpp"
#include "../scene/materials.hpp"
#include "intersection.hpp"
#include "ray.hpp"

// struct IntersectionRecord;

struct Shape {
    Transform transform;
    Material material;

    Shape() {}
    Shape(Transform transform, Material material)
        : transform(transform), material(material) {}

    // TODO: fully understand how (pure) virtual/override works!
    virtual Vector normal_at(const Point p) const = 0;
    virtual IntersectionRecord intersect(const Ray r) const = 0;
};

struct TestShape : public Shape {
    virtual Vector normal_at(const Point p) const { return Vector(0, 0, 0); }
    virtual IntersectionRecord intersect(const Ray r) const {
        return IntersectionRecord();
    }
};

struct Sphere : public Shape {
    Point origin;
    float radius;
    // Transform transform;
    // Material material;

    Sphere() : origin(Point(0, 0, 0)), radius(1) {}
    Sphere(Transform transform, Material material)
        : Shape(transform, material), origin(Point(0, 0, 0)), radius(1) {}

    Vector normal_at(Point p) const override {
        Vector n_o = transform.inverse() * p - origin;
        auto n_w = transform.inverse().transpose() * n_o;
        n_w.w = 0;
        return Vector(n_w).normalized();
    }

    // TODO: abstract to any objects in scene
    // TO UNDERSTAND: why do I want my intersect to check things behind the ray?
    // For reflections!!! + chapter 16
    // TODO: look into math of it
    IntersectionRecord intersect(const Ray r) const override;
};