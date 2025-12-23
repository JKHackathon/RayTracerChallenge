#include "world.hpp"

#include "ray.hpp"

DefaultWorld World::default_world() {
    DefaultWorld result;
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

    // Get before moving!!!
    result.s1 = s1.get();
    result.s2 = s2.get();

    result.w.objects.emplace(s1.get(), std::move(s1));
    result.w.objects.emplace(s2.get(), std::move(s2));
    result.w.lights.emplace(light.get(), std::move(light));

    return result;
}

Color World::shade_hit(PrecomputedIntersection comps) const {
    // TODO: change to allow for multiple lights
    return Shading::phong_lighting(comps.object->material,
                                   lights.begin()->first, comps.point,
                                   comps.eye, comps.normal);
}