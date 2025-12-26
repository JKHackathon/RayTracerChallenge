#ifndef RAY_HPP
#define RAY_HPP

#include <iostream>
#include <vector>

#include "../scene/world.hpp"
#include "sphere.hpp"

struct Intersection {
    float t;
    const Sphere* object;  // const

    Intersection(float t, const Sphere* object) : t(t), object(object) {}

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
        this->intersections = {intersections...};
        // std::vector<Intersection> i = {
        //     (std::forward<Intersection>(intersections),
        //     ...)};  // prob need to forward this cause of Sphere
    }

    // IntersectionRecord(IntersectionRecord xs1, IntersectionRecord xs2) {
    //     IntersectionRecord xs;
    //     xs.count == xs1.count + xs2.count;
    //     for (auto& intersection : xs1) {
    //         xs.intersections.push_back(intersection);
    //     }
    //     for (auto& intersection : xs2) {
    //         xs.intersections.push_back(intersection);
    //     }
    //     return xs;
    // }

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

struct Ray {
    Point origin;
    Vector dir;

    // TODO: unclear if within textbook structure
    // std::vector<Intersection> intersections;

    Ray(Point origin, Vector dir) : origin(origin), dir(dir) {}

    Point position(float t) { return origin + dir * t; }

    // TODO: abstract to any objects in scene
    // TO UNDERSTAND: why do I want my intersect to check things behind the ray?
    // For reflections!!! + chapter 16
    // TODO: look into math of it
    IntersectionRecord intersect(const Sphere* s) const;
    Ray transform(Transform m) const { return Ray(m * origin, m * dir); }
    IntersectionRecord intersect_world(const World* w) const;
};

struct PrecomputedIntersection {
    float t;
    const Sphere* object;
    Point point;
    Vector eye;
    Vector normal;
    bool inside;
    Point over_point;

    PrecomputedIntersection() {}

    static PrecomputedIntersection prepare_computations(Intersection i, Ray r);
};

#endif