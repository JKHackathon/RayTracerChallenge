#include <catch2/catch_test_macros.hpp>
#include <cstdint>

#include "../src/tuples.hpp"

TEST_CASE("A tuple with w=1.0 is a point", "[tuple]") {
    Tuple p(4.3, -4.2, 3.1, 1.0);
    REQUIRE(float_equal(p.x, 4.3));
    REQUIRE(float_equal(p.y, -4.2));
    REQUIRE(float_equal(p.z, 3.1));
    REQUIRE(float_equal(p.w, 1.0));
    REQUIRE(p.is_point() == true);
    REQUIRE(p.is_vector() == false);
}

TEST_CASE("A tuple with w=0 is a vector", "[tuple]") {
    Tuple v(4.3, -4.2, 3.1, 0.0);
    REQUIRE(float_equal(v.x, 4.3));
    REQUIRE(float_equal(v.y, -4.2));
    REQUIRE(float_equal(v.z, 3.1));
    REQUIRE(float_equal(v.w, 0.0));
    REQUIRE(v.is_point() == false);
    REQUIRE(v.is_vector() == true);
}

TEST_CASE("A tuple can be created from a point", "[tuple]") {
    Point p(4.3, -4.2, 3.1);
    Tuple t(4.3, -4.2, 3.1, 1);
    REQUIRE(p == t);
}

TEST_CASE("A tuple can be created from a vector", "[tuple]") {
    Vector v(4.3, -4.2, 3.1);
    Tuple t(4.3, -4.2, 3.1, 0);
    REQUIRE(v == t);
}

TEST_CASE("Adding two tuples", "[tuple]") {
    Tuple a1(3, -2, 5, 1);
    Tuple a2(-2, 3, 1, 0);
    Tuple sum(1, 1, 6, 1);
    REQUIRE(a1 + a2 == sum);
}

TEST_CASE("Subtracting two points", "[tuple]") {
    Point p1(3, 2, 1);
    Point p2(5, 6, 7);
    Vector diff(-2, -4, -6);
    REQUIRE(p1 - p2 == diff);
}

TEST_CASE("Subtracting a vetor from a point", "[tuple]") {
    Point p(3, 2, 1);
    Vector v(5, 6, 7);
    Point diff(-2, -4, -6);
    REQUIRE(p - v == diff);
}

TEST_CASE("Subtracting two vectors", "[tuple]") {
    Vector v1(3, 2, 1);
    Vector v2(5, 6, 7);
    Vector diff(-2, -4, -6);
    REQUIRE(v1 - v2 == diff);
}

TEST_CASE("Subtracting a vector from the zero vector", "[tuple]") {
    Vector zero(0, 0, 0);
    Vector v(1, -2, 3);
    Vector diff(-1, 2, -3);
    REQUIRE(zero - v == diff);
}

// slight deviation from book as points should not be allowed to be negated
TEST_CASE("Negating a vector", "[tuple]") {
    Vector v(1, -2, 3);
    Vector n_v(-1, 2, -3);
    REQUIRE(-v == n_v);
}

TEST_CASE("Multiplying a tuple by a scalar", "[tuple]") {
    Tuple a(1, -2, 3, -4);
    REQUIRE(a * 3.5 == Tuple(3.5, -7, 10.5, -14));
}

TEST_CASE("Multiplying a tuple by a fraction", "[tuple]") {
    Tuple a(1, -2, 3, -4);
    REQUIRE(a * 0.5 == Tuple(0.5, -1, 1.5, -2));
}

TEST_CASE("Dividing a tuple by a scalar", "[tuple]") {
    Tuple a(1, -2, 3, -4);
    REQUIRE(a / 2 == Tuple(0.5, -1, 1.5, -2));
}

TEST_CASE("Computing the magnitude of vector(1, 0, 0)", "[tuple]") {
    Vector v(1, 0, 0);
    REQUIRE(v.magnitude() == 1);
}

TEST_CASE("Computing the magnitude of vector(0, 1, 0)", "[tuple]") {
    Vector v(0, 1, 0);
    REQUIRE(v.magnitude() == 1);
}

TEST_CASE("Computing the magnitude of vector(0, 0, 1)", "[tuple]") {
    Vector v(0, 0, 1);
    REQUIRE(v.magnitude() == 1);
}

TEST_CASE("Computing the magnitude of vector(1, 2, 3)", "[tuple]") {
    Vector v(1, 2, 3);
    REQUIRE(float_equal(v.magnitude(), sqrt(14)));
}

TEST_CASE("Computing the magnitude of vector(-1, -2, -3)", "[tuple]") {
    Vector v(-1, -2, -3);
    REQUIRE(float_equal(v.magnitude(), sqrt(14)));
}

TEST_CASE("Normalizing vector(4, 0, 0) gives (1, 0, 0)", "[tuple]") {
    Vector v(4, 0, 0);
    REQUIRE(v.normalized() == Vector(1, 0, 0));
}

TEST_CASE("Normalizing vector(1, 2, 3)", "[tuple]") {
    Vector v(1, 2, 3);
    REQUIRE(v.normalized() == Vector(1 / sqrt(14), 2 / sqrt(14), 3 / sqrt(14)));
    REQUIRE(float_equal(v.normalized().magnitude(), 1));
}

TEST_CASE("Dot product", "[tuple]") {
    Vector a(1, 2, 3);
    Vector b(2, 3, 4);
    REQUIRE(float_equal(a.dot(b), 20));
}

TEST_CASE("Cross product", "[tuple]") {
    Vector a(1, 2, 3);
    Vector b(2, 3, 4);
    REQUIRE(a.cross(b) == Vector(-1, 2, -1));
    REQUIRE(b.cross(a) == Vector(1, -2, 1));
}