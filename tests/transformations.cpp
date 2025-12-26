#include "../src/math/transformations.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Multiplying by a translation matrix", "[transformations]") {
    Transform transform = Transform::translation(5, -3, 2);
    Point p(-3, 4, 5);
    REQUIRE(transform * p == Point(2, 1, 7));
}

TEST_CASE("Multiplying by the inverse of a translation matrix",
          "[transformations]") {
    Transform transform = Transform::translation(5, -3, 2);
    Transform inv = transform.inverse();
    Point p(-3, 4, 5);
    REQUIRE(inv * p == Point(-8, 7, 3));
}

TEST_CASE("Translation does not affect vectors", "[transformations]") {
    Transform transform = Transform::translation(5, -3, 2);
    Vector v(-3, 4, 5);
    REQUIRE(transform * v == v);
}

TEST_CASE("A scaling matrix applied to a point", "[transformations]") {
    Transform transform = Transform::scaling(2, 3, 4);
    Point p(-4, 6, 8);
    REQUIRE(transform * p == Point(-8, 18, 32));
}

TEST_CASE("A scaling matrix applied to a vector", "[transformations]") {
    Transform transform = Transform::scaling(2, 3, 4);
    Vector v(-4, 6, 8);
    REQUIRE(transform * v == Vector(-8, 18, 32));
}

TEST_CASE("Multiplying by the inverse of a scaling matrix",
          "[transformations]") {
    Transform transform = Transform::scaling(2, 3, 4);
    Transform inv = transform.inverse();
    Vector v(-4, 6, 8);
    REQUIRE(inv * v == Vector(-2, 2, 2));
}

TEST_CASE("Reflection is scaling by a negative value", "[transformations]") {
    Transform transform = Transform::scaling(-1, 1, 1);
    Point p(2, 3, 4);
    REQUIRE(transform * p == Point(-2, 3, 4));
}

TEST_CASE("Rotating a point around the x axis", "[transformations]") {
    Point p(0, 1, 0);
    Transform half_quarter = Transform::rotation_x(M_PI / 4);
    Transform full_quarter = Transform::rotation_x(M_PI / 2);
    REQUIRE(half_quarter * p == Point(0, sqrt(2) / 2, sqrt(2) / 2));
    REQUIRE(full_quarter * p == Point(0, 0, 1));
}

TEST_CASE("The inverse of an x-rotation rotates in the opposite direction",
          "[transformations]") {
    Point p(0, 1, 0);
    Transform half_quarter = Transform::rotation_x(M_PI / 4);
    Transform inv = half_quarter.inverse();
    REQUIRE(inv * p == Point(0, sqrt(2) / 2, -sqrt(2) / 2));
}

TEST_CASE("Rotating a point around the y axis", "[transformations]") {
    Point p(0, 0, 1);
    Transform half_quarter = Transform::rotation_y(M_PI / 4);
    Transform full_quarter = Transform::rotation_y(M_PI / 2);
    REQUIRE(half_quarter * p == Point(sqrt(2) / 2, 0, sqrt(2) / 2));
    REQUIRE(full_quarter * p == Point(1, 0, 0));
}

TEST_CASE("Rotating a point around the z axis", "[transformations]") {
    Point p(0, 1, 0);
    Transform half_quarter = Transform::rotation_z(M_PI / 4);
    Transform full_quarter = Transform::rotation_z(M_PI / 2);
    REQUIRE(half_quarter * p == Point(-sqrt(2) / 2, sqrt(2) / 2, 0));
    REQUIRE(full_quarter * p == Point(-1, 0, 0));
}

TEST_CASE("A shearing transformation moves x in proportion to y",
          "[transformations]") {
    Transform transform = Transform::shearing(1, 0, 0, 0, 0, 0);
    Point p(2, 3, 4);
    REQUIRE(transform * p == Point(5, 3, 4));
}

TEST_CASE("A shearing transformation moves x in proportion to z",
          "[transformations]") {
    Transform transform = Transform::shearing(1, 0, 0, 0, 0, 0);
    Point p(2, 3, 4);
    REQUIRE(transform * p == Point(5, 3, 4));
}

TEST_CASE("A shearing transformation moves y in proportion to x",
          "[transformations]") {
    Transform transform = Transform::shearing(1, 0, 0, 0, 0, 0);
    Point p(2, 3, 4);
    REQUIRE(transform * p == Point(5, 3, 4));
}

TEST_CASE("A shearing transformation moves y in proportion to z",
          "[transformations]") {
    Transform transform = Transform::shearing(1, 0, 0, 0, 0, 0);
    Point p(2, 3, 4);
    REQUIRE(transform * p == Point(5, 3, 4));
}

TEST_CASE("A shearing transformation moves z in proportion to x",
          "[transformations]") {
    Transform transform = Transform::shearing(1, 0, 0, 0, 0, 0);
    Point p(2, 3, 4);
    REQUIRE(transform * p == Point(5, 3, 4));
}

TEST_CASE("A shearing transformation moves z in proportion to y",
          "[transformations]") {
    Transform transform = Transform::shearing(1, 0, 0, 0, 0, 0);
    Point p(2, 3, 4);
    REQUIRE(transform * p == Point(5, 3, 4));
}

TEST_CASE("Chained transformation equivalent to individual transformations",
          "[transformations]") {
    Point p(1, 0, 1);
    Transform A = Transform::rotation_x(M_PI / 2);
    Transform B = Transform::scaling(5, 5, 5);
    Transform C = Transform::translation(10, 5, 7);

    Point p2 = A * p;
    REQUIRE(p2 == Point(1, -1, 0));
    Point p3 = B * p2;
    REQUIRE(p3 == Point(5, -5, 0));
    Point p4 = C * p3;
    REQUIRE(p4 == Point(15, 0, 7));

    Matrix T = C * B * A;
    REQUIRE(T * p == p4);
}

TEST_CASE("The transformation matrix for the default orientation",
          "[scene][transformations]") {
    Point from(0, 0, 0);
    Point to(0, 0, -1);
    Vector up(0, 1, 0);
    Transform t = Transform::view_transform(from, to, up);

    REQUIRE(t == identity_matrix4);
}

TEST_CASE("A view transformation matrix looking in positive z direction",
          "[scene][transformations]") {
    Point from(0, 0, 0);
    Point to(0, 0, 1);
    Vector up(0, 1, 0);
    Transform t = Transform::view_transform(from, to, up);

    REQUIRE(t == Transform::scaling(-1, 1, -1));
}

// TODO: page 99, how does this prove it moves the world and not the eye?
TEST_CASE("The view transformation moves the world",
          "[scene][transformations]") {
    Point from(0, 0, 8);
    Point to(0, 0, 0);
    Vector up(0, 1, 0);
    Transform t = Transform::view_transform(from, to, up);

    REQUIRE(t == Transform::translation(0, 0, -8));
}

TEST_CASE("An arbitrary view transformation", "[scene][transformations]") {
    Point from(1, 3, 2);
    Point to(4, -2, 8);
    Vector up(1, 1, 0);
    Transform t = Transform::view_transform(from, to, up);
    Matrix targetTransform(4, 4);
    targetTransform = {-0.50709, 0.50709, 0.67612,  -2.36643,
                       0.76772,  0.60609, 0.12122,  -2.82843,
                       -0.35857, 0.59761, -0.71714, 0.00000,
                       0.00000,  0.00000, 0.00000,  1.00000};
    REQUIRE(t == targetTransform);
}