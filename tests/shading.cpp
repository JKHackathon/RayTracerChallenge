#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/geometry/shapes.hpp"
#include "../src/rendering/lighting.hpp"

TEST_CASE("The normal on a sphere at a point on the x axis",
          "[sphere][shading]") {
    Sphere s;
    Vector n = s.normal_at(Point(1, 0, 0));
    REQUIRE(n == Vector(1, 0, 0));
}

TEST_CASE("The normal on a sphere at a point on the y axis",
          "[sphere][shading]") {
    Sphere s;
    Vector n = s.normal_at(Point(0, 1, 0));
    REQUIRE(n == Vector(0, 1, 0));
}

TEST_CASE("The normal on a sphere at a point on the z axis",
          "[sphere][shading]") {
    Sphere s;
    Vector n = s.normal_at(Point(0, 0, 1));
    REQUIRE(n == Vector(0, 0, 1));
}

TEST_CASE("The normal on a sphere at a nonaxial point", "[sphere][shading]") {
    Sphere s;
    Vector n = s.normal_at(Point(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
    REQUIRE(n == Vector(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
}

TEST_CASE("The normal is a normalized vector", "[sphere][shading]") {
    Sphere s;
    Vector n = s.normal_at(Point(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
    REQUIRE(n == n.normalized());
}

TEST_CASE("Computing the normal on a translated sphere", "[sphere][shading]") {
    Sphere s;
    s.transform = Transform::translation(0, 1, 0);
    Vector n = s.normal_at(Point(0, 1.70711, -0.70711));
    REQUIRE(n == Vector(0, 0.70711, -0.70711));
}

TEST_CASE("Computing the normal on a transformed sphere", "[sphere][shading]") {
    Sphere s;
    s.transform =
        Transform::scaling(1, 0.5, 1) * Transform::rotation_z(M_PI / 5);
    Vector n = s.normal_at(Point(0, sqrt(2) / 2, -sqrt(2) / 2));
    REQUIRE(n == Vector(0, 0.97014, -0.24254));
}

TEST_CASE("Reflecting a vector approaching at 45°", "[tuple][shading]") {
    Vector v(1, -1, 0);
    Vector n(0, 1, 0);
    Vector r = v.reflect(n);
    REQUIRE(r == Vector(1, 1, 0));
}

TEST_CASE("Reflecting a vector off a slanted surface", "[tuple][shading]") {
    Vector v(0, -1, 0);
    Vector n(sqrt(2) / 2, sqrt(2) / 2, 0);
    Vector r = v.reflect(n);
    REQUIRE(r == Vector(1, 0, 0));
}

TEST_CASE("A point light has a position and intensity", "[lights][shading]") {
    auto intensity = Color(1, 1, 1);
    Point position(0, 0, 0);
    PointLight light(position, intensity);
    REQUIRE(light.pos == position);
    REQUIRE(light.intensity == intensity);
}

TEST_CASE("The default material", "[materials][shading]") {
    Material m;
    REQUIRE(m.color == Color(1, 1, 1));
    REQUIRE(float_equal(m.ambient, .1f));
    REQUIRE(float_equal(m.diffuse, .9f));
    REQUIRE(float_equal(m.specular, .9));
    REQUIRE(float_equal(m.shininess, 200));
}

TEST_CASE("A sphere has a default material", "[sphere][materials][shading]") {
    Sphere s;
    Material m;
    REQUIRE(s.material == m);
}

TEST_CASE("A sphere may be assigned a material",
          "[sphere][materials][shading]") {
    Sphere s;
    Material m;
    m.ambient = 1;
    s.material = m;
    REQUIRE(s.material == m);
}

struct MaterialFixture {
    Material m;
    Point pos;
    MaterialFixture() : pos(Point(0, 0, 0)) {}

    ~MaterialFixture() {
        m = Material();
        pos = Point(0, 0, 0);
    }
};

TEST_CASE_METHOD(MaterialFixture,
                 "Lighting with the eye between the light and the surface",
                 "[materials][shading]") {
    Vector eye(0, 0, -1);
    Vector normal(0, 0, -1);
    PointLight light(Point(0, 0, -10), Color(1, 1, 1));
    auto result = Shading::phong_lighting(m, &light, pos, eye, normal);
    REQUIRE(result == Color(1.9, 1.9, 1.9));
}

TEST_CASE_METHOD(
    MaterialFixture,
    "Lighting with the eye between light and surface, eye offset 45°",
    "[materials][shading]") {
    Vector eye(0, sqrt(2) / 2, -sqrt(2) / 2);
    Vector normal(0, 0, -1);
    PointLight light(Point(0, 0, -10), Color(1, 1, 1));
    auto result = Shading::phong_lighting(m, &light, pos, eye, normal);
    REQUIRE(result == Color(1, 1, 1));
}

TEST_CASE_METHOD(MaterialFixture,
                 "Lighting with eye opposite surface, light offset 45°",
                 "[materials][shading]") {
    Vector eye(0, 0, -1);
    Vector normal(0, 0, -1);
    PointLight light(Point(0, 10, -10), Color(1, 1, 1));
    auto result = Shading::phong_lighting(m, &light, pos, eye, normal);
    REQUIRE(result == Color(0.7364, 0.7364, 0.7364));
}

TEST_CASE_METHOD(MaterialFixture,
                 "Lighting with eye in the path of the reflection vector",
                 "[materials][shading]") {
    Vector eye(0, -sqrt(2) / 2, -sqrt(2) / 2);
    Vector normal(0, 0, -1);
    PointLight light(Point(0, 10, -10), Color(1, 1, 1));
    auto result = Shading::phong_lighting(m, &light, pos, eye, normal);
    REQUIRE(result == Color(1.6364, 1.6364, 1.6364));
}

TEST_CASE_METHOD(MaterialFixture, "Lighting with the light behind the surface",
                 "[materials][shading]") {
    Vector eye(0, 0, -1);
    Vector normal(0, 0, -1);
    PointLight light(Point(0, 0, 10), Color(1, 1, 1));
    auto result = Shading::phong_lighting(m, &light, pos, eye, normal);
    REQUIRE(result == Color(.1, .1, .1));
}

TEST_CASE_METHOD(MaterialFixture, "Lighting with the surface in shadow",
                 "[materials][shading][shadows]") {
    Vector eye(0, 0, -1);
    Vector normal(0, 0, -1);
    PointLight light(Point(0, 0, -10), Color(1, 1, 1));
    auto result = Shading::phong_lighting(m, &light, pos, eye, normal, true);
    REQUIRE(result == Color(.1, .1, .1));
}