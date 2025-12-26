#pragma once

#include <unordered_map>

#include "../geometry/intersection.hpp"
#include "../geometry/shapes.hpp"
#include "../rendering/lighting.hpp"

// struct UniquePtrHash {
//     template <typename T>
//     size_t operator()(const std::unique_ptr<T>& p) const {
//         return std::hash<T*>{}(p.get());
//     }
// };

// struct UniquePtrEqual {
//     template <typename T>
//     bool operator()(const std::unique_ptr<T>& a,
//                     const std::unique_ptr<T>& b) const {
//         return a.get() == b.get();
//     }
// };

// Return object ptrs for testing
struct DefaultWorld;

struct World {
    //    private:
    // TODO: does order matter? Might at least for tests
    // TODO: is this the best way of storing? quick lookup, but is this what
    // should own objects/lights
    std::unordered_map<Sphere*, std::unique_ptr<Sphere>> objects;

    // TODO: allow for multiple lights
    // std::unordered_map<PointLight*, std::unique_ptr<PointLight>> lights;
    std::unique_ptr<PointLight> light;

    //    public:

    static DefaultWorld default_world();

    IntersectionRecord intersect_world(const Ray r) const;
    Color shade_hit(PrecomputedIntersection comps) const;
    Color color_at(Ray r) const;
    bool is_shadowed(Point p) const;

    // TODO: should intersect_world be a member of this? Not ray? cause
    // is_shadowed refers to point...
};

struct DefaultWorld {
    World w;
    Sphere* s1;
    Sphere* s2;
};