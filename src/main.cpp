#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "canvas.hpp"
#include "lighting.hpp"
#include "lights.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "transformations.hpp"

int main(int argc, char* argv[]) {
    if (argc > 1 && (std::strcmp(argv[1], "-h") == 0 ||
                     std::strcmp(argv[1], "--help") == 0)) {
        std::cout << "./raytracer [canvas_width] [canvas_height] [pixel_scale]";
        return 0;
    }
    int width = 500;
    int height = 500;

    if (argc > 2) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    int pixel_scale = std::min(width, height);
    if (argc > 3) {
        pixel_scale = atoi(argv[3]);
    }

    Canvas canvas(width, height);

    // Sphere
    auto s_ptr = std::make_unique<Sphere>();
    Sphere* s = s_ptr.get();
    s->set_transform(Transform::translation(0, 0, 10));
    s->material = Material();
    s->material.color = Color(1, 0.2, 1);

    // Light
    PointLight light(Point(-10, 10, -10), Color(1, 1, 1));

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
                Point hit_position = camera_ray.position(s_intersection.t);
                Vector surface_normal =
                    s_intersection.object->normal_at(hit_position);
                auto eye = -camera_ray.dir;
                Color li = Shading::phong_lighting(
                    s_intersection.object->material, light, hit_position, eye,
                    surface_normal);
                canvas.write_pixel(i, j, li);
            }
        }
    }

    std::string ppm_data = canvas.to_ppm();
    std::ofstream outputFile("output.ppm");
    assert(outputFile.is_open());
    outputFile << ppm_data;
    return 0;
}