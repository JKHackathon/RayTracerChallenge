#include "../src/accel/bounding_box.hpp"
#include "../src/geometry/shapes/all_shapes.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Creating an empty bounding box", "[accel][bounding_box]") {
    BoundingBox box;
    REQUIRE(box.min == Point(MAX_DOUBLE, MAX_DOUBLE, MAX_DOUBLE));
    REQUIRE(box.max == Point(MIN_DOUBLE, MIN_DOUBLE, MIN_DOUBLE));
}

TEST_CASE("Creating a bounding box with volume", "[accel][bounding_box]") {
    BoundingBox box(Point(-1, -2, -3), Point(3, 2, 1));
    REQUIRE(box.min == Point(-1, -2, -3));
    REQUIRE(box.max == Point(3, 2, 1));
}

TEST_CASE("Adding points to an empty bounding box", "[accel][bounding_box]") {
    BoundingBox box;
    box.add_point(Point(-5, 2, 0));
    box.add_point(Point(7, 0, -3));
    REQUIRE(box.min == Point(-5, 0, -3));
    REQUIRE(box.max == Point(7, 2, 0));
}

TEST_CASE("A sphere has a bounding box", "[accel][bounding_box][spheres]") {
    Sphere shape;
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(-1, -1, -1));
    REQUIRE(box.max == Point(1, 1, 1));
}

TEST_CASE("A plane has a bounding box", "[accel][bounding_box][planes]") {
    Plane shape;
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(MIN_DOUBLE, 0, MIN_DOUBLE));
    REQUIRE(box.max == Point(MAX_DOUBLE, 0, MAX_DOUBLE));
}

TEST_CASE("A cube has a bounding box", "[accel][bounding_box][cubes]") {
    Cube shape;
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(-1, -1, -1));
    REQUIRE(box.max == Point(1, 1, 1));
}

TEST_CASE("An unbounded cylinder has a bounding box", "[accel][bounding_box][cylinders]") {
    Cylinder shape;
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(-1, MIN_DOUBLE, -1));
    REQUIRE(box.max == Point(1, MAX_DOUBLE, 1));
}

TEST_CASE("A bounded cylinder has a bounding box", "[accel][bounding_box][cylinders]") {
    Cylinder shape;
    shape.minimum = -5;
    shape.maximum = 3;
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(-1, -5, -1));
    REQUIRE(box.max == Point(1, 3, 1));
}

TEST_CASE("An unbounded cone has a bounding box", "[accel][bounding_box][cones]") {
    Cone shape;
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(MIN_DOUBLE, MIN_DOUBLE, MIN_DOUBLE));
    REQUIRE(box.max == Point(MAX_DOUBLE, MAX_DOUBLE, MAX_DOUBLE));
}

TEST_CASE("A bounded cone has a bounding box", "[accel][bounding_box][cones]") {
    Cone shape;
    shape.minimum = -5;
    shape.maximum = 3;
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(-5, -5, -5));
    REQUIRE(box.max == Point(5, 3, 5));
}

TEST_CASE("A triangle has a bounding box", "[accel][bounding_box][triangles]") {
    Point p1(-3, 7, 2);
    Point p2(6, 2, -4);
    Point p3(2, -1, -1);
    Triangle shape(p1, p2, p3);
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(-3, -1, -4));
    REQUIRE(box.max == Point(6, 7, 2));
}

TEST_CASE("Test shape has arbitrary bounds", "[accel][bounding_box][shapes]") {
    TestShape shape;
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(-1, -1, -1));
    REQUIRE(box.max == Point(1, 1, 1));
}

TEST_CASE("Adding one bounding box to another", "[accel][bounding_box]") {
    BoundingBox box1(Point(-5, -2, 0), Point(7, 4, 4));
    BoundingBox box2(Point(8, -7, -2), Point(14, 2, 8));
    box1.add_BB(box2);
    REQUIRE(box1.min == Point(-5, -7, -2));
    REQUIRE(box1.max == Point(14, 4, 8));
}

TEST_CASE("Checking to see if a box contains a given point", "[accel][bounding_box]") {
    BoundingBox box(Point(5, -2, 0), Point(11, 4, 7));

    Point p(5, -2, 0);
    REQUIRE(box.contains_point(p));

    p = Point(11, 4, 7);
    REQUIRE(box.contains_point(p));

    p = Point(8, 1, 3);
    REQUIRE(box.contains_point(p));

    p = Point(3, 0, 3);
    REQUIRE(!box.contains_point(p));

    p = Point(8, -4, 3);
    REQUIRE(!box.contains_point(p));

    p = Point(8, 1, -1);
    REQUIRE(!box.contains_point(p));

    p = Point(13, 1, 3);
    REQUIRE(!box.contains_point(p));

    p = Point(8, 5, 3);
    REQUIRE(!box.contains_point(p));

    p = Point(8, 1, 8);
    REQUIRE(!box.contains_point(p));
}

TEST_CASE("Checking to see if a box contains a given box", "[accel][bounding_box]") {
    BoundingBox box(Point(5, -2, 0), Point(11, 4, 7));

    BoundingBox box2(Point(5, -2, 0), Point(11, 4, 7));
    REQUIRE(box.contains_bb(box2));

    box2 = BoundingBox(Point(6, -1, 1), Point(10, 3, 6));
    REQUIRE(box.contains_bb(box2));

    box2 = BoundingBox(Point(4, -3, 1), Point(10, 3, 6));
    REQUIRE(!box.contains_bb(box2));

    box2 = BoundingBox(Point(6, -1, 1), Point(12, 5, 8));
    REQUIRE(!box.contains_bb(box2));
}

TEST_CASE("Transforming a bounding box", "[accel][bounding_box]") {
    BoundingBox box(Point(-1, -1, -1), Point(1, 1, 1));
    auto transformation = Transform::rotation_x(M_PI / 4) * Transform::rotation_y(M_PI / 4);
    BoundingBox box2 = box.transform(transformation);
    REQUIRE(box2.min == Point(-1.4142, -1.7071, -1.7071));
    REQUIRE(box2.max == Point(1.4142, 1.7071, 1.7071));
}

TEST_CASE("Querying a shape's bounding box in its parent's space", "[accel][bounding_box][shapes]") {
    Sphere shape;
    shape.transform = Transform::translation(1, -3, 5) * Transform::scaling(0.5, 2, 4);
    BoundingBox box = shape.parent_space_bounds_of();
    REQUIRE(box.min == Point(0.5, -5, 1));
    REQUIRE(box.max == Point(1.5, -1, 9));
}

TEST_CASE("A group has a bounding box that contains its children", "[accel][bounding_box][groups]") {
    auto s_u = std::make_unique<Sphere>();
    s_u.get()->transform = Transform::translation(2, 5, -3) * Transform::scaling(2, 2, 2);
    auto c_u = std::make_unique<Cylinder>();
    Cylinder* c = c_u.get();
    c->minimum = -2;
    c->maximum = 2;
    c->transform = Transform::translation(-4, -1, 4) * Transform::scaling(.5, 1., .5);

    Group shape;
    shape.add_child(std::move(s_u));
    shape.add_child(std::move(c_u));
    auto box = shape.bounds_of();
    REQUIRE(box.min == Point(-4.5, -3, -5));
    REQUIRE(box.max == Point(4, 7, 4.5));
}

TEST_CASE("Intersecting a ray with a bounding box at the origin", "[accel][bounding_box]") {
    BoundingBox box(Point(-1, -1, -1), Point(1, 1, 1));

    Ray r(Point(5, .5, 0), Vector(-1, 0, 0));
    REQUIRE(box.intersects(r));

    r = Ray(Point(-5, .5, 0), Vector(1, 0, 0));
    REQUIRE(box.intersects(r));

    r = Ray(Point(.5, 5, 0), Vector(0, -1, 0));
    REQUIRE(box.intersects(r));

    r = Ray(Point(.5, -5, 0), Vector(0, 1, 0));
    REQUIRE(box.intersects(r));

    r = Ray(Point(.5, 0, 5), Vector(0, 0, -1));
    REQUIRE(box.intersects(r));

    r = Ray(Point(.5, 0, -5), Vector(0, 0, 1));
    REQUIRE(box.intersects(r));

    r = Ray(Point(0, .5, 0), Vector(0, 0, 1));
    REQUIRE(box.intersects(r));

    r = Ray(Point(-2, 0, 0), Vector(2, 4, 6));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(0, -2, 0), Vector(6, 2, 4));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(0, 0, -2), Vector(4, 6, 2));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(2, 0, 2), Vector(0, 0, -1));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(0, 2, 2), Vector(0, -1, 0));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(2, 2, 0), Vector(-1, 0, 0));
    REQUIRE(!box.intersects(r));
}

TEST_CASE("Intersecting a ray with a non-cubic bounding box", "[accel][bounding_box]") {
    BoundingBox box(Point(5, -2, 0), Point(11, 4, 7));

    Ray r(Point(15, 1, 2), Vector(-1, 0, 0));
    REQUIRE(box.intersects(r));

    r = Ray(Point(-5, -1, 4), Vector(1, 0, 0));
    REQUIRE(box.intersects(r));

    r = Ray(Point(7, 6, 5), Vector(0, -1, 0));
    REQUIRE(box.intersects(r));

    r = Ray(Point(9, -5, 6), Vector(0, 1, 0));
    REQUIRE(box.intersects(r));

    r = Ray(Point(8, 2, 12), Vector(0, 0, -1));
    REQUIRE(box.intersects(r));

    r = Ray(Point(6, 0, -5), Vector(0, 0, 1));
    REQUIRE(box.intersects(r));

    r = Ray(Point(8, 1, 3.5), Vector(0, 0, 1));
    REQUIRE(box.intersects(r));

    r = Ray(Point(9, -1, -8), Vector(2, 4, 6));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(8, 3, -4), Vector(6, 2, 4));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(9, -1, -2), Vector(4, 6, 2));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(4, 0, 9), Vector(0, 0, -1));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(8, 6, -1), Vector(0, -1, 0));
    REQUIRE(!box.intersects(r));

    r = Ray(Point(12, 5, 4), Vector(-1, 0, 0));
    REQUIRE(!box.intersects(r));
}

TEST_CASE("Intersecting ray+group doesn't test children if box is missed", "[accel][bounding_box]") {
    auto child_u = std::make_unique<TestShape>();
    TestShape* child = child_u.get();
    Group shape;
    shape.add_child(std::move(child_u));
    Ray r(Point(0, 0, -5), Vector(0, 1, 0));
    auto xs = shape.intersect(r);
    REQUIRE(!child->saved_ray.has_value());
}

TEST_CASE("Intersecting ray+group doesn't test children if box is hit", "[accel][bounding_box]") {
    auto child_u = std::make_unique<TestShape>();
    TestShape* child = child_u.get();
    Group shape;
    shape.add_child(std::move(child_u));
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto xs = shape.intersect(r);
    REQUIRE(child->saved_ray.has_value());
}