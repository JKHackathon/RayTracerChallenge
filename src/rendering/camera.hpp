#pragma once

#include "../canvas.hpp"
#include "../scene/world.hpp"

struct Camera {
    size_t hsize;
    size_t vsize;
    double fov;
    Transform transform;
    double pixel_size;
    double half_width;
    double half_height;

    Camera(size_t hsize, size_t vsize, double fov);

    Ray ray_for_pixel(size_t px, size_t py) const;
    Canvas render(const World* w) const;
};