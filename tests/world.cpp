#include "../src/world.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/ray.hpp"

TEST_CASE("Creating a world", "[world]") {
    World w;
    REQUIRE(w.objects.empty());
    REQUIRE(w.lights.empty());
}

// Test doesnt really work when using ptrs
// TEST_CASE("The default world", "[world]") {
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

TEST_CASE("Intersect a world with a ray", "[world]") {
    World w = World::default_world();
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto xs = r.intersect_world(w);

    REQUIRE(xs.count == 4);
    REQUIRE(float_equal(xs.intersections[0].t, 4));
    REQUIRE(float_equal(xs.intersections[1].t, 4.5));
    REQUIRE(float_equal(xs.intersections[2].t, 5.5));
    REQUIRE(float_equal(xs.intersections[3].t, 6));
}