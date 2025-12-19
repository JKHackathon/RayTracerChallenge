#include "canvas.hpp"

#include <cassert>
#include <iostream>

int Canvas::coords_to_index(int x, int y) const {
    assert(x < width && x >= 0 && "invald pixel 'x' index");
    assert(y < height && y >= 0 && "invald pixel 'y' index");
    return y * width + x;
}

void Canvas::write_pixel(int x, int y, Color c) {
    pixels[coords_to_index(x, y)] = c;
}

std::string Canvas::to_ppm_header() {
    return std::format("P3\n{} {}\n255\n", width, height);
}

std::string Canvas::pixel_data_to_ppm() {
    std::string ppm;
    for (int j = 0; j < height; j++) {
        int chars_added = 0;
        for (int i = 0; i < width; i++) {
            Color c = pixels[coords_to_index(i, j)];
            int r_final = std::clamp<int>(c.r * 255, 0, 255);
            int g_final = std::clamp<int>(c.g * 255, 0, 255);
            int b_final = std::clamp<int>(c.b * 255, 0, 255);

            std::string ppm_color =
                std::format("{} {} {} ", r_final, g_final, b_final);
            chars_added += ppm_color.size();
            if (chars_added > 70) {
                ppm += "\n";
                chars_added = 0;
            }
            ppm += ppm_color;
        }
        ppm += "\n";
    }
    return ppm;
}

std::string Canvas::to_ppm() { return to_ppm_header() + pixel_data_to_ppm(); }