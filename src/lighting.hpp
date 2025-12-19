#pragma once

#include "color.hpp"
#include "lights.hpp"
#include "materials.hpp"

namespace Shading {
Color phong_lighting(Material m, PointLight light, Point point, Vector eye,
                     Vector normal);
}