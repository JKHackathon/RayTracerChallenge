#pragma once

#include "color.hpp"

struct Material {
    Color color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;

    Material()
        : color(Color(1, 1, 1)),
          ambient(.1),
          diffuse(.9),
          specular(.9),
          shininess(200) {}

    bool operator==(const Material& other) const {
        return color == other.color && float_equal(ambient, other.ambient) &&
               float_equal(diffuse, other.diffuse) &&
               float_equal(specular, other.specular) &&
               float_equal(shininess, other.shininess);
    }
};