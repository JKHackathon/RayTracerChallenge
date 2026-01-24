#include "camera.hpp"

#include <iomanip>
#include <chrono>

Camera::Camera(size_t hsize, size_t vsize, double fov)
    : hsize(hsize), vsize(vsize), fov(fov), transform(identity_matrix4) {
    double half_view = tan(fov / 2);
    double aspect = float(hsize) / vsize;

    if (aspect >= 1) {
        half_width = half_view;
        half_height = half_view / aspect;
    }
    if (aspect < 1) {  // TODO: why does it flip??? pg 102
        half_width = half_view * aspect;
        half_height = half_view;
    }

    pixel_size = half_width * 2 / hsize;
}

Ray Camera::ray_for_pixel(size_t px, size_t py) const {
    // Offset from edge of canvas to pixel's center
    double xoffset = (px + 0.5) * pixel_size;
    double yoffset = (py + 0.5) * pixel_size;

    // untransformed coords of pixel in world space (camera looks toward -z)
    double world_x = half_width - xoffset;
    double world_y = half_height - yoffset;

    // Using camera matrix, transfomr canvas point + origin, compute ray's dir
    // vector (canvas at z=-1)
    auto pixel = transform.inverse() * Point(world_x, world_y, -1);
    auto origin = transform.inverse() * Point(0, 0, 0);
    Vector dir = Vector(pixel - origin).normalized();
    return Ray(origin, dir);
}

std::string format_duration(std::chrono::milliseconds ms) {
    using namespace std::chrono;

    auto total_ms = ms.count();

    auto hours = total_ms / 3'600'000;
    total_ms %= 3'600'000;
    auto minutes = total_ms / 60'000;
    total_ms %= 60'000;
    auto seconds = total_ms / 1'000;

    std::ostringstream oss;

    if (hours > 0)
        oss << hours << "h ";
    if (minutes > 0 || hours > 0)
        oss << minutes << "m ";
    oss << seconds << "s";

    return oss.str();
}


Canvas Camera::render(const World* w) const {
    Canvas image(hsize, vsize);

    constexpr int bar_width = 50;
    using clock = std::chrono::steady_clock;
    auto start_time = clock::now();

    for (size_t y = 0; y < vsize; y++) {
        for (size_t x = 0; x < hsize; x++) {
            Ray r = ray_for_pixel(x, y);
            Color c = w->color_at(r);
            image.write_pixel(x, y, c);
        }

        double progress = static_cast<double>(y + 1) / vsize;
        int pos = static_cast<int>(bar_width * progress);

        auto now = clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);

        // --- ETA calculation ---
        size_t rows_done = y + 1;
        size_t rows_left = vsize - rows_done;

        std::chrono::milliseconds eta_ms(0);
        if (rows_done > 0) {
            eta_ms = elapsed * rows_left / rows_done;
        }

        // --- Progress bar output ---
        std::cout << "\r[";
        for (int i = 0; i < bar_width; ++i) {
            if (i < pos)
                std::cout << "=";
            else if (i == pos)
                std::cout << ">";
            else
                std::cout << " ";
        }

        std::cout << "] "
            << std::setw(3) << static_cast<int>(progress * 100) << "% "
            << "Elapsed: " << format_duration(elapsed)
            << " | ETA: " << format_duration(eta_ms);

        std::cout.flush();
    }

    auto end_time = clock::now();
    auto total_elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "\nTotal time: "
        << format_duration(total_elapsed)
        << " (" << total_elapsed.count() << " ms)\n";

    return image;
}