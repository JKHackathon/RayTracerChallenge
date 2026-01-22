#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "geometry/ray.hpp"
#include "rendering/camera.hpp"
#include "rendering/lighting.hpp"

Canvas shadow_puppets_scene();

Canvas reflection_and_refraction_scene();
Canvas glass_air_bubble_forum_scene();
Canvas glass_air_bubble_exact_scene();

Canvas glass_air_cube_scene();
Canvas reflection_and_refraction_cube_scene();

Canvas cylinder_scene();

Canvas cone_scene();

Canvas group_scene();

int main(int argc, char* argv[]) {

    Canvas canvas = group_scene();//reflection_and_refraction_scene(); //shadow_puppets_scene(); //glass_air_bubble_exact_scene(); //

    std::string ppm_data = canvas.to_ppm();
    std::ofstream outputFile("groups.ppm");
    assert(outputFile.is_open());
    outputFile << ppm_data;
    return 0;
}

Canvas group_scene() {
    auto hexagon_corner = []() {
        auto corner_u = std::make_unique<Sphere>();
        corner_u.get()->transform = Transform::translation(0, 0, -1) *
            Transform::scaling(.25, .25, .25);
        return corner_u;
        };

    auto hexagon_edge = []() {
        auto edge_u = std::make_unique<Cylinder>();
        Cylinder* edge = edge_u.get();
        edge->minimum = 0;
        edge->maximum = 1;
        edge->transform = Transform::translation(0, 0, -1) *
            Transform::rotation_y(-M_PI / 6) *
            Transform::rotation_z(-M_PI / 2) *
            Transform::scaling(.25, 1, .25);
        return edge_u;
        };

    auto hexagon_side = [hexagon_corner, hexagon_edge]() {
        auto side_u = std::make_unique<Group>();
        side_u.get()->add_child(std::move(hexagon_corner()));
        side_u.get()->add_child(std::move(hexagon_edge()));
        return side_u;
        };

    auto hexagon = [hexagon_side]() {
        auto hex = std::make_unique<Group>();
        for (int n = 0; n < 6; n++) {
            auto side = hexagon_side();
            side.get()->transform = Transform::rotation_y(n * M_PI / 3);
            hex.get()->add_child(std::move(side));
        }
        return hex;
        };

    auto light_u = std::make_unique<PointLight>(Point(2, 10, -5), Color(.9, .9, .9));
    Camera camera(300, 300, .45);
    camera.transform = Transform::view_transform(
        Point(2, 3, -7), Point(0, 0, 0), Vector(0, 1, 0));

    World w;
    w.light = std::move(light_u);
    auto hex_model_u = hexagon();
    w.objects.emplace(hex_model_u.get(), std::move(hex_model_u));
    return camera.render(&w);
}

Canvas cone_scene() {
    Camera camera(600, 300, .314);
    camera.transform = Transform::view_transform(
        Point(7, 4, -8), Point(0, .3, 0), Vector(0, 1, 0));

    auto light_u = std::make_unique<PointLight>(Point(1, 6.9, -4.9), Color(1, 1, 1));

    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    CheckerPattern floor_pattern(Color(.5, .5, .5), Color(.75, .75, .75));
    floor_pattern.transform = Transform::rotation_y(.3) * Transform::scaling(.25, .25, .25);
    floor->material.pattern = &floor_pattern;
    floor->material.specular = 0;
    floor->material.ambient = .2;
    floor->material.diffuse = .9;

    // concentric cylinders
    auto concentric_1_u = std::make_unique<Cone>();
    Cone* concentric_1 = concentric_1_u.get();
    concentric_1->minimum = 0;
    concentric_1->maximum = .2;
    concentric_1->closed = false;
    concentric_1->transform = Transform::translation(1, 0, 0) * Transform::scaling(.8, 1, .8);
    concentric_1->material.color = Color(1, 1, .3);
    concentric_1->material.ambient = .1;
    concentric_1->material.diffuse = .8;
    concentric_1->material.specular = .9;
    concentric_1->material.shininess = 300;

    auto concentric_2_u = std::make_unique<Cone>();
    Cone* concentric_2 = concentric_2_u.get();
    concentric_2->minimum = 0;
    concentric_2->maximum = .3;
    concentric_2->closed = false;
    concentric_2->transform = Transform::translation(1, 0, 0) * Transform::scaling(.6, 1, .6);
    concentric_2->material.color = Color(1, .9, .4);
    concentric_2->material.ambient = .1;
    concentric_2->material.diffuse = .8;
    concentric_2->material.shininess = 300;
    concentric_2->material.specular = .9;

    auto concentric_3_u = std::make_unique<Cone>();
    Cone* concentric_3 = concentric_3_u.get();
    concentric_3->minimum = 0;
    concentric_3->maximum = .4;
    concentric_3->closed = false;
    concentric_3->transform = Transform::translation(1, 0, 0) * Transform::scaling(.4, 1, .4);
    concentric_3->material.color = Color(1, .8, .5);
    concentric_3->material.ambient = .1;
    concentric_3->material.diffuse = .8;
    concentric_3->material.shininess = 300;
    concentric_3->material.specular = .9;

    auto concentric_4_u = std::make_unique<Cone>();
    Cone* concentric_4 = concentric_4_u.get();
    concentric_4->minimum = 0;
    concentric_4->maximum = .5;
    concentric_4->closed = true;
    concentric_4->transform = Transform::translation(1, 0, 0) * Transform::scaling(.2, 1, .2);
    concentric_4->material.color = Color(1, .7, .6);
    concentric_4->material.ambient = .1;
    concentric_4->material.diffuse = .8;
    concentric_4->material.shininess = 300;
    concentric_4->material.specular = .9;

    // Decorative cylinders
    auto decorative_1_u = std::make_unique<Cone>();
    Cone* decorative_1 = decorative_1_u.get();
    decorative_1->minimum = 0;
    decorative_1->maximum = .3;
    decorative_1->closed = true;
    decorative_1->transform = Transform::translation(0, 0, -.75) * Transform::scaling(.05, 1, .05);
    decorative_1->material.color = Color(1, 0, 0);
    decorative_1->material.ambient = .1;
    decorative_1->material.diffuse = .9;
    decorative_1->material.shininess = 300;
    decorative_1->material.specular = .9;

    auto decorative_2_u = std::make_unique<Cone>();
    Cone* decorative_2 = decorative_2_u.get();
    decorative_2->minimum = 0;
    decorative_2->maximum = .3;
    decorative_2->closed = true;
    decorative_2->transform = Transform::translation(0, 0, -2.25) * Transform::rotation_y(-.15) * Transform::translation(0, 0, 1.5) * Transform::scaling(.05, 1, .05);
    decorative_2->material.color = Color(1, 1, 0);
    decorative_2->material.ambient = .1;
    decorative_2->material.diffuse = .9;
    decorative_2->material.shininess = 300;
    decorative_2->material.specular = .9;

    auto decorative_3_u = std::make_unique<Cone>();
    Cone* decorative_3 = decorative_3_u.get();
    decorative_3->minimum = 0;
    decorative_3->maximum = .3;
    decorative_3->closed = true;
    decorative_3->transform = Transform::translation(0, 0, -2.25) * Transform::rotation_y(-.3) * Transform::translation(0, 0, 1.5) * Transform::scaling(.05, 1, .05);
    decorative_3->material.color = Color(0, 1, 0);
    decorative_3->material.ambient = .1;
    decorative_3->material.diffuse = .9;
    decorative_3->material.shininess = 300;
    decorative_3->material.specular = .9;

    auto decorative_4_u = std::make_unique<Cone>();
    Cone* decorative_4 = decorative_4_u.get();
    decorative_4->minimum = 0;
    decorative_4->maximum = .3;
    decorative_4->closed = true;
    decorative_4->transform = Transform::translation(0, 0, -2.25) * Transform::rotation_y(-.45) * Transform::translation(0, 0, 1.5) * Transform::scaling(.05, 1, .05);
    decorative_4->material.color = Color(0, 1, 1);
    decorative_4->material.ambient = .1;
    decorative_4->material.diffuse = .9;
    decorative_4->material.shininess = 300;
    decorative_4->material.specular = .9;


    auto glass_u = std::make_unique<Cone>();
    Cone* glass = glass_u.get();
    glass->minimum = 0.0001;
    glass->maximum = .5;
    glass->closed = true;
    glass->transform = Transform::translation(0, 0, -1.5) * Transform::scaling(.33, 1, .33);
    glass->material.color = Color(.25, 0, 0);
    glass->material.diffuse = .1;
    glass->material.shininess = 300;
    glass->material.specular = .9;
    glass->material.transparency = .9;
    glass->material.refractive_index = 1.5;

    auto reflective_u = std::make_unique<Cone>();
    Cone* reflective = reflective_u.get();
    reflective->minimum = 0;
    reflective->maximum = .75;
    reflective->closed = true;
    reflective->transform = Transform::translation(-1, 0, 1) * Transform::scaling(.5, 1, .5);
    reflective->material.color = Color(0, 0, 0.6); // TODO: why did this render so red
    reflective->material.diffuse = .1;
    reflective->material.shininess = 300;
    reflective->material.specular = .9;
    reflective->material.reflective = .9;

    World w;

    w.objects.emplace(floor, std::move(floor_u));
    w.objects.emplace(concentric_1, std::move(concentric_1_u));
    // w.objects.emplace(concentric_2, std::move(concentric_2_u));
    // w.objects.emplace(concentric_3, std::move(concentric_3_u));
    // w.objects.emplace(concentric_4, std::move(concentric_4_u));
    // w.objects.emplace(decorative_1, std::move(decorative_1_u));
    // w.objects.emplace(decorative_2, std::move(decorative_2_u));
    // w.objects.emplace(decorative_3, std::move(decorative_3_u));
    // w.objects.emplace(decorative_4, std::move(decorative_4_u));
    // w.objects.emplace(glass, std::move(glass_u));
    // w.objects.emplace(reflective, std::move(reflective_u));
    w.light = std::move(light_u);

    return camera.render(&w);
}

Canvas cylinder_scene() {
    Camera camera(400, 200, .314);
    camera.transform = Transform::view_transform(
        Point(8, 3.5, -9), Point(0, .3, 0), Vector(0, 1, 0));

    auto light_u = std::make_unique<PointLight>(Point(1, 6.9, -4.9), Color(1, 1, 1));

    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    CheckerPattern floor_pattern(Color(.5, .5, .5), Color(.75, .75, .75));
    floor_pattern.transform = Transform::rotation_y(.3) * Transform::scaling(.25, .25, .25);
    floor->material.pattern = &floor_pattern;
    floor->material.specular = 0;
    floor->material.ambient = .2;
    floor->material.diffuse = .9;

    // concentric cylinders
    auto concentric_1_u = std::make_unique<Cylinder>();
    Cylinder* concentric_1 = concentric_1_u.get();
    concentric_1->minimum = 0;
    concentric_1->maximum = .2;
    concentric_1->closed = false;
    concentric_1->transform = Transform::translation(1, 0, 0) * Transform::scaling(.8, 1, .8);
    concentric_1->material.color = Color(1, 1, .3);
    concentric_1->material.ambient = .1;
    concentric_1->material.diffuse = .8;
    concentric_1->material.specular = .9;
    concentric_1->material.shininess = 300;

    auto concentric_2_u = std::make_unique<Cylinder>();
    Cylinder* concentric_2 = concentric_2_u.get();
    concentric_2->minimum = 0;
    concentric_2->maximum = .3;
    concentric_2->closed = false;
    concentric_2->transform = Transform::translation(1, 0, 0) * Transform::scaling(.6, 1, .6);
    concentric_2->material.color = Color(1, .9, .4);
    concentric_2->material.ambient = .1;
    concentric_2->material.diffuse = .8;
    concentric_2->material.shininess = 300;
    concentric_2->material.specular = .9;

    auto concentric_3_u = std::make_unique<Cylinder>();
    Cylinder* concentric_3 = concentric_3_u.get();
    concentric_3->minimum = 0;
    concentric_3->maximum = .4;
    concentric_3->closed = false;
    concentric_3->transform = Transform::translation(1, 0, 0) * Transform::scaling(.4, 1, .4);
    concentric_3->material.color = Color(1, .8, .5);
    concentric_3->material.ambient = .1;
    concentric_3->material.diffuse = .8;
    concentric_3->material.shininess = 300;
    concentric_3->material.specular = .9;

    auto concentric_4_u = std::make_unique<Cylinder>();
    Cylinder* concentric_4 = concentric_4_u.get();
    concentric_4->minimum = 0;
    concentric_4->maximum = .5;
    concentric_4->closed = true;
    concentric_4->transform = Transform::translation(1, 0, 0) * Transform::scaling(.2, 1, .2);
    concentric_4->material.color = Color(1, .7, .6);
    concentric_4->material.ambient = .1;
    concentric_4->material.diffuse = .8;
    concentric_4->material.shininess = 300;
    concentric_4->material.specular = .9;

    // Decorative cylinders
    auto decorative_1_u = std::make_unique<Cylinder>();
    Cylinder* decorative_1 = decorative_1_u.get();
    decorative_1->minimum = 0;
    decorative_1->maximum = .3;
    decorative_1->closed = true;
    decorative_1->transform = Transform::translation(0, 0, -.75) * Transform::scaling(.05, 1, .05);
    decorative_1->material.color = Color(1, 0, 0);
    decorative_1->material.ambient = .1;
    decorative_1->material.diffuse = .9;
    decorative_1->material.shininess = 300;
    decorative_1->material.specular = .9;

    auto decorative_2_u = std::make_unique<Cylinder>();
    Cylinder* decorative_2 = decorative_2_u.get();
    decorative_2->minimum = 0;
    decorative_2->maximum = .3;
    decorative_2->closed = true;
    decorative_2->transform = Transform::translation(0, 0, -2.25) * Transform::rotation_y(-.15) * Transform::translation(0, 0, 1.5) * Transform::scaling(.05, 1, .05);
    decorative_2->material.color = Color(1, 1, 0);
    decorative_2->material.ambient = .1;
    decorative_2->material.diffuse = .9;
    decorative_2->material.shininess = 300;
    decorative_2->material.specular = .9;

    auto decorative_3_u = std::make_unique<Cylinder>();
    Cylinder* decorative_3 = decorative_3_u.get();
    decorative_3->minimum = 0;
    decorative_3->maximum = .3;
    decorative_3->closed = true;
    decorative_3->transform = Transform::translation(0, 0, -2.25) * Transform::rotation_y(-.3) * Transform::translation(0, 0, 1.5) * Transform::scaling(.05, 1, .05);
    decorative_3->material.color = Color(0, 1, 0);
    decorative_3->material.ambient = .1;
    decorative_3->material.diffuse = .9;
    decorative_3->material.shininess = 300;
    decorative_3->material.specular = .9;

    auto decorative_4_u = std::make_unique<Cylinder>();
    Cylinder* decorative_4 = decorative_4_u.get();
    decorative_4->minimum = 0;
    decorative_4->maximum = .3;
    decorative_4->closed = true;
    decorative_4->transform = Transform::translation(0, 0, -2.25) * Transform::rotation_y(-.45) * Transform::translation(0, 0, 1.5) * Transform::scaling(.05, 1, .05);
    decorative_4->material.color = Color(0, 1, 1);
    decorative_4->material.ambient = .1;
    decorative_4->material.diffuse = .9;
    decorative_4->material.shininess = 300;
    decorative_4->material.specular = .9;


    auto glass_u = std::make_unique<Cylinder>();
    Cylinder* glass = glass_u.get();
    glass->minimum = 0.0001;
    glass->maximum = .5;
    glass->closed = true;
    glass->transform = Transform::translation(0, 0, -1.5) * Transform::scaling(.33, 1, .33);
    glass->material.color = Color(.25, 0, 0);
    glass->material.diffuse = .1;
    glass->material.shininess = 300;
    glass->material.specular = .9;
    glass->material.transparency = .9;
    glass->material.refractive_index = 1.5;

    auto reflective_u = std::make_unique<Cylinder>();
    Cylinder* reflective = reflective_u.get();
    reflective->minimum = 0;
    reflective->maximum = .75;
    reflective->closed = true;
    reflective->transform = Transform::translation(-1, 0, 1) * Transform::scaling(.5, 1, .5);
    reflective->material.color = Color(0, 0, 0.6); // TODO: why did this render so red
    reflective->material.diffuse = .1;
    reflective->material.shininess = 300;
    reflective->material.specular = .9;
    reflective->material.reflective = .9;

    World w;

    w.objects.emplace(floor, std::move(floor_u));
    w.objects.emplace(concentric_1, std::move(concentric_1_u));
    w.objects.emplace(concentric_2, std::move(concentric_2_u));
    w.objects.emplace(concentric_3, std::move(concentric_3_u));
    w.objects.emplace(concentric_4, std::move(concentric_4_u));
    w.objects.emplace(decorative_1, std::move(decorative_1_u));
    w.objects.emplace(decorative_2, std::move(decorative_2_u));
    w.objects.emplace(decorative_3, std::move(decorative_3_u));
    w.objects.emplace(decorative_4, std::move(decorative_4_u));
    w.objects.emplace(glass, std::move(glass_u));
    w.objects.emplace(reflective, std::move(reflective_u));
    w.light = std::move(light_u);

    return camera.render(&w);
}

Canvas reflection_and_refraction_cube_scene() {
    Camera camera(400, 200, 1.152);
    camera.transform = Transform::view_transform(
        Point(-2.6, 1.5, -3.9), Point(-.6, 1, -.8), Vector(0, 1, 0));

    Material wall_material;
    StripePattern wall_pattern(Color(.45, .45, .45), Color(.55, .55, .55));
    wall_pattern.transform = Transform::rotation_y(M_PI / 2) * Transform::scaling(.25, .25, .25);
    wall_material.ambient = 0;
    wall_material.diffuse = .4;
    wall_material.specular = 0;
    wall_material.reflective = .3;
    wall_material.pattern = &wall_pattern;

    auto light_u = std::make_unique<PointLight>(Point(-4.9, 4.9, -1), Color(1, 1, 1));

    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    floor->transform = Transform::rotation_y(M_PI);
    CheckerPattern floor_pattern(Color(.35, .35, .35), Color(.65, .65, .65));
    floor->material.pattern = &floor_pattern;
    floor->material.specular = 0;
    floor->material.reflective = .4;

    auto ceiling_u = std::make_unique<Plane>();
    Plane* ceiling = ceiling_u.get();
    ceiling->transform = Transform::translation(0, 5, 0);
    ceiling->material.color = Color(.8, .8, .8);
    ceiling->material.ambient = .3;
    ceiling->material.specular = 0;

    auto west_wall_u = std::make_unique<Plane>();
    Plane* west_wall = west_wall_u.get();
    west_wall->transform = Transform::translation(-5, 0, 0) * Transform::rotation_z(M_PI / 2) * Transform::rotation_y(M_PI / 2);
    west_wall->material = wall_material;

    auto east_wall_u = std::make_unique<Plane>();
    Plane* east_wall = east_wall_u.get();
    east_wall->transform = Transform::translation(5, 0, 0) * Transform::rotation_z(M_PI / 2) * Transform::rotation_y(M_PI / 2);
    east_wall->material = wall_material;

    auto north_wall_u = std::make_unique<Plane>();
    Plane* north_wall = north_wall_u.get();
    north_wall->transform = Transform::translation(0, 0, 5) * Transform::rotation_x(M_PI / 2);
    north_wall->material = wall_material;

    auto south_wall_u = std::make_unique<Plane>();
    Plane* south_wall = south_wall_u.get();
    south_wall->transform = Transform::translation(0, 0, -5) * Transform::rotation_x(M_PI / 2);
    south_wall->material = wall_material;

    // Background balls
    auto b_sphere_1_u = std::make_unique<Cube>();
    Cube* b_sphere_1 = b_sphere_1_u.get();
    b_sphere_1->transform = Transform::translation(4.6, .4, 1) * Transform::scaling(.4, .4, .4);
    b_sphere_1->material.color = Color(.8, .5, .3);
    b_sphere_1->material.shininess = 50;

    auto b_sphere_2_u = std::make_unique<Cube>();
    Cube* b_sphere_2 = b_sphere_2_u.get();
    b_sphere_2->transform = Transform::translation(4.7, .3, .4) * Transform::scaling(.3, .3, .3);
    b_sphere_2->material.color = Color(.9, .4, .5);
    b_sphere_2->material.shininess = 50;

    auto b_sphere_3_u = std::make_unique<Cube>();
    Cube* b_sphere_3 = b_sphere_3_u.get();
    b_sphere_3->transform = Transform::translation(-1, .5, 4.5) * Transform::scaling(.5, .5, .5);
    b_sphere_3->material.color = Color(.4, .9, .6);
    b_sphere_3->material.shininess = 50;

    auto b_sphere_4_u = std::make_unique<Cube>();
    Cube* b_sphere_4 = b_sphere_4_u.get();
    b_sphere_4->transform = Transform::translation(-1.7, .3, 4.7) * Transform::scaling(.3, .3, .3);
    b_sphere_4->material.color = Color(.4, .6, .9);
    b_sphere_4->material.shininess = 50;

    // Foreground balls
    auto red_sphere_u = std::make_unique<Cube>();
    Cube* red_sphere = red_sphere_u.get();
    red_sphere->transform = Transform::translation(-.6, 1, .6);
    red_sphere->material.color = Color(1, .3, .2);
    red_sphere->material.specular = .4;
    red_sphere->material.shininess = 5;

    auto blue_sphere_u = std::make_unique<Cube>();
    Cube* blue_sphere = blue_sphere_u.get();
    blue_sphere->transform = Transform::translation(.6, .7, -.6) * Transform::scaling(.7, .7, .7);
    blue_sphere->material.color = Color(0, 0, .2);
    blue_sphere->material.ambient = 0;
    blue_sphere->material.diffuse = .4;
    blue_sphere->material.specular = .9;
    red_sphere->material.shininess = 300;
    blue_sphere->material.reflective = .9;
    blue_sphere->material.transparency = .9;
    blue_sphere->material.refractive_index = 1.5;

    auto green_sphere_u = std::make_unique<Cube>();
    Cube* green_sphere = green_sphere_u.get();
    green_sphere->transform = Transform::translation(-.7, .5, -.8) * Transform::scaling(.5, .5, .5);
    green_sphere->material.color = Color(0, .2, 0);
    green_sphere->material.ambient = 0;
    green_sphere->material.diffuse = .4;
    green_sphere->material.specular = .9;
    green_sphere->material.shininess = 300;
    green_sphere->material.reflective = .9;
    green_sphere->material.transparency = .9;
    green_sphere->material.refractive_index = 1.5;

    World w;

    w.objects.emplace(floor, std::move(floor_u));
    w.objects.emplace(ceiling, std::move(ceiling_u));
    w.objects.emplace(north_wall, std::move(north_wall_u));
    w.objects.emplace(west_wall, std::move(west_wall_u));
    w.objects.emplace(east_wall, std::move(east_wall_u));
    w.objects.emplace(south_wall, std::move(south_wall_u));
    w.objects.emplace(b_sphere_1, std::move(b_sphere_1_u));
    w.objects.emplace(b_sphere_2, std::move(b_sphere_2_u));
    w.objects.emplace(b_sphere_3, std::move(b_sphere_3_u));
    w.objects.emplace(b_sphere_4, std::move(b_sphere_4_u));
    w.objects.emplace(red_sphere, std::move(red_sphere_u));
    w.objects.emplace(blue_sphere, std::move(blue_sphere_u));
    w.objects.emplace(green_sphere, std::move(green_sphere_u));
    w.light = std::move(light_u);

    return camera.render(&w);
}

Canvas glass_air_cube_scene() {
    Camera camera(300, 300, .45);
    camera.transform = Transform::view_transform(
        Point(1, 1, -5), Point(0, 0, 0), Vector(0, 1, 0));
    // Point(0, 0, -5), Point(0, 0, 0), Vector(0, 1, 0));

    auto light_u = std::make_unique<PointLight>(Point(2, 10, -5), Color(.9, .9, .9));

    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    floor->transform = Transform::translation(0, 0, 10) * Transform::rotation_x(M_PI / 2);
    CheckerPattern floor_pattern(Color(.15, .15, .15), Color(.85, .85, .85));
    floor->material.pattern = &floor_pattern;
    floor->material.ambient = .8;
    floor->material.diffuse = .2;
    floor->material.specular = 0;

    auto glass_sphere_u = std::make_unique<Cube>();
    Cube* glass_sphere = glass_sphere_u.get();
    glass_sphere->transform = Transform::scaling(.5, .5, .5);
    glass_sphere->material.color = Color(1, 0, 0);
    glass_sphere->material.ambient = 0;
    glass_sphere->material.diffuse = 0;
    glass_sphere->material.specular = .9;
    glass_sphere->material.shininess = 300;
    glass_sphere->material.reflective = .9;
    glass_sphere->material.transparency = .9;
    glass_sphere->material.refractive_index = 1.5;

    auto inner_air_sphere_u = std::make_unique<Cube>();
    Cube* inner_air_sphere = inner_air_sphere_u.get();
    inner_air_sphere->transform = Transform::scaling(.25, .25, .25);
    inner_air_sphere->material.ambient = 0;
    inner_air_sphere->material.diffuse = 0;
    inner_air_sphere->material.specular = 0.9;
    inner_air_sphere->material.shininess = 300;
    inner_air_sphere->material.reflective = .9;
    inner_air_sphere->material.transparency = .9;
    inner_air_sphere->material.refractive_index = 1.0000034;
    inner_air_sphere->material.color = Color(1, 1, 1);

    World w;
    w.objects.emplace(floor, std::move(floor_u));
    w.objects.emplace(glass_sphere, std::move(glass_sphere_u));
    w.objects.emplace(inner_air_sphere, std::move(inner_air_sphere_u));
    std::cout << "Hollow glass cube\n";
    w.light = std::move(light_u);

    return camera.render(&w);
}

Canvas glass_air_bubble_exact_scene() {
    Camera camera(300, 300, .45);
    camera.transform = Transform::view_transform(
        Point(0, 0, -5), Point(0, 0, 0), Vector(0, 1, 0));

    auto light_u = std::make_unique<PointLight>(Point(2, 10, -5), Color(.9, .9, .9));

    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    floor->transform = Transform::translation(0, 0, 10) * Transform::rotation_x(M_PI / 2);
    CheckerPattern floor_pattern(Color(.15, .15, .15), Color(.85, .85, .85));
    floor->material.pattern = &floor_pattern;
    floor->material.ambient = .8;
    floor->material.diffuse = .2;
    floor->material.specular = 0;

    auto glass_sphere_u = std::make_unique<Sphere>();
    Sphere* glass_sphere = glass_sphere_u.get();
    glass_sphere->material.color = Color(1, 1, 1);
    glass_sphere->material.ambient = 0;
    glass_sphere->material.diffuse = 0;
    glass_sphere->material.specular = .9;
    glass_sphere->material.shininess = 300;
    glass_sphere->material.reflective = .9;
    glass_sphere->material.transparency = .9;
    glass_sphere->material.refractive_index = 1.5;

    auto inner_air_sphere_u = std::make_unique<Sphere>();
    Sphere* inner_air_sphere = inner_air_sphere_u.get();
    inner_air_sphere->transform = Transform::scaling(.5, .5, .5);
    inner_air_sphere->material.ambient = 0;
    inner_air_sphere->material.diffuse = 0;
    inner_air_sphere->material.specular = 0.9;
    inner_air_sphere->material.shininess = 300;
    inner_air_sphere->material.reflective = .9;
    inner_air_sphere->material.transparency = .9;
    inner_air_sphere->material.refractive_index = 1.0000034;
    inner_air_sphere->material.color = Color(1, 1, 1);

    World w;
    w.objects.emplace(floor, std::move(floor_u));
    w.objects.emplace(glass_sphere, std::move(glass_sphere_u));
    w.objects.emplace(inner_air_sphere, std::move(inner_air_sphere_u));
    std::cout << "Hollow glass - no reflections - colors 1\n";
    w.light = std::move(light_u);

    return camera.render(&w);
}

Canvas glass_air_bubble_forum_scene() {
    Camera camera(512, 512, M_PI / 3);
    camera.transform = Transform::view_transform(
        Point(0, 2.5, 0), Point(0, 0, 0), Vector(1, 0, 0));

    auto light_u = std::make_unique<PointLight>(Point(2, 10, -5), Color(.9, .9, .9));

    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    floor->transform = Transform::translation(0, -5, 0);//(0, -10, 0);
    CheckerPattern floor_pattern(Color(0, 0, 0), Color(1, 1, 1));
    floor->material.pattern = &floor_pattern;

    auto glass_sphere_u = std::make_unique<Sphere>();
    Sphere* glass_sphere = glass_sphere_u.get();
    glass_sphere->material.diffuse = .1;
    glass_sphere->material.shininess = 300;
    glass_sphere->material.reflective = 0;//1;
    glass_sphere->material.transparency = 1;
    glass_sphere->material.refractive_index = 1;//.52;
    glass_sphere->material.color = Color(0, 0, .1);

    auto inner_air_sphere_u = std::make_unique<Sphere>();
    Sphere* inner_air_sphere = inner_air_sphere_u.get();
    inner_air_sphere->transform = Transform::scaling(.5, .5, .5);
    inner_air_sphere->material.diffuse = .1;
    inner_air_sphere->material.shininess = 300;
    inner_air_sphere->material.reflective = 1;
    inner_air_sphere->material.transparency = 1;
    inner_air_sphere->material.refractive_index = 1;
    inner_air_sphere->material.color = Color(0, 0, .1);

    World w;
    w.objects.emplace(floor, std::move(floor_u));
    w.objects.emplace(glass_sphere, std::move(glass_sphere_u));
    // w.objects.emplace(inner_air_sphere, std::move(inner_air_sphere_u));
    w.light = std::move(light_u);

    return camera.render(&w);
}

Canvas shadow_puppets_scene() {
    Camera camera(400, 200, .524);
    camera.transform = Transform::view_transform(
        Point(40, 0, -70), Point(0, 0, -5), Vector(0, 1, 0));

    auto light_u = std::make_unique<PointLight>(Point(0, 0, -100), Color(1, 1, 1));

    // Shadow Backdrop
    Material sphere_material;
    sphere_material.ambient = .2;
    sphere_material.diffuse = .8;
    sphere_material.specular = .3;
    sphere_material.shininess = 200;

    auto sphere_u = std::make_unique<Sphere>();
    Sphere* sphere = sphere_u.get();
    sphere->material.color = Color(1, 1, 1);
    sphere->material.ambient = 0;
    sphere->material.diffuse = .5;
    sphere->material.specular = 0;
    sphere->transform = Transform::translation(0, 0, 20) * Transform::scaling(200, 200, .01);

    auto wrist_u = std::make_unique<Sphere>();
    Sphere* wrist = wrist_u.get();
    wrist->material = sphere_material;
    wrist->material.color = Color(.1, 1, 1);
    wrist->transform = Transform::rotation_z(M_PI / 4) * Transform::translation(-4, 0, -21) * Transform::scaling(3, 3, 3);

    auto palm_u = std::make_unique<Sphere>();
    Sphere* palm = palm_u.get();
    palm->material = sphere_material;
    palm->material.color = Color(.1, .1, 1);
    palm->transform = Transform::translation(0, 0, -15) * Transform::scaling(4, 3, 3);

    auto thumb_u = std::make_unique<Sphere>();
    Sphere* thumb = thumb_u.get();
    thumb->material = sphere_material;
    thumb->material.color = Color(.1, .1, 1);
    thumb->transform = Transform::translation(-2, 2, -16) * Transform::scaling(1, 3, 1);

    auto index_u = std::make_unique<Sphere>();
    Sphere* index = index_u.get();
    index->material = sphere_material;
    index->material.color = Color(1, 1, .1);
    index->transform = Transform::translation(3, 2, -22) * Transform::scaling(3, .75, .75);

    auto middle_u = std::make_unique<Sphere>();
    Sphere* middle = middle_u.get();
    middle->material = sphere_material;
    middle->material.color = Color(.1, 1, .5);
    middle->transform = Transform::translation(4, 1, -19) * Transform::scaling(3, .75, .75);

    auto ring_u = std::make_unique<Sphere>();
    Sphere* ring = ring_u.get();
    ring->material = sphere_material;
    ring->material.color = Color(.1, 1, .1);
    ring->transform = Transform::translation(4, 0, -18) * Transform::scaling(3, .75, .75);

    auto pinky_u = std::make_unique<Sphere>();
    Sphere* pinky = pinky_u.get();
    pinky->material = sphere_material;
    pinky->material.color = Color(.1, .5, 1);
    pinky->transform = Transform::translation(3, -1.5, -20) * Transform::rotation_z(-M_PI / 10) * Transform::translation(1, 0, 0) * Transform::scaling(2.5, .6, .6);

    World w;

    w.objects.emplace(sphere, std::move(sphere_u));
    w.objects.emplace(wrist, std::move(wrist_u));
    w.objects.emplace(palm, std::move(palm_u));
    w.objects.emplace(thumb, std::move(thumb_u));
    w.objects.emplace(index, std::move(index_u));
    w.objects.emplace(middle, std::move(middle_u));
    w.objects.emplace(ring, std::move(ring_u));
    w.objects.emplace(pinky, std::move(pinky_u));
    w.light = std::move(light_u);

    return camera.render(&w);
}

Canvas reflection_and_refraction_scene() {
    Camera camera(400, 200, 1.152);
    camera.transform = Transform::view_transform(
        Point(-2.6, 1.5, -3.9), Point(-.6, 1, -.8), Vector(0, 1, 0));

    Material wall_material;
    StripePattern wall_pattern(Color(.45, .45, .45), Color(.55, .55, .55));
    wall_pattern.transform = Transform::rotation_y(M_PI / 2) * Transform::scaling(.25, .25, .25);
    wall_material.ambient = 0;
    wall_material.diffuse = .4;
    wall_material.specular = 0;
    wall_material.reflective = .3;
    wall_material.pattern = &wall_pattern;

    auto light_u = std::make_unique<PointLight>(Point(-4.9, 4.9, -1), Color(1, 1, 1));

    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    floor->transform = Transform::rotation_y(M_PI);
    CheckerPattern floor_pattern(Color(.35, .35, .35), Color(.65, .65, .65));
    floor->material.pattern = &floor_pattern;
    floor->material.specular = 0;
    floor->material.reflective = .4;

    auto ceiling_u = std::make_unique<Plane>();
    Plane* ceiling = ceiling_u.get();
    ceiling->transform = Transform::translation(0, 5, 0);
    ceiling->material.color = Color(.8, .8, .8);
    ceiling->material.ambient = .3;
    ceiling->material.specular = 0;

    auto west_wall_u = std::make_unique<Plane>();
    Plane* west_wall = west_wall_u.get();
    west_wall->transform = Transform::translation(-5, 0, 0) * Transform::rotation_z(M_PI / 2) * Transform::rotation_y(M_PI / 2);
    west_wall->material = wall_material;

    auto east_wall_u = std::make_unique<Plane>();
    Plane* east_wall = east_wall_u.get();
    east_wall->transform = Transform::translation(5, 0, 0) * Transform::rotation_z(M_PI / 2) * Transform::rotation_y(M_PI / 2);
    east_wall->material = wall_material;

    auto north_wall_u = std::make_unique<Plane>();
    Plane* north_wall = north_wall_u.get();
    north_wall->transform = Transform::translation(0, 0, 5) * Transform::rotation_x(M_PI / 2);
    north_wall->material = wall_material;

    auto south_wall_u = std::make_unique<Plane>();
    Plane* south_wall = south_wall_u.get();
    south_wall->transform = Transform::translation(0, 0, -5) * Transform::rotation_x(M_PI / 2);
    south_wall->material = wall_material;

    // Background balls
    auto b_sphere_1_u = std::make_unique<Sphere>();
    Sphere* b_sphere_1 = b_sphere_1_u.get();
    b_sphere_1->transform = Transform::translation(4.6, .4, 1) * Transform::scaling(.4, .4, .4);
    b_sphere_1->material.color = Color(.8, .5, .3);
    b_sphere_1->material.shininess = 50;

    auto b_sphere_2_u = std::make_unique<Sphere>();
    Sphere* b_sphere_2 = b_sphere_2_u.get();
    b_sphere_2->transform = Transform::translation(4.7, .3, .4) * Transform::scaling(.3, .3, .3);
    b_sphere_2->material.color = Color(.9, .4, .5);
    b_sphere_2->material.shininess = 50;

    auto b_sphere_3_u = std::make_unique<Sphere>();
    Sphere* b_sphere_3 = b_sphere_3_u.get();
    b_sphere_3->transform = Transform::translation(-1, .5, 4.5) * Transform::scaling(.5, .5, .5);
    b_sphere_3->material.color = Color(.4, .9, .6);
    b_sphere_3->material.shininess = 50;

    auto b_sphere_4_u = std::make_unique<Sphere>();
    Sphere* b_sphere_4 = b_sphere_4_u.get();
    b_sphere_4->transform = Transform::translation(-1.7, .3, 4.7) * Transform::scaling(.3, .3, .3);
    b_sphere_4->material.color = Color(.4, .6, .9);
    b_sphere_4->material.shininess = 50;

    // Foreground balls
    auto red_sphere_u = std::make_unique<Sphere>();
    Sphere* red_sphere = red_sphere_u.get();
    red_sphere->transform = Transform::translation(-.6, 1, .6);
    red_sphere->material.color = Color(1, .3, .2);
    red_sphere->material.specular = .4;
    red_sphere->material.shininess = 5;

    auto blue_sphere_u = std::make_unique<Sphere>();
    Sphere* blue_sphere = blue_sphere_u.get();
    blue_sphere->transform = Transform::translation(.6, .7, -.6) * Transform::scaling(.7, .7, .7);
    blue_sphere->material.color = Color(0, 0, .2);
    blue_sphere->material.ambient = 0;
    blue_sphere->material.diffuse = .4;
    blue_sphere->material.specular = .9;
    red_sphere->material.shininess = 300;
    blue_sphere->material.reflective = .9;
    blue_sphere->material.transparency = .9;
    blue_sphere->material.refractive_index = 1.5;

    auto green_sphere_u = std::make_unique<Sphere>();
    Sphere* green_sphere = green_sphere_u.get();
    green_sphere->transform = Transform::translation(-.7, .5, -.8) * Transform::scaling(.5, .5, .5);
    green_sphere->material.color = Color(0, .2, 0);
    green_sphere->material.ambient = 0;
    green_sphere->material.diffuse = .4;
    green_sphere->material.specular = .9;
    green_sphere->material.shininess = 300;
    green_sphere->material.reflective = .9;
    green_sphere->material.transparency = .9;
    green_sphere->material.refractive_index = 1.5;

    World w;

    w.objects.emplace(floor, std::move(floor_u));
    w.objects.emplace(ceiling, std::move(ceiling_u));
    w.objects.emplace(north_wall, std::move(north_wall_u));
    w.objects.emplace(west_wall, std::move(west_wall_u));
    w.objects.emplace(east_wall, std::move(east_wall_u));
    w.objects.emplace(south_wall, std::move(south_wall_u));
    w.objects.emplace(b_sphere_1, std::move(b_sphere_1_u));
    w.objects.emplace(b_sphere_2, std::move(b_sphere_2_u));
    w.objects.emplace(b_sphere_3, std::move(b_sphere_3_u));
    w.objects.emplace(b_sphere_4, std::move(b_sphere_4_u));
    w.objects.emplace(red_sphere, std::move(red_sphere_u));
    w.objects.emplace(blue_sphere, std::move(blue_sphere_u));
    w.objects.emplace(green_sphere, std::move(green_sphere_u));
    w.light = std::move(light_u);

    return camera.render(&w);
}