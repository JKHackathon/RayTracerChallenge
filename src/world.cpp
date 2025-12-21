#include "world.hpp"

#include "ray.hpp"

World World::default_world() {
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

Color World::shade_hit(PrecomputedIntersection comps) {
    // TODO: change to allow for multiple lights
    return Shading::phong_lighting(comps.object->material,
                                   lights.begin()->get(), comps.point,
                                   comps.eye, comps.normal);
}