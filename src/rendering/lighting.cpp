#include "lighting.hpp"

// TODO: iterate over all light sources
Color Shading::phong_lighting(Material m, const Shape* object,
                              const PointLight* light, Point point, Vector eye,
                              Vector normal, bool in_shadow) {
    Color effective_color = m.color * light->intensity;
    if (m.pattern) {
        effective_color =
            m.pattern->pattern_at_shape(object, point) * light->intensity;
    }

    Vector dir_to_light = Vector(light->pos - point).normalized();

    auto ambient = effective_color * m.ambient;

    if (in_shadow) {
        return ambient;
    }

    // light_dot_normal represents the cosine of the angle between the
    // light vector and the normal vector. A negative number means the
    // light is on the other side of the surface.
    auto light_dot_normal = dir_to_light.dot(normal);
    Color diffuse(Color(0, 0, 0));
    Color specular(Color(0, 0, 0));
    if (light_dot_normal >= 0) {
        diffuse = effective_color * m.diffuse * light_dot_normal;
        Vector reflect_dir = (-dir_to_light).reflect(normal);
        // negative # means light reflects away from eye
        auto reflect_dot_eye = reflect_dir.dot(eye);
        if (reflect_dot_eye > 0) {
            // Compute specular contribution
            specular = light->intensity * m.specular *
                       pow(reflect_dot_eye, m.shininess);
        }
    }

    return ambient + diffuse + specular;
}
