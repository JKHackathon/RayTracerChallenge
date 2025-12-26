#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/geometry/ray.hpp"
#include "../src/scene/world.hpp"

TEST_CASE("There is no shadow when nothing is collinear with point and light",
          "[shadows][world]") {
    auto const [w, s1, s2] = World::default_world();
    Point p(0, 10, 0);
    REQUIRE(!w.is_shadowed(p));
}

TEST_CASE("The shadow when an object is between the point and the light",
          "[shadows][world]") {
    auto const [w, s1, s2] = World::default_world();
    Point p(10, -10, 10);
    REQUIRE(w.is_shadowed(p));
}

TEST_CASE("There is no shadow when an object is behind the light",
          "[shadows][world]") {
    auto const [w, s1, s2] = World::default_world();
    Point p(-20, 20, -20);
    REQUIRE(!w.is_shadowed(p));
}

TEST_CASE("There is no shadow when an object is behind the point",
          "[shadows][world]") {
    auto const [w, s1, s2] = World::default_world();
    Point p(-2, 2, -2);
    REQUIRE(!w.is_shadowed(p));
}

TEST_CASE("shade_hit() is given an intersection in shadow",
          "[shadows][world]") {
    World w;
    auto light = std::make_unique<PointLight>(Point(0, 0, -10), Color(1, 1, 1));
    w.light = std::move(light);
    auto s1_u = std::make_unique<Sphere>();
    auto s2_u = std::make_unique<Sphere>();
    s2_u.get()->transform = Transform::translation(0, 0, 10);

    Ray r(Point(0, 0, 5), Vector(0, 0, 1));
    auto i = Intersection(4, s2_u.get());

    auto comps = PrecomputedIntersection::prepare_computations(i, r);

    w.objects.emplace(s1_u.get(), std::move(s1_u));
    w.objects.emplace(s2_u.get(), std::move(s2_u));
    Color c = w.shade_hit(comps);
    REQUIRE(c == Color(.1, .1, .1));
}

TEST_CASE("The hit should offset the point", "[shadows][intersections]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto shape_u = std::make_unique<Sphere>();
    shape_u.get()->transform = Transform::translation(0, 0, 1);
    auto i = Intersection(5, shape_u.get());

    auto comps = PrecomputedIntersection::prepare_computations(i, r);
    REQUIRE(comps.over_point.z < -EPSILON / 2);
    REQUIRE(comps.point.z > comps.over_point.z);
}
