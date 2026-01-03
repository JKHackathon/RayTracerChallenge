#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "geometry/ray.hpp"
#include "rendering/camera.hpp"
#include "rendering/lighting.hpp"

void build_reflective_world(World* w);
void build_refractive_world(World* w);

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
    World w;

    build_reflective_world(&w);

    // auto floor_u = std::make_unique<Plane>();
    // Plane* floor = floor_u.get();
    // floor->material.color = Color(1, 0, 0);
    // floor->material.specular = 0;
    // floor->transform = Transform::translation(0, -.2, 0);


    // // auto water_u = std::make_unique<Plane>();
    // // Plane* water = water_u.get();
    // // water->material.refractive_index = 1;
    // // water->material.transparency = 1;
    // // // water->material.reflective = .1;
    // // // water->material.color = Color(0, 0, 0.1);
    // // water->material.specular = 0;
    // // water->material.diffuse = 0;
    // // water->material.shininess = 0;
    // // water->material.ambient = 0;

    // // build_reflective_world(&w);
    // // build_refractive_world(&w);
    // auto wall_u = std::make_unique<Plane>();
    // auto wall = wall_u.get();
    // wall->transform =
    //     Transform::translation(0, 0, 15) * Transform::rotation_x(M_PI / 2);
    // CheckerPattern wall_pattern(Color(1, 1, 1), Color(0, 0, 0));
    // wall->material.pattern = &wall_pattern;
    // wall->material.specular = 0;



    // // Spheres
    // // auto sphere_u = std::make_unique<GlassSphere>();
    // // GlassSphere* sphere = sphere_u.get();
    // // sphere->transform = Transform::scaling(2, 2, 2);
    // // sphere->material.refractive_index = 2.5;

    // // // Inner air sphere
    // // auto inner_sphere_u = std::make_unique<Sphere>();
    // // Sphere* inner_sphere = inner_sphere_u.get();
    // // inner_sphere->material.transparency = 1;
    // // inner_sphere->material.refractive_index = 1;
    // // inner_sphere->material.ambient = 0;
    // // inner_sphere->material.diffuse = 0;
    // // inner_sphere->material.specular = 0;
    // // inner_sphere->material.shininess = 0;
    // // inner_sphere->material.color = Color(1, 0, 0);
    // // inner_sphere->transform = Transform::scaling(1.8, 1.8, 1.8);

    // // Light
    // auto light_u =
    //     std::make_unique<PointLight>(Point(-10, 10, -10), Color(1, 1, 1));

    // // World
    // w.objects.emplace(wall, std::move(wall_u));
    // w.objects.emplace(floor, std::move(floor_u));
    // // w.objects.emplace(water, std::move(water_u));
    // // w.objects.emplace(sphere, std::move(sphere_u));
    // // w.objects.emplace(inner_sphere, std::move(inner_sphere_u));
    // w.light = std::move(light_u);

    // Camera
    Camera camera(300, 300, M_PI / 2);  // 100, 50, M_PI / 3);
    camera.transform = Transform::view_transform(
        Point(0, .01, -5), Point(0, 0, 0), Vector(0, 1, 0));

    // Rendering
    auto canvas = camera.render(&w);

    std::string ppm_data = canvas.to_ppm();
    std::ofstream outputFile("output.ppm");
    assert(outputFile.is_open());
    outputFile << ppm_data;
    return 0;
}

void build_refractive_world(World* w) {
    // Walls
    auto wall_u = std::make_unique<Plane>();
    auto wall = wall_u.get();
    wall->transform =
        Transform::translation(0, 0, 5) * Transform::rotation_x(M_PI / 2);
    CheckerPattern wall_pattern(Color(1, 1, 1), Color(0, 0, 0));
    wall->material.pattern = &wall_pattern;

    // Spheres
    auto sphere_u = std::make_unique<GlassSphere>();
    GlassSphere* sphere = sphere_u.get();

    // Light
    auto light_u =
        std::make_unique<PointLight>(Point(-10, 10, -10), Color(1, 1, 1));

    // World
    w->objects.emplace(wall, std::move(wall_u));
    w->objects.emplace(sphere, std::move(sphere_u));
    w->light = std::move(light_u);
}

void build_reflective_world(World* w) {
    // Floor
    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    floor->material.specular = 0;
    // floor->material.reflective = .5;

    // Walls
    auto back_wall_u = std::make_unique<Plane>();
    auto back_wall = back_wall_u.get();
    back_wall->transform =
        Transform::translation(0, 0, 5) * Transform::rotation_x(M_PI / 2);
    back_wall->material.color = Color(0, 0, 1);
    back_wall->material.specular = 0;

    // Spheres
    auto middle_s_u = std::make_unique<Sphere>();
    Sphere* middle_s = middle_s_u.get();
    middle_s->transform = Transform::translation(-.5, 1, .5) * Transform::scaling(1,1,2);
    middle_s->material.color = Color(1, 0.2, 1);
    middle_s->material.diffuse = .7;
    middle_s->material.specular = .3;
    middle_s->material.reflective = 1;

    auto right_s_u = std::make_unique<Sphere>();
    Sphere* right_s = right_s_u.get();
    right_s->transform =
        Transform::translation(1.5, .5, -.5) * Transform::scaling(.5, .5, .5);
    right_s->material.diffuse = .7;
    right_s->material.specular = .3;
    right_s->material.reflective = .75;

    auto left_s_u = std::make_unique<Sphere>();
    Sphere* left_s = left_s_u.get();
    left_s->transform = Transform::translation(-1.5, .33, -.75) *
        Transform::scaling(.33, .33, .33);
    left_s->material.color = Color(1, .8, .1);
    left_s->material.diffuse = .7;
    left_s->material.specular = .3;
    left_s->material.reflective = .5;

    // Light
    auto light_u =
        std::make_unique<PointLight>(Point(-10, 10, -10), Color(1, 1, 1));

    // World
    // w->objects.emplace(floor, std::move(floor_u));
    w->objects.emplace(back_wall, std::move(back_wall_u));
    w->objects.emplace(middle_s, std::move(middle_s_u));
    // w->objects.emplace(right_s, std::move(right_s_u));
    // w->objects.emplace(left_s, std::move(left_s_u));
    w->light = std::move(light_u);
}

// TODO: this doesnt work because patterns are local!!
void build_patterns_world(World* w) {
    // Floor
    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    floor->material.specular = 0;
    CheckerPattern pattern(Color(1, 1, 1), Color(0, 0, 0));
    floor->material.pattern = &pattern;
    // floor->material.diffuse = 1;
    // floor->material.ambient = 1;

    // Walls
    auto back_wall_u = std::make_unique<Plane>();
    auto back_wall = back_wall_u.get();
    back_wall->transform =
        Transform::translation(0, 0, 5) * Transform::rotation_x(M_PI / 2);
    StripePattern back_wall_pattern(Color(.8, .8, .8), Color(0, 0, 0));
    back_wall_pattern.transform =
        Transform::scaling(.3, .3, .3) * Transform::rotation_z(M_PI / 4);
    back_wall->material.pattern = &back_wall_pattern;

    // auto right_wall_u = std::make_unique<Sphere>();
    // auto right_wall = right_wall_u.get();
    // right_wall->transform =
    //     Transform::translation(0, 0, 5) * Transform::rotation_y(M_PI / 4) *
    //     Transform::rotation_x(M_PI / 2) * Transform::scaling(10, .01, 10);
    // right_wall->material = floor->material;

    // Spheres
    auto middle_s_u = std::make_unique<Sphere>();
    Sphere* middle_s = middle_s_u.get();
    middle_s->transform = Transform::translation(-.5, 1, .5);
    middle_s->material.color = Color(1, 0.2, 1);
    RingPattern sphere_pattern(Color(1, 0.2, 1), Color(1, 0, 0));
    sphere_pattern.transform =
        Transform::scaling(.1, .1, .1) * Transform::rotation_z(-M_PI / 3) *
        Transform::rotation_x(M_PI / 2.5) * Transform::translation(0, -3.5, 0);
    middle_s->material.pattern = &sphere_pattern;

    middle_s->material.diffuse = .7;
    middle_s->material.specular = .3;

    auto right_s_u = std::make_unique<Sphere>();
    Sphere* right_s = right_s_u.get();
    right_s->transform =
        Transform::translation(1.5, .5, -.5) * Transform::scaling(.5, .5, .5);
    GradientPattern r_sphere_pattern(Color(.5, 1, .1), Color(1, 0, 0));
    r_sphere_pattern.transform =
        Transform::scaling(2, 2, 2) * Transform::translation(.5, 0, 0);
    right_s->material.pattern = &r_sphere_pattern;
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
    w->objects.emplace(floor, std::move(floor_u));
    w->objects.emplace(back_wall, std::move(back_wall_u));
    // w.objects.emplace(right_wall, std::move(right_wall_u));
    w->objects.emplace(middle_s, std::move(middle_s_u));
    w->objects.emplace(right_s, std::move(right_s_u));
    w->objects.emplace(left_s, std::move(left_s_u));
    w->light = std::move(light_u);
}