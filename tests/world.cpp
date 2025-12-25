#include "../src/world.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/ray.hpp"

TEST_CASE("Creating a world", "[world][scene]") {
    World w;
    REQUIRE(w.objects.empty());
    REQUIRE(w.light == nullptr);
    // REQUIRE(w.lights.empty());
}

// Test doesnt really work when using ptrs
// TEST_CASE("The default world", "[world][scene]") {
//     World w = World::default_world();

//     auto light =
//         std::make_unique<PointLight>(Point(-10, 10, -10), Color(1, 1, 1));

//     Material m;
//     m.color = Color(0.8, 1, 0.6);
//     m.diffuse = 0.7;
//     m.specular = 0.2;
//     auto s1 = std::make_unique<Sphere>();
//     s1.get()->material = m;
//     auto s2 = std::make_unique<Sphere>();
//     s2.get()->transform = Transform::scaling(0.5, 0.5, 0.5);

//     REQUIRE(w.lights.contains(light));
//     REQUIRE(w.objects.contains(s1));
//     REQUIRE(w.objects.contains(s2));
// }

TEST_CASE("Intersect a world with a ray", "[world][scene]") {
    const auto [w, s1, s2] = World::default_world();
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto xs = r.intersect_world(&w);

    REQUIRE(xs.count == 4);
    REQUIRE(float_equal(xs.intersections[0].t, 4));
    REQUIRE(float_equal(xs.intersections[1].t, 4.5));
    REQUIRE(float_equal(xs.intersections[2].t, 5.5));
    REQUIRE(float_equal(xs.intersections[3].t, 6));
}

TEST_CASE("Precomputing the state of an intersection",
          "[scene][intersections]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto shape = std::make_unique<Sphere>();
    Intersection i(4, shape.get());
    auto comps = PrecomputedIntersection::prepare_computations(i, r);

    REQUIRE(comps.t == i.t);
    REQUIRE(comps.object == i.object);
    REQUIRE(comps.point == Point(0, 0, -1));
    REQUIRE(comps.eye == Vector(0, 0, -1));
    REQUIRE(comps.normal == Vector(0, 0, -1));
}

TEST_CASE("The hit, when an intersection occurs on the outside",
          "[scene][intersections]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto shape = std::make_unique<Sphere>();
    Intersection i(4, shape.get());
    auto comps = PrecomputedIntersection::prepare_computations(i, r);
    REQUIRE(!comps.inside);
}

TEST_CASE("The hit, when an intersection occurs on the inside",
          "[scene][intersections]") {
    Ray r(Point(0, 0, 0), Vector(0, 0, 1));
    auto shape = std::make_unique<Sphere>();
    Intersection i(1, shape.get());
    auto comps = PrecomputedIntersection::prepare_computations(i, r);

    REQUIRE(comps.point == Point(0, 0, 1));
    REQUIRE(comps.eye == Vector(0, 0, -1));
    REQUIRE(comps.inside);
    REQUIRE(comps.normal == Vector(0, 0, -1));
}

// TODO: the below tests are problematic as they require specific order of
// objects (i am using unordered_set)
TEST_CASE("Shading an intersection", "[scene][world]") {
    const auto [w, s1, s2] = World::default_world();
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    // auto shape = w.objects.begin()->get();
    Intersection i = Intersection(4, s1);
    auto comps = PrecomputedIntersection::prepare_computations(i, r);
    Color c = w.shade_hit(comps);

    REQUIRE(c == Color(0.38066, 0.47583, 0.2855));
}

TEST_CASE("Shading an intersection from the inside", "[scene][world]") {
    auto [w, s1, s2] = World::default_world();
    w.light = std::make_unique<PointLight>(Point(0, 0.25, 0), Color(1, 1, 1));

    Ray r(Point(0, 0, 0), Vector(0, 0, 1));
    Intersection i = Intersection(.5, s2);

    auto comps = PrecomputedIntersection::prepare_computations(i, r);
    Color c = w.shade_hit(comps);

    REQUIRE(c == Color(0.90498, 0.90498, 0.90498));
}

TEST_CASE("The color when a ray misses", "[world][scene]") {
    const auto [w, s1, s2] = World::default_world();
    Ray r(Point(0, 0, -5), Vector(0, 1, 0));
    Color c = w.color_at(r);
    REQUIRE(c == Color(0, 0, 0));
}

TEST_CASE("The color when a ray hits", "[world][scene]") {
    const auto [w, s1, s2] = World::default_world();
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));

    Color c = w.color_at(r);
    REQUIRE(c == Color(0.38066, 0.47583, 0.2855));
}

TEST_CASE("The color with an intersection behind the ray", "[world][scene]") {
    const auto [w, outer, inner] = World::default_world();
    outer->material.ambient = 1;
    inner->material.ambient = 1;
    Ray r(Point(0, 0, .75), Vector(0, 0, -1));

    Color c = w.color_at(r);
    REQUIRE(c == inner->material.color);
}
