#pragma once

#include "../canvas.hpp"
#include "../scene/world.hpp"

struct Camera {
    size_t hsize;
    size_t vsize;
    float fov;
    Transform transform;
    float pixel_size;
    float half_width;
    float half_height;

    Camera(size_t hsize, size_t vsize, float fov);

    Ray ray_for_pixel(size_t px, size_t py) const;
    Canvas render(const World* w) const;
};