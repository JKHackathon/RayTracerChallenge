#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/geometry/shapes/all_shapes.hpp"
#include "../src/scene/world.hpp"

TEST_CASE("Transparency and Refractive Index for the default material", "[refraction][materials]") {
    Material m;
    REQUIRE(double_equal(m.transparency, 0));
    REQUIRE(double_equal(m.refractive_index, 1));
}

TEST_CASE("A helper for producing a sphere with a glassy material", "[refraction][spheres]") {
    GlassSphere s;
    REQUIRE(s.transform == identity_matrix4);
    REQUIRE(double_equal(s.material.transparency, 1));
    REQUIRE(double_equal(s.material.refractive_index, 1.5));
}

TEST_CASE("Finding n1 and n2 at various intersections", "[refraction][intersections]") {
    GlassSphere A;
    A.transform = Transform::scaling(2, 2, 2);
    A.material.refractive_index = 1.5;

    GlassSphere B;
    B.transform = Transform::translation(0, 0, -.25);
    B.material.refractive_index = 2;

    GlassSphere C;
    C.transform = Transform::translation(0, 0, .25);
    C.material.refractive_index = 2.5;

    Ray r(Point(0, 0, -4), Vector(0, 0, 1));
    auto xs = IntersectionRecord(Intersection(2, &A), Intersection(2.75, &B),
        Intersection(3.25, &C), Intersection(4.75, &B),
        Intersection(5.25, &C), Intersection(6, &A));

    std::vector<double> n1 = { 1, 1.5, 2, 2.5, 2.5, 1.5 };
    std::vector<double> n2 = { 1.5, 2, 2.5, 2.5, 1.5, 1 };

    for (int i = 0; i < xs.count; i++) {
        auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[i], r, &xs);
        REQUIRE(double_equal(comps.n1, n1[i]));
        REQUIRE(double_equal(comps.n2, n2[i]));
    }
}

TEST_CASE("The under point is offset below the surface", "[refraction][intersections]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    GlassSphere shape;
    shape.transform = Transform::translation(0, 0, 1);
    Intersection i(5, &shape);
    auto xs = IntersectionRecord(i);
    auto comps = PrecomputedIntersection::prepare_computations(i, r, &xs);

    REQUIRE(comps.under_point.z > EPSILON / 2);
    REQUIRE(comps.point.z < comps.under_point.z);
}

TEST_CASE("The refracted color with an opaque surface", "[refraction]") {
    const auto [w, s1, s2] = World::default_world();
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto xs = IntersectionRecord(Intersection(4, s1), Intersection(6, s1));
    auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[0], r, &xs);

    Color c = w.refracted_color(comps, 5);
    REQUIRE(c == Color(0, 0, 0));
}

TEST_CASE("The refracted color at the maximum recursive depth", "[refraction]") {
    auto [w, s1, s2] = World::default_world();
    s1->material.transparency = 1;
    s1->material.refractive_index = 1.5;
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto xs = IntersectionRecord(Intersection(4, s1), Intersection(6, s1));
    auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[0], r, &xs);

    Color c = w.refracted_color(comps, 0);
    REQUIRE(c == Color(0, 0, 0));
}

TEST_CASE("The refracted color under total internal reflection", "[refraction]") {
    auto [w, s1, s2] = World::default_world();
    s1->material.transparency = 1;
    s1->material.refractive_index = 1.5;
    Ray r(Point(0, 0, sqrt(2) / 2), Vector(0, 1, 0));
    auto xs = IntersectionRecord(Intersection(-sqrt(2) / 2, s1), Intersection(sqrt(2) / 2, s1));
    auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[1], r, &xs);

    Color c = w.refracted_color(comps, 5);
    REQUIRE(c == Color(0, 0, 0));
}

TEST_CASE("The refracted color with a refracted ray", "[refraction]") {
    auto [w, s1, s2] = World::default_world();
    s1->material.ambient = 1;
    TestPattern test_pattern;
    s1->material.pattern = &test_pattern;

    s2->material.transparency = 1;
    s2->material.refractive_index = 1.5;

    Ray r(Point(0, 0, .1), Vector(0, 1, 0));
    auto xs = IntersectionRecord(Intersection(-.9899, s1), Intersection(-.4899, s2), Intersection(.4899, s2), Intersection(.9899, s1));
    auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[2], r, &xs);

    Color c = w.refracted_color(comps, 5);
    REQUIRE(c == Color(0, 0.99888, .04725));
}

TEST_CASE("shade_hit() with a transparent material", "[refraction][world]") {
    auto [w, s1, s2] = World::default_world();
    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    floor->transform = Transform::translation(0, -1, 0);
    floor->material.transparency = .5;
    floor->material.refractive_index = 1.5;
    w.objects.emplace(floor, std::move(floor_u));

    auto ball_u = std::make_unique<Sphere>();
    Sphere* ball = ball_u.get();
    ball->material.color = Color(1, 0, 0);
    ball->material.ambient = .5;
    ball->transform = Transform::translation(0, -3.5, -.5);
    w.objects.emplace(ball, std::move(ball_u));

    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2) / 2, sqrt(2) / 2));
    auto xs = IntersectionRecord(Intersection(sqrt(2), floor));
    auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[0], r, &xs);

    Color c = w.shade_hit(comps, 5);
    REQUIRE(c == Color(0.93642, 0.68642, 0.68642));
}

TEST_CASE("The Schlick approximation under total internal reflection", "[refraction]") {
    GlassSphere shape;
    Ray r(Point(0, 0, sqrt(2 / 2)), Vector(0, 1, 0));
    auto xs = IntersectionRecord(Intersection(-sqrt(2) / 2, &shape), Intersection(sqrt(2) / 2, &shape));
    auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[1], r, &xs);
    float reflectance = Refraction::schlick(comps);
    REQUIRE(double_equal(reflectance, 1));
}

TEST_CASE("The Schlick approximation with a perpendicular viewing angle", "[refraction]") {
    GlassSphere shape;
    Ray r(Point(0, 0, 0), Vector(0, 1, 0));
    auto xs = IntersectionRecord(Intersection(-1, &shape), Intersection(1, &shape));
    auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[1], r, &xs);
    float reflectance = Refraction::schlick(comps);
    REQUIRE(double_equal(reflectance, .04));
}

TEST_CASE("The Schlick approximation with small angle and n2 > n1", "[refraction]") {
    GlassSphere shape;
    Ray r(Point(0, 0.99, -2), Vector(0, 0, 1));
    auto xs = IntersectionRecord(Intersection(1.8589, &shape));
    auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[0], r, &xs);
    float reflectance = Refraction::schlick(comps);
    REQUIRE(double_equal(reflectance, .48873));
}

TEST_CASE("shade_hit() with a reflective, transparent material", "[refraction][shading][world]") {
    auto [w, s1, s2] = World::default_world();
    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2) / 2, sqrt(2) / 2));

    auto floor_u = std::make_unique<Plane>();
    Plane* floor = floor_u.get();
    floor->transform = Transform::translation(0, -1, 0);
    floor->material.reflective = .5;
    floor->material.transparency = .5;
    floor->material.refractive_index = 1.5;
    w.objects.emplace(floor, std::move(floor_u));

    auto ball_u = std::make_unique<Sphere>();
    Sphere* ball = ball_u.get();
    ball->material.color = Color(1, 0, 0);
    ball->material.ambient = .5;
    ball->transform = Transform::translation(0, -3.5, -.5);
    w.objects.emplace(ball, std::move(ball_u));

    auto xs = IntersectionRecord(Intersection(sqrt(2), floor));
    auto comps = PrecomputedIntersection::prepare_computations(xs.intersections[0], r, &xs);
    Color c = w.shade_hit(comps, 5);
    REQUIRE(c == Color(0.93391, 0.69643, 0.69243));
}