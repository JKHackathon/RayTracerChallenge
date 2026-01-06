#include "../src/geometry/shapes/all_shapes.hpp"

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
    REQUIRE(double_equal(xs.intersections[0].t, 1));
    REQUIRE(xs.intersections[0].object == &p);
}

TEST_CASE("Ray intersecting a plane from below", "[shapes][planes]") {
    Plane p;
    Ray r(Point(0, -1, 0), Vector(0, 1, 0));
    auto xs = p.intersect(r);
    REQUIRE(xs.count == 1);
    REQUIRE(double_equal(xs.intersections[0].t, 1));
    REQUIRE(xs.intersections[0].object == &p);
}

TEST_CASE("A ray intersects a cube", "[shapes][cubes]") {
    Cube c;
    Ray x_pos(Point(5, 0.5, 0), Vector(-1, 0, 0));
    auto xs = c.intersect(x_pos);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 4));
    REQUIRE(double_equal(xs.intersections[1].t, 6));


    Ray x_neg(Point(-5, 0.5, 0), Vector(1, 0, 0));
    xs = c.intersect(x_neg);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 4));
    REQUIRE(double_equal(xs.intersections[1].t, 6));

    Ray y_pos(Point(0.5, 5, 0), Vector(0, -1, 0));
    xs = c.intersect(y_pos);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 4));
    REQUIRE(double_equal(xs.intersections[1].t, 6));


    Ray y_neg(Point(0.5, -5, 0), Vector(0, 1, 0));
    xs = c.intersect(y_neg);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 4));
    REQUIRE(double_equal(xs.intersections[1].t, 6));

    Ray z_pos(Point(0.5, 0, 5), Vector(0, 0, -1));
    xs = c.intersect(y_pos);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 4));
    REQUIRE(double_equal(xs.intersections[1].t, 6));

    Ray z_neg(Point(0.5, 0, -5), Vector(0, 0, 1));
    xs = c.intersect(y_neg);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 4));
    REQUIRE(double_equal(xs.intersections[1].t, 6));

    Ray inside(Point(0, 0.5, 0), Vector(0, 0, 1));
    xs = c.intersect(inside);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, -1));
    REQUIRE(double_equal(xs.intersections[1].t, 1));
}

TEST_CASE("A ray misses a cube", "[shapes][cubes]") {
    Cube c;
    Ray r(Point(-2, 0, 0), Vector(0.2673, 0.5345, 0.8018));
    auto xs = c.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, -2, 0), Vector(0.8018, 0.2673, 0.5345));
    xs = c.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 0, -2), Vector(0.5345, 0.8018, 0.2673));
    xs = c.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(2, 0, 2), Vector(0, 0, -1));
    xs = c.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 2, 2), Vector(0, -1, 0));
    xs = c.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(2, 2, 0), Vector(-1, 0, 0));
    xs = c.intersect(r);
    REQUIRE(xs.count == 0);
}
TEST_CASE("The normal on the surface of a cube", "[shapes][cubes]") {
    Cube c;
    Point p(Point(1, 0.5, -0.8));
    Vector normal = c.normal_at(p);
    REQUIRE(normal == Vector(1, 0, 0));

    p = Point(Point(-1, -0.2, 0.9));
    normal = c.normal_at(p);
    REQUIRE(normal == Vector(-1, 0, 0));

    p = Point(Point(-0.4, 1, -0.1));
    normal = c.normal_at(p);
    REQUIRE(normal == Vector(0, 1, 0));

    p = Point(Point(0.3, -1, -0.7));
    normal = c.normal_at(p);
    REQUIRE(normal == Vector(0, -1, 0));

    p = Point(Point(-0.6, 0.3, 1));
    normal = c.normal_at(p);
    REQUIRE(normal == Vector(0, 0, 1));

    p = Point(Point(0.4, 0.4, -1));
    normal = c.normal_at(p);
    REQUIRE(normal == Vector(0, 0, -1));

    p = Point(Point(1, 1, 1));
    normal = c.normal_at(p);
    REQUIRE(normal == Vector(1, 0, 0));

    p = Point(Point(-1, -1, -1));
    normal = c.normal_at(p);
    REQUIRE(normal == Vector(-1, 0, 0));
}

TEST_CASE("A ray misses a cylinder", "[shapes][cylinders]") {
    Cylinder cyl;
    Ray r(Point(1, 0, 0), Vector(0, 1, 0));
    auto xs = cyl.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 0, 0), Vector(0, 1, 0));
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 0, -5), Vector(1, 1, 1).normalized());
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 0);
}

TEST_CASE("A ray strikes a cylinder", "[shapes][cylinders]") {
    Cylinder cyl;
    Ray r(Point(1, 0, -5), Vector(0, 0, 1));
    auto xs = cyl.intersect(r);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 5));
    REQUIRE(double_equal(xs.intersections[1].t, 5));

    r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 4));
    REQUIRE(double_equal(xs.intersections[1].t, 6));

    r = Ray(Point(0.5, 0, -5), Vector(0.1, 1, 1).normalized());
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 6.80798));
    REQUIRE(double_equal(xs.intersections[1].t, 7.08872));
}

TEST_CASE("Normal vector on a cylinder", "[shapes][cylinders]") {
    Cylinder cyl;
    Vector normal = cyl.normal_at(Point(1, 0, 0));
    REQUIRE(normal == Vector(1, 0, 0));

    normal = cyl.normal_at(Point(0, 5, -1));
    REQUIRE(normal == Vector(0, 0, -1));

    normal = cyl.normal_at(Point(0, -2, 1));
    REQUIRE(normal == Vector(0, 0, 1));

    normal = cyl.normal_at(Point(-1, 1, 0));
    REQUIRE(normal == Vector(-1, 0, 0));
}

TEST_CASE("The default minimum and maximum for a cylinder", "[shapes][cylinders]") {
    Cylinder cyl;
    cyl.minimum = std::numeric_limits<double>::lowest();
    cyl.maximum = std::numeric_limits<double>::max();
}

TEST_CASE("Intersecting a constrained cylinder", "[shapes][cylinders]") {
    Cylinder cyl;
    cyl.minimum = 1;
    cyl.maximum = 2;

    Ray r(Point(0, 1.5, 0), Vector(0.1, 1, 0)); // diagonal inside, going through hole in top
    auto xs = cyl.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 3, -5), Vector(0, 0, 1)); // above cylinder
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 0, -5), Vector(0, 0, 1)); // below cylinder
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 2, -5), Vector(0, 0, 1)); // at max
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 1, -5), Vector(0, 0, 1)); // at min
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 1.5, -2), Vector(0, 0, 1)); //through middle
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 2);
}

TEST_CASE("The default closed value for a cylinder", "[shapes][cylinders]") {
    Cylinder cyl;
    REQUIRE(!cyl.closed);
}

TEST_CASE("Intersecting the caps of a closed cylinder", "[shapes][cylinders]") {
    Cylinder cyl;
    cyl.minimum = 1;
    cyl.maximum = 2;
    cyl.closed = true;

    Ray r(Point(0, 3, 0), Vector(0, -1, 0)); // intersect both caps
    auto xs = cyl.intersect(r);
    REQUIRE(xs.count == 2);

    r = Ray(Point(0, 3, -2), Vector(0, -1, 2)); // intersact cap + side
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 2);

    r = Ray(Point(0, 4, -2), Vector(0, -1, 1)); // intersect cap + corner
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 2);

    r = Ray(Point(0, 0, -2), Vector(0, 1, 2)); // intersact cap + side
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 2);

    r = Ray(Point(0, -1, -2), Vector(0, 1, 1)); // intersect cap + corner
    xs = cyl.intersect(r);
    REQUIRE(xs.count == 2);
}

TEST_CASE("The normal vector on a cylinder's end caps", "[shapes][cylinders]") {
    Cylinder cyl;
    cyl.minimum = 1;
    cyl.maximum = 2;
    cyl.closed = true;

    Vector n = cyl.normal_at(Point(0, 1, 0));
    REQUIRE(n == Vector(0, -1, 0));

    n = cyl.normal_at(Point(0.5, 1, 0));
    REQUIRE(n == Vector(0, -1, 0));

    n = cyl.normal_at(Point(0, 1, 0.5));
    REQUIRE(n == Vector(0, -1, 0));

    n = cyl.normal_at(Point(0, 2, 0));
    REQUIRE(n == Vector(0, 1, 0));

    n = cyl.normal_at(Point(0.5, 2, 0));
    REQUIRE(n == Vector(0, 1, 0));

    n = cyl.normal_at(Point(0, 2, 0.5));
    REQUIRE(n == Vector(0, 1, 0));
}

TEST_CASE("Intersecting a cone with a ray", "[shapes][cones]") {
    Cone shape;
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    auto xs = shape.intersect(r);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 5));
    REQUIRE(double_equal(xs.intersections[1].t, 5));

    r = Ray(Point(0, 0, -5), Vector(1, 1, 1).normalized());
    xs = shape.intersect(r);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 8.66025));
    REQUIRE(double_equal(xs.intersections[1].t, 8.66025));

    r = Ray(Point(1, 1, -5), Vector(-.5, -1, 1).normalized());
    xs = shape.intersect(r);
    REQUIRE(xs.count == 2);
    REQUIRE(double_equal(xs.intersections[0].t, 4.55006));
    REQUIRE(double_equal(xs.intersections[1].t, 49.44994));
}

TEST_CASE("Intersecting a cone with a ray parallel to one of its halves", "[shapes][cones]") {
    Cone shape;
    Ray r(Point(0, 0, -1), Vector(0, 1, 1).normalized());
    auto xs = shape.intersect(r);
    REQUIRE(xs.count == 1);
    REQUIRE(double_equal(xs.intersections[0].t, .35355));
}

TEST_CASE("Intersecting a cone's end caps", "[shapes][cones]") {
    Cone shape;
    shape.minimum = -.5;
    shape.maximum = .5;
    shape.closed = true;

    Ray r(Point(0, 0, -5), Vector(0, 1, 0)); // parallel ray
    auto xs = shape.intersect(r);
    REQUIRE(xs.count == 0);

    r = Ray(Point(0, 0, -.25), Vector(0, 1, 1)); // cap + edge
    xs = shape.intersect(r);
    REQUIRE(xs.count == 2);

    r = Ray(Point(0, 0, -.25), Vector(0, 1, 0)); // intersect both caps and both edges
    xs = shape.intersect(r);
    REQUIRE(xs.count == 4);
}

TEST_CASE("Computing the normal vector on a cone", "[shapes][cones]") {
    Cone shape;
    Vector normal = shape.normal_at(Point(0, 0, 0));
    REQUIRE(normal == Vector(0, 0, 0));

    normal = shape.normal_at(Point(1, 1, 1));
    REQUIRE(normal == Vector(1, -sqrt(2), 1).normalized());

    normal = shape.normal_at(Point(-1, -1, 0));
    REQUIRE(normal == Vector(-1, 1, 0).normalized());
}