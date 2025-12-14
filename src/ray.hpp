#ifndef RAY_HPP
#define RAY_HPP

#include <iostream>
#include <vector>

#include "sphere.hpp"
#include "transformations.hpp"
#include "tuples.hpp"

struct Intersection {
    float t;
    const Sphere* object;  // const

    Intersection(float t, const Sphere* object) : t(t), object(object) {}

    bool operator==(const Intersection& other) const {
        return t == other.t && object == other.object;
    }
};

// TODO: Dont love this structure
struct IntersectionRecords {
    size_t count;
    std::vector<Intersection> intersections;

    IntersectionRecords() : count(0) {}

    template <typename... IntersectionPack>
    IntersectionRecords(IntersectionPack... intersections)
        requires(std::same_as<IntersectionPack, Intersection> && ...)
        : count(2) {
        this->intersections = {intersections...};
        // std::vector<Intersection> i = {
        //     (std::forward<Intersection>(intersections),
        //     ...)};  // prob need to forward this cause of Sphere
    }

    std::optional<Intersection> hit() const;
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
    IntersectionRecords intersect(const Sphere* s) const;
    Ray transform(Transform m) const { return Ray(m * origin, m * dir); }
};

#endif