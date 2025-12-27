#include "../src/geometry/shapes.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("The default transformation", "[shapes]") {
    TestShape s;
    REQUIRE(s.transform == identity_matrix4);
}

TEST_CASE("Assigning a transformation", "[shapes]") {
    TestShape s;
    s.transform = Transform::translation(2, 3, 4);
    REQUIRE(s.transform == Transform::translation(2, 3, 4));
}

TEST_CASE("The default material", "[shapes]") {
    TestShape s;
    REQUIRE(s.material == Material());
}

TEST_CASE("Assigning a material", "[shapes]") {
    TestShape s;
    Material m;
    m.ambient = 1;
    s.material = m;
    REQUIRE(s.material == m);
}

TEST_CASE("Intersecting a scaled shape with a ray", "[shapes]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    TestShape s;
    s.transform = Transform::scaling(2, 2, 2);
    auto xs = s.intersect(r);
    REQUIRE(s.saved_ray.has_value());
    REQUIRE(s.saved_ray.value().origin == Point(0, 0, -2.5));
    REQUIRE(s.saved_ray.value().dir == Vector(0, 0, 0.5));
}

TEST_CASE("Intersecting a translated shape with a ray", "[shapes]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    TestShape s;
    s.transform = Transform::translation(5, 0, 0);
    auto xs = s.intersect(r);
    REQUIRE(s.saved_ray.has_value());
    REQUIRE(s.saved_ray.value().origin == Point(-5, 0, -5));
    REQUIRE(s.saved_ray.value().dir == Vector(0, 0, 1));
}

TEST_CASE("Computing the normal on a translated shape", "[shapes]") {
    TestShape s;
    s.transform = Transform::translation(0, 1, 0);
    Vector n = s.normal_at(Point(0, 1.70711, -.70711));
    REQUIRE(n == Vector(0, .70711, -.70711));
}

TEST_CASE("Computing the normal on a transformed shape", "[shapes]") {
    TestShape s;
    s.transform =
        Transform::scaling(1, .5, 1) * Transform::rotation_z(M_PI / 5);
    Vector n = s.normal_at(Point(0, sqrt(2) / 2, -sqrt(2) / 2));
    REQUIRE(n == Vector(0, 0.97014, -0.24254));
}

TEST_CASE("The normal of a plane is constant everywhere", "[shapes][planes]") {
    Plane p;
    REQUIRE(p.normal_at(Point(0, 0, 0)) == Vector(0, 1, 0));
    REQUIRE(p.normal_at(Point(10, 0, -10)) == Vector(0, 1, 0));
    REQUIRE(p.normal_at(Point(-5, 0, 150)) == Vector(0, 1, 0));
}

TEST_CASE("Intersect with a ray parallel to the plane", "[shapes][planes]") {
    Plane p;
    Ray r(Point(0, 10, 0), Vector(0, 0, 1));
    auto xs = p.intersect(r);
    REQUIRE(xs.count == 0);
}

TEST_CASE("Intersect with a coplanar ray", "[shapes][planes]") {
    Plane p;
    Ray r(Point(0, 0, 0), Vector(0, 0, 1));
    auto xs = p.intersect(r);
    REQUIRE(xs.count == 0);
}

TEST_CASE("Ray intersecting a plane from above", "[shapes][planes]") {
    Plane p;
    Ray r(Point(0, 1, 0), Vector(0, -1, 0));
    auto xs = p.intersect(r);
    REQUIRE(xs.count == 1);
    REQUIRE(float_equal(xs.intersections[0].t, 1));
    REQUIRE(xs.intersections[0].object == &p);
}

TEST_CASE("Ray intersecting a plane from below", "[shapes][planes]") {
    Plane p;
    Ray r(Point(0, -1, 0), Vector(0, 1, 0));
    auto xs = p.intersect(r);
    REQUIRE(xs.count == 1);
    REQUIRE(float_equal(xs.intersections[0].t, 1));
    REQUIRE(xs.intersections[0].object == &p);
}