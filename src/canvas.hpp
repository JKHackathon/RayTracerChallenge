#pragma once

#include <vector>

#include "color.hpp"

struct Canvas {
   private:
    size_t width;
    size_t height;
    std::vector<Color> pixels;

   public:
    Canvas(size_t width, size_t height, Color defaultColor = Color(0, 0, 0))
        : width(width), height(height), pixels(width * height, defaultColor) {}

    // Should i move this to utils, pass in width/height?
    int coords_to_index(size_t x, size_t y) const;
    void write_pixel(size_t x, size_t y, Color c);
    Color pixel_at(size_t x, size_t y) const;

    size_t get_width() const { return width; }
    size_t get_height() const { return height; }
    std::vector<Color> get_pixels() const { return pixels; }

    std::string to_ppm_header();
    std::string pixel_data_to_ppm();
    std::string to_ppm();
};