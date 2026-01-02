#include "../src/rendering/camera.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <iostream>

TEST_CASE("Constructing a camera", "[scene][camera]") {
    size_t hsize = 160;
    size_t vsize = 120;
    double fov = M_PI / 2;
    Camera c(hsize, vsize, fov);

    REQUIRE(c.hsize == 160);
    REQUIRE(c.vsize == 120);
    REQUIRE(double_equal(c.fov, M_PI / 2));
    REQUIRE(c.transform == identity_matrix4);
}

TEST_CASE("The pixel size for a horizontal canvas", "[scene][camera]") {
    Camera c(200, 125, M_PI / 2);
    REQUIRE(double_equal(c.pixel_size, 0.01));
}

TEST_CASE("The pixel size for a vertical canvas", "[scene][camera]") {
    Camera c(125, 200, M_PI / 2);
    REQUIRE(double_equal(c.pixel_size, 0.01));
}

TEST_CASE("Constructing a ray through the center of the canvas",
    "[scene][camera]") {
    Camera c(201, 101, M_PI / 2);
    Ray r = c.ray_for_pixel(100, 50);
    REQUIRE(r.origin == Point(0, 0, 0));
    REQUIRE(r.dir == Vector(0, 0, -1));
}

TEST_CASE("Constructing a ray through the corner of the canvas",
    "[scene][camera]") {
    Camera c(201, 101, M_PI / 2);
    Ray r = c.ray_for_pixel(0, 0);
    REQUIRE(r.origin == Point(0, 0, 0));
    REQUIRE(r.dir == Vector(0.66519, 0.33259, -0.66851));
}

TEST_CASE("Constructing a ray when the camera is transformed",
    "[scene][camera]") {
    Camera c(201, 101, M_PI / 2);
    c.transform =
        Transform::rotation_y(M_PI / 4) * Transform::translation(0, -2, 5);
    Ray r = c.ray_for_pixel(100, 50);
    REQUIRE(r.origin == Point(0, 2, -5));
    REQUIRE(r.dir == Vector(sqrt(2) / 2, 0, -sqrt(2) / 2));
}

TEST_CASE("Rendering a world with a camera", "[scene][camera][world]") {
    const auto [w, s1, s2] = World::default_world();
    Camera c(11, 11, M_PI / 2);
    Point from(0, 0, -5);
    Point to(0, 0, 0);
    Vector up(0, 1, 0);
    c.transform = Transform::view_transform(from, to, up);

    Canvas image = c.render(&w);
    REQUIRE(image.pixel_at(5, 5) == Color(0.38066, 0.47583, 0.2855));
}