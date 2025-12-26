#include "world.hpp"

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
    // result.w.lights.emplace(light.get(), std::move(light));
    result.w.light = std::move(light);

    return result;
}

IntersectionRecord World::intersect_world(const Ray r) const {
    IntersectionRecord xs;
    for (auto& [key, object] : objects) {
        xs.append_record(object.get()->intersect(r));
    }

    std::sort(xs.intersections.begin(), xs.intersections.end(),
              [](Intersection a, Intersection b) { return a.t < b.t; });
    return xs;
}

Color World::shade_hit(PrecomputedIntersection comps) const {
    bool shadowed = is_shadowed(comps.over_point);
    // TODO: change to allow for multiple lights
    return Shading::phong_lighting(comps.object->material, light.get(),
                                   comps.point, comps.eye, comps.normal,
                                   shadowed);
}

Color World::color_at(Ray r) const {
    auto xs = this->intersect_world(r);
    auto hit = xs.hit();
    if (!hit.has_value()) {
        return Color(0, 0, 0);
    }

    auto comps = PrecomputedIntersection::prepare_computations(hit.value(), r);
    return shade_hit(comps);
}

bool World::is_shadowed(Point p) const {
    Vector p_to_light = light.get()->pos - p;
    float distance = p_to_light.magnitude();
    Vector dir_to_light = p_to_light.normalized();

    Ray r(p, dir_to_light);
    auto intersections = this->intersect_world(r);
    auto hit = intersections.hit();
    return hit.has_value() && hit.value().t < distance;
}
