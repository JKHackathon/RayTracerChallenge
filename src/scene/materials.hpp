#pragma once

#include <optional>

#include "patterns.hpp"

struct Material {
    Color color;
    double ambient;
    double diffuse;
    double specular;
    double shininess;
    Pattern* pattern;
    double reflective;
    double transparency;
    double refractive_index;

    // TODO: why are these my default values again?
    Material(Color color = Color(1, 1, 1), double ambient = .1,
        double diffuse = .9, double specular = .9, double shininess = 200,
        Pattern* pattern = nullptr, double reflective = 0.0, double transparency = 0.0, double refractive_index = 1)
        : color(color),
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        shininess(shininess),
        pattern(pattern),
        reflective(reflective),
        transparency(transparency),
        refractive_index(refractive_index) {
    }

    bool operator==(const Material& other) const {
        return color == other.color && double_equal(ambient, other.ambient) &&
            double_equal(diffuse, other.diffuse) &&
            double_equal(specular, other.specular) &&
            double_equal(shininess, other.shininess) &&
            pattern == other.pattern &&
            double_equal(reflective, other.reflective);
    }
};