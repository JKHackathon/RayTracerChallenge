#include "camera.hpp"

#include <iomanip>

Camera::Camera(size_t hsize, size_t vsize, double fov)
    : hsize(hsize), vsize(vsize), fov(fov), transform(identity_matrix4) {
    double half_view = tan(fov / 2);
    double aspect = float(hsize) / vsize;

    if (aspect >= 1) {
        half_width = half_view;
        half_height = half_view / aspect;
    }
    if (aspect < 1) {  // TODO: why does it flip??? pg 102
        half_width = half_view * aspect;
        half_height = half_view;
    }

    pixel_size = half_width * 2 / hsize;
}

Ray Camera::ray_for_pixel(size_t px, size_t py) const {
    // Offset from edge of canvas to pixel's center
    double xoffset = (px + 0.5) * pixel_size;
    double yoffset = (py + 0.5) * pixel_size;

    // untransformed coords of pixel in world space (camera looks toward -z)
    double world_x = half_width - xoffset;
    double world_y = half_height - yoffset;

    // Using camera matrix, transfomr canvas point + origin, compute ray's dir
    // vector (canvas at z=-1)
    auto pixel = transform.inverse() * Point(world_x, world_y, -1);
    auto origin = transform.inverse() * Point(0, 0, 0);
    Vector dir = Vector(pixel - origin).normalized();
    return Ray(origin, dir);
}

Canvas Camera::render(const World* w) const {
    Canvas image(hsize, vsize);

    constexpr int bar_width = 50;

    for (size_t y = 0; y < vsize; y++) {
        for (size_t x = 0; x < hsize; x++) {
            Ray r = ray_for_pixel(x, y);
            Color c = w->color_at(r); // Default to depth of 5 // TODO: why did it seg fault when set to 0?
            image.write_pixel(x, y, c);
        }
        // Progress bar update (per row)
        double progress = static_cast<float>(y + 1) / vsize;
        int pos = static_cast<int>(bar_width * progress);

        std::cout << "\r[";
        for (int i = 0; i < bar_width; ++i) {
            if (i < pos)
                std::cout << "=";
            else if (i == pos)
                std::cout << ">";
            else
                std::cout << " ";
        }
        std::cout << "] " << std::setw(3) << static_cast<int>(progress * 100)
            << "%";

        std::cout.flush();
    }
    std::cout << "\n";
    return image;
}