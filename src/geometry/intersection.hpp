#pragma once

#include "ray.hpp"
// #include "shapes.hpp"

struct Shape;

struct Intersection {
    double t;
    const Shape* object;  // const

    Intersection(double t, const Shape* object) : t(t), object(object) {}

    bool operator==(const Intersection& other) const {
        return t == other.t && object == other.object;
    }
};

// TODO: Dont love this structure
struct IntersectionRecord {
    size_t count;
    std::vector<Intersection> intersections;

    IntersectionRecord() : count(0) {}

    template <typename... IntersectionPack>
    IntersectionRecord(IntersectionPack... intersections)
        requires(std::same_as<IntersectionPack, Intersection> && ...)
    : count(sizeof...(IntersectionPack)) {
        this->intersections = { intersections... };
        // std::vector<Intersection> i = {
        //     (std::forward<Intersection>(intersections),
        //     ...)};  // prob need to forward this cause of Sphere
    }

    std::optional<Intersection> hit() const;

    // TODO: Can use move rather than copy for greater efficiency
    void append_record(const IntersectionRecord& other) {
        count += other.count;
        for (auto& intersection : other.intersections) {
            intersections.push_back(intersection);
        }
        assert(intersections.size() == count);
    }
};

struct PrecomputedIntersection {
    double t;
    const Shape* object;
    Point point;
    Vector eye;
    Vector normal;
    bool inside;
    Point over_point; // origin of shadows above surface
    Vector reflect_dir;

    // refraction
    double n1;
    double n2;
    Point under_point; // origin of refracted rays under surface

    PrecomputedIntersection() {}

    static PrecomputedIntersection prepare_computations(Intersection i, Ray r, IntersectionRecord* xs = nullptr);
};

// TODO: rename
namespace Refraction {
    double schlick(PrecomputedIntersection comps);
}