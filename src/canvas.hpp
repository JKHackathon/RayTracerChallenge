#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>

#include "color.hpp"

struct Canvas {
   private:
    int width;
    int height;
    std::vector<Color> pixels;

   public:
    Canvas(int width, int height, Color defaultColor = Color(0,0,0))
        : width(width),
          height(height),
          pixels(width * height, defaultColor) {}

    int coords_to_index(int x, int y);
    void write_pixel(int x, int y, Color c);

    int get_width() const { return width; }
    int get_height() const { return height; }
    std::vector<Color> get_pixels() const { return pixels; }

    std::string to_ppm_header();
    std::string pixel_data_to_ppm();
    std::string to_ppm();
};

#endif