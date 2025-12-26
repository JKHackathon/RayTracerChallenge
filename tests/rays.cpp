#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/geometry/intersection.hpp"
#include "../src/geometry/ray.hpp"
#include "../src/geometry/shapes.hpp"

TEST_CASE("Creating and querying a ray", "[rays]") {
    Point origin(1, 2, 3);
    Vector dir(4, 5, 6);
    Ray r(origin, dir);

    REQUIRE(r.origin == origin);
    REQUIRE(r.dir == dir);
}

TEST_CASE("Computing a point from a distance", "[rays]") {
    Ray r(Point(2, 3, 4), Vector(1, 0, 0));
    REQUIRE(r.position(0) == Point(2, 3, 4));
    REQUIRE(r.position(1) == Point(3, 3, 4));
    REQUIRE(r.position(-1) == Point(1, 3, 4));
    REQUIRE(r.position(2.5) == Point(4.5, 3, 4));
}

TEST_CASE("A ray intersects a sphere at two points", "[rays]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto s = std::make_unique<Sphere>();
    IntersectionRecord xs = s.get()->intersect(r);

    REQUIRE(xs.count == 2);
    REQUIRE(float_equal(xs.intersections[0].t, 4));
    REQUIRE(float_equal(xs.intersections[1].t, 6));
}

TEST_CASE("A ray intersects a sphere at a tangent", "[rays]") {
    Ray r(Point(0, 1, -5), Vector(0, 0, 1));
    auto s = std::make_unique<Sphere>();
    IntersectionRecord xs = s.get()->intersect(r);

    REQUIRE(xs.count == 2);
    REQUIRE(float_equal(xs.intersections[0].t, 5));
    REQUIRE(float_equal(xs.intersections[1].t, 5));
}

TEST_CASE("A ray misses a sphere", "[rays]") {
    Ray r(Point(0, 2, -5), Vector(0, 0, 1));
    auto s = std::make_unique<Sphere>();
    IntersectionRecord xs = s.get()->intersect(r);

    REQUIRE(xs.count == 0);
    REQUIRE(xs.intersections.size() == 0);
}

TEST_CASE("A ray originates inside a sphere", "[rays]") {
    Ray r(Point(0, 0, 0), Vector(0, 0, 1));
    auto s = std::make_unique<Sphere>();
    IntersectionRecord xs = s.get()->intersect(r);

    REQUIRE(xs.count == 2);
    REQUIRE(float_equal(xs.intersections[0].t, -1));
    REQUIRE(float_equal(xs.intersections[1].t, 1));
}

TEST_CASE("A sphere is behind a ray", "[rays]") {
    Ray r(Point(0, 0, 5), Vector(0, 0, 1));
    auto s = std::make_unique<Sphere>();
    IntersectionRecord xs = s.get()->intersect(r);

    REQUIRE(xs.count == 2);
    REQUIRE(float_equal(xs.intersections[0].t, -6));
    REQUIRE(float_equal(xs.intersections[1].t, -4));
}

TEST_CASE("An intersection encapsulates t and object", "[rays]") {
    auto s = std::make_unique<Sphere>();
    Intersection i(3.5, s.get());
    REQUIRE(float_equal(i.t, 3.5));
    REQUIRE(i.object == s.get());
}

TEST_CASE("Aggregating intersections", "[rays]") {
    auto s = std::make_unique<Sphere>();
    Intersection i1(1, s.get());
    Intersection i2(2, s.get());
    IntersectionRecord xs(i1, i2);
    REQUIRE(float_equal(xs.intersections[0].t, 1));
    REQUIRE(float_equal(xs.intersections[1].t, 2));
}

TEST_CASE("Intersect sets the object on the intersection", "[rays]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto s = std::make_unique<Sphere>();
    IntersectionRecord xs = s.get()->intersect(r);
    REQUIRE(xs.count == 2);
    REQUIRE(xs.intersections[0].object == s.get());
    REQUIRE(xs.intersections[1].object == s.get());
}

TEST_CASE("The hit, when all intersections have positive t", "[rays]") {
    auto s = std::make_unique<Sphere>();
    Intersection i1(1, s.get());
    Intersection i2(2, s.get());
    IntersectionRecord xs(i1, i2);
    REQUIRE(xs.hit().has_value());
    REQUIRE(xs.hit().value() == i1);
}

TEST_CASE("The hit, when some intersections have negative t", "[rays]") {
    auto s = std::make_unique<Sphere>();
    Intersection i1(-1, s.get());
    Intersection i2(2, s.get());
    IntersectionRecord xs(i1, i2);
    REQUIRE(xs.hit().has_value());
    REQUIRE(xs.hit().value() == i2);
}

TEST_CASE("The hit, when all intersections have negative t", "[rays]") {
    auto s = std::make_unique<Sphere>();
    Intersection i1(-2, s.get());
    Intersection i2(-1, s.get());
    IntersectionRecord xs(i1, i2);
    REQUIRE(!xs.hit().has_value());
}

TEST_CASE("The hit is always the lowest nonnegative intersection", "[rays]") {
    auto s = std::make_unique<Sphere>();
    Intersection i1(5, s.get());
    Intersection i2(7, s.get());
    Intersection i3(-3, s.get());
    Intersection i4(2, s.get());
    IntersectionRecord xs(i1, i2, i3, i4);
    REQUIRE(xs.hit().has_value());
    REQUIRE(xs.hit().value() == i4);
}

TEST_CASE("Translating a ray", "[rays]") {
    Ray r(Point(1, 2, 3), Vector(0, 1, 0));
    Transform m = Transform::translation(3, 4, 5);
    Ray r2 = r.transform(m);
    REQUIRE(r2.origin == Point(4, 6, 8));
    REQUIRE(r2.dir == Vector(0, 1, 0));
}

TEST_CASE("Scaling a ray", "[rays]") {
    Ray r(Point(1, 2, 3), Vector(0, 1, 0));
    Transform m = Transform::scaling(2, 3, 4);
    Ray r2 = r.transform(m);
    REQUIRE(r2.origin == Point(2, 6, 12));
    REQUIRE(r2.dir == Vector(0, 3, 0));
}

TEST_CASE("A sphere's default transformation", "[rays][objects]") {
    Sphere s;
    REQUIRE(s.transform == identity_matrix4);
}

TEST_CASE("Changing a sphere's transformation", "[rays][objects]") {
    Sphere s;
    Transform t = Transform::translation(2, 3, 4);
    s.transform = t;
    REQUIRE(s.transform == t);
}

TEST_CASE("Intersecting a scaled sphere with a ray", "[rays]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto s_ptr = std::make_unique<Sphere>();
    Sphere* s = s_ptr.get();
    s->transform = Transform::scaling(2, 2, 2);
    IntersectionRecord xs = s->intersect(r);

    REQUIRE(xs.count == 2);
    REQUIRE(float_equal(xs.intersections[0].t, 3));
    REQUIRE(float_equal(xs.intersections[1].t, 7));
}

TEST_CASE("Intersecting a translated sphere with a ray", "[rays]") {
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto s_ptr = std::make_unique<Sphere>();
    Sphere* s = s_ptr.get();
    s->transform = Transform::translation(5, 0, 0);
    IntersectionRecord xs = s->intersect(r);

    REQUIRE(xs.count == 0);
}