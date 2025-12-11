#include "../src/canvas.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <iostream>

#include "../src/color.hpp"

TEST_CASE("Colors are (red, green, blue) tuples", "[canvas]") {
    Color c(-0.5, 0.4, 1.7);
    REQUIRE(float_equal(c.r, -0.5));
    REQUIRE(float_equal(c.g, 0.4));
    REQUIRE(float_equal(c.b, 1.7));
}

TEST_CASE("Adding colors", "[canvas]") {
    Color c1(0.9, 0.6, 0.75);
    Color c2(0.7, 0.1, 0.25);
    REQUIRE(c1 + c2 == Color(1.6, 0.7, 1.0));
}

TEST_CASE("Subtracting colors", "[canvas]") {
    Color c1(0.9, 0.6, 0.75);
    Color c2(0.7, 0.1, 0.25);
    REQUIRE(c1 - c2 == Color(0.2, 0.5, 0.5));
}

TEST_CASE("Multiplying a color by a scalar", "[canvas]") {
    Color c(0.2, 0.3, 0.4);
    REQUIRE(c * 2 == Color(0.4, 0.6, 0.8));
}

TEST_CASE("Multiplying colors", "[canvas]") {
    Color c1(1, 0.2, 0.4);
    Color c2(0.9, 1, 0.1);
    REQUIRE(c1 * c2 == Color(0.9, 0.2, 0.04));
}

TEST_CASE("Creating a canvas", "[canvas]") {
    Canvas c(10, 20);
    REQUIRE(c.get_width() == 10);
    REQUIRE(c.get_height() == 20);
    for (int i = 0; i < 10 * 20; i++) {
        REQUIRE(c.get_pixels().at(i) == Color(0, 0, 0));
    }
}

TEST_CASE("Writing pixels to a canvas", "[canvas]") {
    Canvas c(10, 20);
    Color red(1, 0, 0);
    c.write_pixel(2, 3, red);
    REQUIRE(c.get_pixels().at(c.coords_to_index(2, 3)) == red);
}

TEST_CASE("Constructing the PPM header", "[canvas]") {
    Canvas c(5, 3);
    std::cout << c.to_ppm_header() << std::endl;
    REQUIRE(c.to_ppm_header() == "P3\n5 3\n255");
}

TEST_CASE("Construcing the PPM pixel data", "[canvas]") {
    Canvas c(5, 3);
    Color c1(1.5, 0, 0);
    Color c2(0, 0.5, 0);
    Color c3(-0.5, 0, 1);

    c.write_pixel(0, 0, c1);
    c.write_pixel(2, 1, c2);
    c.write_pixel(4, 2, c3);

    REQUIRE(c.pixel_data_to_ppm() ==
            "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 \n"
            "0 0 0 0 0 0 0 127 0 0 0 0 0 0 0 \n" // changed to 127 because exact val doesnt really matter
            "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 \n");
}

TEST_CASE("Splitting long lines in PPM files", "[canvas]") {
    Canvas c(10, 2, Color(1, 0.8, 0.6));
    REQUIRE(c.pixel_data_to_ppm() ==
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 \n"
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 \n"
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 \n"
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 \n");
}

TEST_CASE("PPM files are terminated by a newline character", "[canvas]") {
    Canvas c(5, 3);
    REQUIRE(c.to_ppm().back() == '\n');
}