#pragma once

#include "color.hpp"

struct Material {
    Color color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;

    Material(Color color = Color(1, 1, 1), float ambient = .1,
             float diffuse = .9, float specular = .9, float shininess = 200)
        : color(color),
          ambient(ambient),
          diffuse(diffuse),
          specular(specular),
          shininess(shininess) {}

    bool operator==(const Material& other) const {
        return color == other.color && float_equal(ambient, other.ambient) &&
               float_equal(diffuse, other.diffuse) &&
               float_equal(specular, other.specular) &&
               float_equal(shininess, other.shininess);
    }
};