#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "camera.hpp"
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

    // Canvas canvas(width, height);

    // Floor
    auto floor_u = std::make_unique<Sphere>();
    Sphere* floor = floor_u.get();
    floor->transform = Transform::scaling(10, 0.01, 10);
    // floor->material = Material();
    floor->material.color = Color(1, .9, .9);
    floor->material.specular = 0;
    // floor->material.diffuse = 1;
    // floor->material.ambient = 1;

    // Walls
    auto left_wall_u = std::make_unique<Sphere>();
    auto left_wall = left_wall_u.get();
    left_wall->transform =
        Transform::translation(0, 0, 5) * Transform::rotation_y(-M_PI / 4) *
        Transform::rotation_x(M_PI / 2) * Transform::scaling(10, .01, 10);
    left_wall->material = floor->material;

    auto right_wall_u = std::make_unique<Sphere>();
    auto right_wall = right_wall_u.get();
    right_wall->transform =
        Transform::translation(0, 0, 5) * Transform::rotation_y(M_PI / 4) *
        Transform::rotation_x(M_PI / 2) * Transform::scaling(10, .01, 10);
    right_wall->material = floor->material;

    // Spheres
    auto middle_s_u = std::make_unique<Sphere>();
    Sphere* middle_s = middle_s_u.get();
    middle_s->transform = Transform::translation(-.5, 1, .5);
    middle_s->material.color = Color(1, 0.2, 1);
    middle_s->material.diffuse = .7;
    middle_s->material.specular = .3;

    auto right_s_u = std::make_unique<Sphere>();
    Sphere* right_s = right_s_u.get();
    right_s->transform =
        Transform::translation(1.5, .5, -.5) * Transform::scaling(.5, .5, .5);
    right_s->material.color = Color(.5, 1, .1);
    right_s->material.diffuse = .7;
    right_s->material.specular = .3;

    auto left_s_u = std::make_unique<Sphere>();
    Sphere* left_s = left_s_u.get();
    left_s->transform = Transform::translation(-1.5, .33, -.75) *
                        Transform::scaling(.33, .33, .33);
    left_s->material.color = Color(1, .8, .1);
    left_s->material.diffuse = .7;
    left_s->material.specular = .3;

    // Light
    auto light_u =
        std::make_unique<PointLight>(Point(-10, 10, -10), Color(1, 1, 1));

    // World
    World w;
    w.objects.emplace(floor, std::move(floor_u));
    w.objects.emplace(left_wall, std::move(left_wall_u));
    w.objects.emplace(right_wall, std::move(right_wall_u));
    w.objects.emplace(middle_s, std::move(middle_s_u));
    w.objects.emplace(right_s, std::move(right_s_u));
    w.objects.emplace(left_s, std::move(left_s_u));
    w.light = std::move(light_u);

    // Camera
    Camera camera(300, 150, M_PI / 3);  // 100, 50, M_PI / 3);
    camera.transform = Transform::view_transform(
        Point(0, 1.5, -5), Point(0, 1, 0), Vector(0, 1, 0));

    // Rendering
    auto canvas = camera.render(&w);

    // float wall_size = 7;

    // for (int i = 0; i < width; i++) {
    //     for (int j = 0; j < height; j++) {
    //         // auto y = wall_size / 2 - j * wall_size / pixel_scale;
    //         Vector camera_dir = Point((i - (width / 2.f)) / pixel_scale,
    //                                   ((height / 2.f) - j) / pixel_scale, 0)
    //                                   -
    //                             camera;
    //         camera_dir = camera_dir.normalized();
    //         // printf("%f %f %f\n", camera_dir.x, camera_dir.y,
    //         camera_dir.z); Ray camera_ray(camera, camera_dir);

    //         auto xs = camera_ray.intersect(s);
    //         auto hit = xs.hit();

    //         if (hit.has_value()) {
    //             Intersection s_intersection = hit.value();
    //             Point hit_position = camera_ray.position(s_intersection.t);
    //             Vector surface_normal =
    //                 s_intersection.object->normal_at(hit_position);
    //             auto eye = -camera_ray.dir;
    //             Color li = Shading::phong_lighting(
    //                 s_intersection.object->material, &light, hit_position,
    //                 eye, surface_normal);
    //             canvas.write_pixel(i, j, li);
    //         }
    //     }
    // }

    std::string ppm_data = canvas.to_ppm();
    std::ofstream outputFile("output.ppm");
    assert(outputFile.is_open());
    outputFile << ppm_data;
    return 0;
}