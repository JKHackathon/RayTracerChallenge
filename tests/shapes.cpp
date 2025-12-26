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
    TestShape s;;
    REQUIRE(s.material == Material());
}

TEST_CASE("Assigning a material", "[shapes]") {
    TestShape s;
    Material m;
    m.ambient = 1;
    s.material = m;
    REQUIRE(s.material == m);
}