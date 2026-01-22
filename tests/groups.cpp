#include "../src/geometry/shapes/all_shapes.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Creating a new group", "[shapes][groups]") {
    Group g;
    REQUIRE(g.transform == identity_matrix4);
    REQUIRE(g.shapes.empty());
}

TEST_CASE("A shape has a parent attribute", "[shapes][groups]") {
    TestShape s;
    REQUIRE(!s.parent.has_value());
}

TEST_CASE("Adding a child to a group", "[shapes][groups]") {
    Group g;
    auto s_u = std::make_unique<TestShape>();
    TestShape* s = s_u.get();
    g.add_child(std::move(s_u));
    REQUIRE(g.shapes[0].get() == s);
    REQUIRE(s->parent.has_value());
    REQUIRE(s->parent.value() == &g);
}

TEST_CASE("Intersecting a ray with an empty group", "[shapes][groups]") {
    Group g;
    Ray r(Point(0, 0, 0), Vector(0, 0, 1));
    auto xs = g.intersect(r);
    REQUIRE(xs.count == 0);
}

TEST_CASE("Intersecting a ray with a nonempty group", "[shapes][groups]") {
    Group g;
    auto s1_u = std::make_unique<Sphere>();
    Sphere* s1 = s1_u.get();
    auto s2_u = std::make_unique<Sphere>();
    Sphere* s2 = s2_u.get();
    s2->transform = Transform::translation(0, 0, -3);
    auto s3_u = std::make_unique<Sphere>();
    Sphere* s3 = s3_u.get();
    s3->transform = Transform::translation(5, 0, 0);

    g.add_child(std::move(s1_u));
    g.add_child(std::move(s2_u));
    g.add_child(std::move(s3_u));

    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto xs = g.intersect(r);
    REQUIRE(xs.count == 4);
    REQUIRE(xs.intersections[0].object == s2);
    REQUIRE(xs.intersections[1].object == s2);
    REQUIRE(xs.intersections[2].object == s1);
    REQUIRE(xs.intersections[3].object == s1);
}

TEST_CASE("Intersecting a transformed group", "[shapes][groups]") {
    Group g;
    g.transform = Transform::scaling(2, 2, 2);
    auto s_u = std::make_unique<Sphere>();
    Sphere* s = s_u.get();
    s->transform = Transform::translation(5, 0, 0);
    g.add_child(std::move(s_u));

    Ray r(Point(10, 0, -10), Vector(0, 0, 1));
    auto xs = g.intersect(r);
    REQUIRE(xs.count == 2);
}

TEST_CASE("Converting a point from world to object space", "[shapes][groups]") {
    Group g1;
    g1.transform = Transform::rotation_y(M_PI / 2);

    auto g2_u = std::make_unique<Group>();
    Group* g2 = g2_u.get();
    g2->transform = Transform::scaling(2, 2, 2);
    g1.add_child(std::move(g2_u));

    auto s_u = std::make_unique<Sphere>();
    Sphere* s = s_u.get();
    s->transform = Transform::translation(5, 0, 0);
    g2->add_child(std::move(s_u));

    Point p = s->world_to_object(Point(-2, 0, -10));
    REQUIRE(p == Point(0, 0, -1));
}

TEST_CASE("Converting a normal from object to world space", "[shapes][groups]") {
    Group g1;
    g1.transform = Transform::rotation_y(M_PI / 2);

    auto g2_u = std::make_unique<Group>();
    Group* g2 = g2_u.get();
    g2->transform = Transform::scaling(1, 2, 3);
    g1.add_child(std::move(g2_u));

    auto s_u = std::make_unique<Sphere>();
    Sphere* s = s_u.get();
    s->transform = Transform::translation(5, 0, 0);
    g2->add_child(std::move(s_u));

    Vector n = s->normal_to_world(Vector(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
    REQUIRE(n == Vector(0.2857, 0.4286, -0.8571));
}

TEST_CASE("Finding the normal on a child object", "[shapes][groups]") {
    Group g1;
    g1.transform = Transform::rotation_y(M_PI / 2);

    auto g2_u = std::make_unique<Group>();
    Group* g2 = g2_u.get();
    g2->transform = Transform::scaling(1, 2, 3);
    g1.add_child(std::move(g2_u));

    auto s_u = std::make_unique<Sphere>();
    Sphere* s = s_u.get();
    s->transform = Transform::translation(5, 0, 0);
    g2->add_child(std::move(s_u));

    Vector n = s->normal_at(Point(1.7321, 1.1547, -5.5774));
    REQUIRE(n == Vector(0.2857, 0.4286, -0.8571));
}