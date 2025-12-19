#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "canvas.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "transformations.hpp"

int main(int argc, char* argv[]) {
    if (argc > 1 && (std::strcmp(argv[1], "-h") == 0 ||
                     std::strcmp(argv[1], "--help") == 0)) {
        std::cout << "./raytracer [canvas_width] [canvas_height] [pixel_scale]";
        return 0;
    }
    int width = 100;
    int height = 100;

    if (argc > 2) {
        width =  atoi(argv[1]);
        height = atoi(argv[2]);
    }
    int pixel_scale = std::min(width, height);
    if (argc > 3) {
        pixel_scale = atoi(argv[3]);
    }

    Canvas canvas(width, height);

    auto s_ptr = std::make_unique<Sphere>();
    Sphere* s = s_ptr.get();
    s->set_transform(Transform::translation(0, 0, 10));

    Point camera(0, 0, -5);
    float wall_size = 7;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // auto y = wall_size / 2 - j * wall_size / pixel_scale;
            Vector camera_dir = Point((i - (width / 2.f)) / pixel_scale,
                                      ((height / 2.f) - j) / pixel_scale, 0) -
                                camera;
            camera_dir = camera_dir.normalized();
            // printf("%f %f %f\n", camera_dir.x, camera_dir.y, camera_dir.z);
            Ray camera_ray(camera, camera_dir);
            auto xs = camera_ray.intersect(s);
            auto hit = xs.hit();
            if (hit.has_value()) {
                Intersection s_intersection = hit.value();
                canvas.write_pixel(i, j, Color(0, 255, 0));
            }
        }
    }

    std::string ppm_data = canvas.to_ppm();
    std::ofstream outputFile("output.ppm");
    assert(outputFile.is_open());
    outputFile << ppm_data;
    return 0;
}