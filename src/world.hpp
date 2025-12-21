#pragma once

#include <unordered_set>

#include "lighting.hpp"
#include "lights.hpp"
#include "sphere.hpp"

struct PrecomputedIntersection;

struct UniquePtrHash {
    template <typename T>
    size_t operator()(const std::unique_ptr<T>& p) const {
        return std::hash<T*>{}(p.get());
    }
};

struct UniquePtrEqual {
    template <typename T>
    bool operator()(const std::unique_ptr<T>& a,
                    const std::unique_ptr<T>& b) const {
        return a.get() == b.get();
    }
};

struct World {
    //    private:
    // TODO: does order matter? Might at least for tests
    std::unordered_set<std::unique_ptr<Sphere>, UniquePtrHash, UniquePtrEqual>
        objects;
    std::unordered_set<std::unique_ptr<PointLight>, UniquePtrHash,
                       UniquePtrEqual>
        lights;

    //    public:
    static World default_world();

    Color shade_hit(PrecomputedIntersection comps);
};