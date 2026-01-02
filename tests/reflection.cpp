#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/geometry/shapes.hpp"
#include "../src/scene/materials.hpp"
#include "../src/scene/world.hpp"

TEST_CASE("Reflectivity for the default material", "[reflections][materials]") {
    Material m;
    REQUIRE(double_equal(m.reflective, 0));
}

TEST_CASE("Precomputing the reflection vector",
    "[reflections][intersections]") {
    Plane shape;
    Ray r(Point(0, 1, -1), Vector(0, -sqrt(2) / 2, sqrt(2) / 2));
    Intersection i(sqrt(2), &shape);
    auto comps = PrecomputedIntersection::prepare_computations(i, r);

    REQUIRE(comps.reflect_dir == Vector(0, sqrt(2) / 2, sqrt(2) / 2));
}

TEST_CASE("The reflected color for a nonreflective material", "[reflections][world]") {
    auto [w, s1, s2] = World::default_world();
    Ray r(Point(0, 0, 0), Vector(0, 0, 1));
    s2->material.ambient = 1;
    Intersection i(1, s2);
    auto comps = PrecomputedIntersection::prepare_computations(i, r);
    Color c = w.reflected_color(comps);
    REQUIRE(c == Color(0, 0, 0));
}

TEST_CASE("The reflected color for a reflective material", "[reflections][world]") {
    auto [w, s1, s2] = World::default_world();
    auto shape_u = std::make_unique<Plane>();
    Plane* shape = shape_u.get();
    shape->material.reflective = .5;
    shape->transform = Transform::translation(0, -1, 0);
    w.objects.emplace(shape, std::move(shape_u));

    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2) / 2, sqrt(2) / 2));
    Intersection i(sqrt(2), shape);
    auto comps = PrecomputedIntersection::prepare_computations(i, r);
    Color c = w.reflected_color(comps);

    REQUIRE(c == Color(0.19032, 0.2379, 0.14274));
}

TEST_CASE("shade_hit() with a reflective material", "[reflections][world]") {
    auto [w, s1, s2] = World::default_world();
    auto shape_u = std::make_unique<Plane>();
    Plane* shape = shape_u.get();
    shape->material.reflective = .5;
    shape->transform = Transform::translation(0, -1, 0);
    w.objects.emplace(shape, std::move(shape_u));

    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2) / 2, sqrt(2) / 2));
    Intersection i(sqrt(2), shape);
    auto comps = PrecomputedIntersection::prepare_computations(i, r);
    Color c = w.shade_hit(comps);

    REQUIRE(c == Color(0.87677, 0.92436, 0.82918));
}

TEST_CASE("color_at() with mutually reflective surfaces", "[reflections][world]") {
    World w;
    w.light = std::make_unique<PointLight>(Point(0, 0, 0), Color(1, 1, 1));
    auto lower_u = std::make_unique<Plane>();
    Plane* lower = lower_u.get();
    lower->material.reflective = 1;
    lower->transform = Transform::translation(0, -1, 0);
    w.objects.emplace(lower, std::move(lower_u));

    auto upper_u = std::make_unique<Plane>();
    Plane* upper = upper_u.get();
    upper->material.reflective = 1;
    upper->transform = Transform::translation(0, 1, 0);
    w.objects.emplace(upper, std::move(upper_u));

    Ray r(Point(0, 0, 0), Vector(0, 1, 0));

    REQUIRE_NOTHROW(w.color_at((r)));
}

TEST_CASE("The reflected color at the maximum recursive depth", "[reflections][world]") {
    auto [w, s1, s2] = World::default_world();
    auto shape_u = std::make_unique<Plane>();
    Plane* shape = shape_u.get();
    shape->material.reflective = .5;
    shape->transform = Transform::translation(0, -1, 0);
    w.objects.emplace(shape, std::move(shape_u));

    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2) / 2, sqrt(2) / 2));
    Intersection i(sqrt(2), shape);
    auto comps = PrecomputedIntersection::prepare_computations(i, r);
    Color c = w.reflected_color(comps, REFLECTION_DEPTH);

    REQUIRE(c == Color(0, 0, 0));
}