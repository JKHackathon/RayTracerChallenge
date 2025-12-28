#include "../src/scene/patterns.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/geometry/shapes.hpp"
#include "../src/rendering/lighting.hpp"

struct ColorFixture {
    Color black;
    Color white;
    ColorFixture() : black(Color(0, 0, 0)), white(Color(1, 1, 1)) {}
};

TEST_CASE_METHOD(ColorFixture, "Creating a stripe pattern", "[patterns]") {
    StripePattern pattern(white, black);
    REQUIRE(pattern.a == white);
    REQUIRE(pattern.b == black);
}

TEST_CASE_METHOD(ColorFixture, "A stripe pattern is constant in y",
                 "[patterns]") {
    StripePattern pattern(white, black);
    REQUIRE(pattern.stripe_at(Point(0, 0, 0)) == white);
    REQUIRE(pattern.stripe_at(Point(0, 1, 0)) == white);
    REQUIRE(pattern.stripe_at(Point(0, 2, 0)) == white);
}

TEST_CASE_METHOD(ColorFixture, "A stripe pattern is constant in z",
                 "[patterns]") {
    StripePattern pattern(white, black);
    REQUIRE(pattern.stripe_at(Point(0, 0, 0)) == white);
    REQUIRE(pattern.stripe_at(Point(0, 0, 1)) == white);
    REQUIRE(pattern.stripe_at(Point(0, 0, 2)) == white);
}

TEST_CASE_METHOD(ColorFixture, "A stripe pattern alternates in x",
                 "[patterns]") {
    StripePattern pattern(white, black);
    REQUIRE(pattern.stripe_at(Point(0, 0, 0)) == white);
    REQUIRE(pattern.stripe_at(Point(0.9, 0, 0)) == white);
    REQUIRE(pattern.stripe_at(Point(1, 0, 0)) == black);
    REQUIRE(pattern.stripe_at(Point(-.1, 0, 0)) == black);
    REQUIRE(pattern.stripe_at(Point(-1, 0, 0)) == black);
    REQUIRE(pattern.stripe_at(Point(-1.1, 0, 0)) == white);
}

TEST_CASE_METHOD(ColorFixture, "Lighting with a pattern applied",
                 "[patterns][materials][shading]") {
    Material m;
    StripePattern pattern(white, black);
    m.pattern = &pattern;
    m.ambient = 1;
    m.diffuse = 0;
    m.specular = 0;
    Vector eye = Vector(0, 0, -1);
    Vector normal = Vector(0, 0, -1);
    PointLight light(Point(0, 0, -10), white);

    Sphere s;
    Color c1 =
        Shading::phong_lighting(m, &s, &light, Point(.9, 0, 0), eye, normal);
    Color c2 =
        Shading::phong_lighting(m, &s, &light, Point(1.1, 0, 0), eye, normal);

    REQUIRE(c1 == white);
    REQUIRE(c2 == black);
}

TEST_CASE_METHOD(ColorFixture, "Stripes with an object transformation",
                 "[patterns]") {
    Sphere object;
    object.transform = Transform::scaling(2, 2, 2);
    auto pattern = StripePattern(white, black);
    Color c = pattern.pattern_at_shape(&object, Point(1.5, 0, 0));
    REQUIRE(c == white);
}

TEST_CASE_METHOD(ColorFixture, "Stripes with a pattern transformation",
                 "[patterns]") {
    Sphere object;
    auto pattern = StripePattern(white, black);
    pattern.transform = Transform::scaling(2, 2, 2);
    Color c = pattern.pattern_at_shape(&object, Point(1.5, 0, 0));
    REQUIRE(c == white);
}

TEST_CASE_METHOD(ColorFixture,
                 "Stripes with both an object and a pattern transformation",
                 "[patterns]") {
    Sphere object;
    object.transform = Transform::scaling(2, 2, 2);
    auto pattern = StripePattern(white, black);
    pattern.transform = Transform::translation(.5, 0, 0);
    Color c = pattern.pattern_at_shape(&object, Point(2.5, 0, 0));
    REQUIRE(c == white);
}

TEST_CASE("The default pattern transformation", "[patterns]") {
    TestPattern pattern;
    REQUIRE(pattern.transform == identity_matrix4);
}

TEST_CASE("Assigning a transformation", "[patterns]") {
    TestPattern pattern;
    pattern.transform = Transform::translation(1, 2, 3);
    REQUIRE(pattern.transform == Transform::translation(1, 2, 3));
}

TEST_CASE("A pattern with an object transformation", "[patterns]") {
    Sphere shape;
    shape.transform = Transform::scaling(2, 2, 2);
    TestPattern pattern;
    Color c = pattern.pattern_at_shape(&shape, Point(2, 3, 4));
    REQUIRE(c == Color(1, 1.5, 2));
}

TEST_CASE("A pattern with a pattern transformation", "[patterns]") {
    Sphere shape;
    TestPattern pattern;
    pattern.transform = Transform::scaling(2, 2, 2);
    Color c = pattern.pattern_at_shape(&shape, Point(2, 3, 4));
    REQUIRE(c == Color(1, 1.5, 2));
}

TEST_CASE("A pattern with both an object and a pattern transformation",
          "[patterns]") {
    Sphere shape;
    shape.transform = Transform::scaling(2, 2, 2);
    TestPattern pattern;
    pattern.transform = Transform::translation(.5, 1, 1.5);
    Color c = pattern.pattern_at_shape(&shape, Point(2.5, 3, 3.5));
    REQUIRE(c == Color(.75, .5, .25));
}

TEST_CASE_METHOD(ColorFixture,
                 "A gradient linearly interpolates between colors",
                 "[patterns]") {
    GradientPattern pattern(white, black);
    Sphere s;
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, 0, 0)) == white);
    REQUIRE(pattern.pattern_at_shape(&s, Point(0.25, 0, 0)) ==
            Color(.75, .75, .75));
    REQUIRE(pattern.pattern_at_shape(&s, Point(0.5, 0, 0)) ==
            Color(.5, .5, .5));
    REQUIRE(pattern.pattern_at_shape(&s, Point(0.75, 0, 0)) ==
            Color(.25, .25, .25));
}

TEST_CASE_METHOD(ColorFixture, "A ring should extend in both x and z",
                 "[patterns]") {
    RingPattern pattern(white, black);
    Sphere s;
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, 0, 0)) == white);
    REQUIRE(pattern.pattern_at_shape(&s, Point(1, 0, 0)) == black);
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, 0, 1)) == black);
    REQUIRE(pattern.pattern_at_shape(&s, Point(.708, 0, .708)) == black);
}

TEST_CASE_METHOD(ColorFixture, "Checkers should repeat in x", "[patterns]") {
    CheckerPattern pattern(white, black);
    Sphere s;
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, 0, 0)) == white);
    REQUIRE(pattern.pattern_at_shape(&s, Point(.99, 0, 0)) == white);
    REQUIRE(pattern.pattern_at_shape(&s, Point(1.01, 0, 0)) == black);
}

TEST_CASE_METHOD(ColorFixture, "Checkers should repeat in y", "[patterns]") {
    CheckerPattern pattern(white, black);
    Sphere s;
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, 0, 0)) == white);
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, .99, 0)) == white);
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, 1.01, 0)) == black);
}

TEST_CASE_METHOD(ColorFixture, "Checkers should repeat in z", "[patterns]") {
    CheckerPattern pattern(white, black);
    Sphere s;
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, 0, 0)) == white);
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, 0, .99)) == white);
    REQUIRE(pattern.pattern_at_shape(&s, Point(0, 0, 1.01)) == black);
}