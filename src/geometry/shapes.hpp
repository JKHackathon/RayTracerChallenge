#pragma once

#include <optional>

#include "../math/transformations.hpp"
#include "../math/tuples.hpp"
#include "../scene/materials.hpp"
#include "intersection.hpp"
#include "ray.hpp"

// struct IntersectionRecord;

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

struct Sphere : public Shape {
public:
    Point origin;
    float radius;
    // Transform transform;
    // Material material;

    Sphere() : origin(Point(0, 0, 0)), radius(1) {}
    Sphere(Transform transform, Material material)
        : Shape(transform, material), origin(Point(0, 0, 0)), radius(1) {
    }

private:
    Vector local_normal_at(const Point local_p) const override {
        return local_p - origin;
    }

    // TODO: abstract to any objects in scene
    // TO UNDERSTAND: why do I want my intersect to check things behind the ray?
    // For reflections!!! + chapter 16
    // TODO: look into math of it
    IntersectionRecord local_intersect(const Ray local_r) const override;
};

struct GlassSphere : public Sphere {
    GlassSphere() {
        material.transparency = 1;
        material.refractive_index = 1.5;
    }
};

struct Plane : public Shape {
private:
    Vector local_normal_at(const Point local_p) const override {
        return Vector(0, 1, 0);
    }

    IntersectionRecord local_intersect(const Ray local_r) const override {
        if (abs(local_r.dir.y) < EPSILON) {
            return IntersectionRecord();
        }

        float t = -local_r.origin.y / local_r.dir.y;
        return Intersection(t, this);
    }
};