#pragma once

#include <unordered_set>

#include "lights.hpp"
#include "sphere.hpp"

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
    std::unordered_set<std::unique_ptr<Sphere>, UniquePtrHash, UniquePtrEqual>
        objects;
    std::unordered_set<std::unique_ptr<PointLight>, UniquePtrHash,
                       UniquePtrEqual>
        lights;

    //    public:
    static World default_world() {
        auto light =
            std::make_unique<PointLight>(Point(-10, 10, -10), Color(1, 1, 1));

        Material m;
        m.color = Color(0.8, 1, 0.6);
        m.diffuse = 0.7;
        m.specular = 0.2;
        auto s1 = std::make_unique<Sphere>();
        s1.get()->material = m;
        auto s2 = std::make_unique<Sphere>();
        s2.get()->transform = Transform::scaling(0.5, 0.5, 0.5);

        World w;
        w.objects.emplace(std::move(s1));
        w.objects.emplace(std::move(s2));
        w.lights.emplace(std::move(light));

        return w;
    }
};