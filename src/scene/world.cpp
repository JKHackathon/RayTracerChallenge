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

Color World::shade_hit(PrecomputedIntersection comps, int recursion_depth) const {
    bool shadowed = is_shadowed(comps.over_point);
    // TODO: change to allow for multiple lights
    Color surface = Shading::phong_lighting(comps.object->material, comps.object,
        light.get(), comps.over_point, comps.eye, // TESTED CHANGING TO OVER_POINT
        comps.normal, shadowed);
    Color reflected = reflected_color(comps, recursion_depth);
    Color refracted = refracted_color(comps, recursion_depth);
    return surface + reflected + refracted;
}

Color World::color_at(Ray r, int recursion_depth) const {
    auto xs = this->intersect_world(r);
    auto hit = xs.hit();
    if (!hit.has_value()) {
        return Color(0, 0, 0);
    }

    auto comps = PrecomputedIntersection::prepare_computations(hit.value(), r);
    // ---- DEBUGGING: visualize which object was hit ----
    // if (hit->object->material.reflective == 1) return Color(1, 0, 0); // red = self-hit
    // else if (hit->object->material.reflective != 1) return Color(0, 1, 0);
    return shade_hit(comps, recursion_depth);
}

bool World::is_shadowed(Point p) const {
    Vector p_to_light = light.get()->pos - p;
    double distance = p_to_light.magnitude();
    Vector dir_to_light = p_to_light.normalized();

    Ray r(p, dir_to_light);
    auto intersections = this->intersect_world(r);
    auto hit = intersections.hit();
    return hit.has_value() && hit.value().t < distance; // && hit.value().t > 0;
}

Color World::reflected_color(PrecomputedIntersection comps, int recursion_depth) const {
    if (recursion_depth >= REFLECTION_DEPTH || double_equal(comps.object->material.reflective, 0)) {
        return Color(0, 0, 0);
    }

    Ray reflected_ray(comps.over_point, comps.reflect_dir);
    return color_at(reflected_ray, recursion_depth + 1) * comps.object->material.reflective;
}

Color World::refracted_color(PrecomputedIntersection comps, int recursion_depth) const {
    if (recursion_depth >= REFLECTION_DEPTH || double_equal(comps.object->material.transparency, 0)) {
        return Color(0, 0, 0);
    }

    // Snell's Law
    double n_ratio = comps.n1 / comps.n2;
    double cos_i = comps.eye.dot(comps.normal);
    double sin2_t = pow(n_ratio, 2) * (1 - pow(cos_i, 2));

    if (sin2_t > 1) { // Total internal reflection
        assert(false);
        std::cout << "Total Internal reflection!!!\n";
        return Color(0, 0, 0);
    }

    double cos_t = sqrt(1 - sin2_t);
    Vector refracted_dir = comps.normal * (n_ratio * cos_i - cos_t) - comps.eye * n_ratio;
    Ray refracted_ray(comps.under_point, refracted_dir.normalized());

    return color_at(refracted_ray, recursion_depth + 1) * comps.object->material.transparency;
}