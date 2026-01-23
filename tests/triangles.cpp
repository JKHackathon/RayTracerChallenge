#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/geometry/shapes/all_shapes.hpp"
#include "../src/geometry/shapes/obj_parser.hpp"

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

TEST_CASE("Ignoring unrecognized lines", "[triangles][obj_files]") {
    ObjParser parser = ObjParser::parse_obj_file("../tests/test_files/giberrish.obj");
    REQUIRE(parser.ignored_lines == 5);
}

TEST_CASE("Vertex records", "[triangles][obj_files]") {
    ObjParser parser = ObjParser::parse_obj_file("../tests/test_files/test9.obj");
    REQUIRE(parser.ignored_lines == 0);
    REQUIRE(parser.vertices[1] == Point(-1, 1, 0));
    REQUIRE(parser.vertices[2] == Point(-1, .5, 0));
    REQUIRE(parser.vertices[3] == Point(1, 0, 0));
    REQUIRE(parser.vertices[4] == Point(1, 1, 0));
}

TEST_CASE("Parsing triangle faces", "[triangles][obj_files]") {
    ObjParser parser = ObjParser::parse_obj_file("../tests/test_files/test10.obj");
    Group* g = parser.get_default_group();
    auto t1 = dynamic_cast<Triangle*>(g->shapes[0].get());
    auto t2 = dynamic_cast<Triangle*>(g->shapes[1].get());
    REQUIRE(t1->p1 == parser.vertices[1]);
    REQUIRE(t1->p2 == parser.vertices[2]);
    REQUIRE(t1->p3 == parser.vertices[3]);
    REQUIRE(t2->p1 == parser.vertices[1]);
    REQUIRE(t2->p2 == parser.vertices[3]);
    REQUIRE(t2->p3 == parser.vertices[4]);
}

TEST_CASE("Triangulating polygons", "[triangles][obj_files]") {
    ObjParser parser = ObjParser::parse_obj_file("../tests/test_files/test11.obj");
    Group* g = parser.get_default_group();
    auto t1 = dynamic_cast<Triangle*>(g->shapes[0].get());
    auto t2 = dynamic_cast<Triangle*>(g->shapes[1].get());
    auto t3 = dynamic_cast<Triangle*>(g->shapes[2].get());

    REQUIRE(t1->p1 == parser.vertices[1]);
    REQUIRE(t1->p2 == parser.vertices[2]);
    REQUIRE(t1->p3 == parser.vertices[3]);
    REQUIRE(t2->p1 == parser.vertices[1]);
    REQUIRE(t2->p2 == parser.vertices[3]);
    REQUIRE(t2->p3 == parser.vertices[4]);
    REQUIRE(t3->p1 == parser.vertices[1]);
    REQUIRE(t3->p2 == parser.vertices[4]);
    REQUIRE(t3->p3 == parser.vertices[5]);
}

TEST_CASE("Triangles in groups", "[triangles][obj_files]") {
    ObjParser parser = ObjParser::parse_obj_file("../tests/test_files/test12.obj");
    Group* g1 = parser.get_group("FirstGroup");
    Group* g2 = parser.get_group("SecondGroup");
    auto t1 = dynamic_cast<Triangle*>(g1->shapes[0].get());
    auto t2 = dynamic_cast<Triangle*>(g2->shapes[0].get());

    REQUIRE(t1->p1 == parser.vertices[1]);
    REQUIRE(t1->p2 == parser.vertices[2]);
    REQUIRE(t1->p3 == parser.vertices[3]);
    REQUIRE(t2->p1 == parser.vertices[1]);
    REQUIRE(t2->p2 == parser.vertices[3]);
    REQUIRE(t2->p3 == parser.vertices[4]);
}

TEST_CASE("Converting an OBJ file to a group", "[triangles][obj_files]") {
    ObjParser parser = ObjParser::parse_obj_file("../tests/test_files/test12.obj");
    Group* g1 = parser.get_group("FirstGroup");
    Group* g2 = parser.get_group("SecondGroup");

    auto g = std::move(parser).obj_to_group();

    REQUIRE((g->shapes[0].get() == g1 || g->shapes[0].get() == g2));
    REQUIRE((g->shapes[1].get() == g1 || g->shapes[1].get() == g2));

}

struct SmoothTriangleFixture {
    Point p1 = Point(0, 1, 0);
    Point p2 = Point(-1, 0, 0);
    Point p3 = Point(1, 0, 0);
    Vector n1 = Vector(0, 1, 0);
    Vector n2 = Vector(-1, 0, 0);
    Vector n3 = Vector(1, 0, 0);
    SmoothTriangle tri = SmoothTriangle(p1, p2, p3, n1, n2, n3);
};

TEST_CASE_METHOD(SmoothTriangleFixture, "Constructing a smooth triangle", "[triangles]") {
    REQUIRE(tri.p1 == p1);
    REQUIRE(tri.p2 == p2);
    REQUIRE(tri.p3 == p3);
    REQUIRE(tri.n1 == n1);
    REQUIRE(tri.n2 == n2);
    REQUIRE(tri.n3 == n3);
}

TEST_CASE("An intersection can encapsulate `u` and `v`", "[triangles][intersections]") {
    Triangle s(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    auto i = Intersection(3.5, &s, .2, .4);
    REQUIRE(double_equal(i.u, 0.2));
    REQUIRE(double_equal(i.v, 0.4));
}

TEST_CASE_METHOD(SmoothTriangleFixture, "An intersection with a smooth triangle stores u/v", "[triangles]") {
    Ray r(Point(-.2, .3, -2), Vector(0, 0, 1));
    auto xs = tri.intersect(r);
    REQUIRE(xs.count == 1);
    REQUIRE(double_equal(xs.intersections[0].u, 0.45));
    REQUIRE(double_equal(xs.intersections[0].v, 0.25));
}

TEST_CASE_METHOD(SmoothTriangleFixture, "A smooth triangle uses u/v to interpolate the normal", "[triangles]") {
    Intersection i(1, &tri, .45, .25);
    Vector n = tri.normal_at(Point(0, 0, 0), i);
    REQUIRE(n == Vector(-0.5547, 0.83205, 0));
}

TEST_CASE_METHOD(SmoothTriangleFixture, "Preparing the normal on a smooth triangle", "[triangles]") {
    Intersection i(1, &tri, .45, .25);
    Ray r(Point(-.2, .3, -2), Vector(0, 0, 1));
    auto xs = IntersectionRecord(i);
    auto comps = PrecomputedIntersection::prepare_computations(i, r, &xs);
    REQUIRE(comps.normal == Vector(-0.5547, 0.83205, 0));
}

TEST_CASE("Vertex normal records", "[triangles][obj_files]") {
    ObjParser parser = ObjParser::parse_obj_file("../tests/test_files/test19.obj");
    REQUIRE(parser.normals[1] == Vector(0, 0, 1));
    REQUIRE(parser.normals[2] == Vector(0.707, 0, -0.707));
    REQUIRE(parser.normals[3] == Vector(1, 2, 3));
}

TEST_CASE("Faces with normals", "[triangles][obj_files]") {
    ObjParser parser = ObjParser::parse_obj_file("../tests/test_files/test20.obj");
    auto g = parser.get_default_group();
    auto t1 = dynamic_cast<SmoothTriangle*>(g->shapes[0].get());
    auto t2 = dynamic_cast<SmoothTriangle*>(g->shapes[1].get());

    REQUIRE(t1->p1 == parser.vertices[1]);
    REQUIRE(t1->p2 == parser.vertices[2]);
    REQUIRE(t1->p3 == parser.vertices[3]);
    REQUIRE(t1->n1 == parser.normals[3]);
    REQUIRE(t1->n2 == parser.normals[1]);
    REQUIRE(t1->n3 == parser.normals[2]);

    REQUIRE(t2->p1 == parser.vertices[1]);
    REQUIRE(t2->p2 == parser.vertices[2]);
    REQUIRE(t2->p3 == parser.vertices[3]);
    REQUIRE(t2->n1 == parser.normals[3]);
    REQUIRE(t2->n2 == parser.normals[1]);
    REQUIRE(t2->n3 == parser.normals[2]);
    // REQUIRE(t1 == t2); // TODO: should triangles with identical values be considered equivalent?
}