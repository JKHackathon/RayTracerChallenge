#pragma once

#include "../scene/lights.hpp"
#include "../scene/materials.hpp"

namespace Shading {
Color phong_lighting(Material m, const PointLight* light, Point point,
                     Vector eye, Vector normal, bool in_shadow = false);
}