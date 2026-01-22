#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/geometry/shapes/all_shapes.hpp"

TEST_CASE("Constructing a triangle", "[shapes][triangles]") {
    Point p1(0, 1, 0);
    Point p2(-1, 0, 0);
    Point p3(1, 0, 0);
    Triangle t(p1, p2, p3);

    REQUIRE(t.p1 == p1);
    REQUIRE(t.p2 == p2);
    REQUIRE(t.p3 == p3);
    REQUIRE(t.e1 == Vector(-1, -1, 0));
    REQUIRE(t.e2 == Vector(1, -1, 0));
    REQUIRE(t.normal == Vector(0, 0, -1));
}

TEST_CASE("Finding hte normal on a triangle", "[shapes][triangles]") {
    Triangle t(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    auto n1 = t.normal_at(Point(0, .5, 0));
    auto n2 = t.normal_at(Point(-.5, .75, 0));
    auto n3 = t.normal_at(Point(.5, .25, 0));
    REQUIRE(t.normal == n1);
    REQUIRE(t.normal == n2);
    REQUIRE(t.normal == n3);
}

TEST_CASE("Intersecting a ray parallel to the triangle", "[shapes][triangles]") {
    Triangle t(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(0, -1, -2), Vector(0, 1, 0));
    auto xs = t.intersect(r);
    REQUIRE(xs.count == 0);
}

TEST_CASE("A ray misses the p1-p3 edge", "[shapes][triangles]") {
    Triangle t(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(1, 1, -2), Vector(0, 0, 1));
    auto xs = t.intersect(r);
    REQUIRE(xs.count == 0);
}

TEST_CASE("A ray misses the p1-p2 edge", "[shapes][triangles]") {
    Triangle t(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(-1, 1, -2), Vector(0, 0, 1));
    auto xs = t.intersect(r);
    REQUIRE(xs.count == 0);
}

TEST_CASE("A ray misses the p2-p3 edge", "[shapes][triangles]") {
    Triangle t(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(0, -1, -2), Vector(0, 0, 1));
    auto xs = t.intersect(r);
    REQUIRE(xs.count == 0);
}

TEST_CASE("A ray strikes a triangle", "[shapes][triangles]") {
    Triangle t(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(0, .5, -2), Vector(0, 0, 1));
    auto xs = t.intersect(r);
    REQUIRE(xs.count == 1);
    REQUIRE(double_equal(xs.intersections[0].t, 2));
}