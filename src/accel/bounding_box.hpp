#pragma once
#include <algorithm>
#include "../geometry/ray.hpp"

struct BoundingBox {
    Point min;
    Point max;

    BoundingBox() {
        min = Point(MAX_DOUBLE, MAX_DOUBLE, MAX_DOUBLE);
        max = Point(MIN_DOUBLE, MIN_DOUBLE, MIN_DOUBLE);
    }

    BoundingBox(Point min, Point max) : min(min), max(max) {}

    void add_point(Point p);
    void add_BB(BoundingBox bb);

    bool contains_point(Point p) const;
    bool contains_bb(BoundingBox bb) const;

    BoundingBox transform(Transform transformation) const;

    bool intersects(Ray local_r) const;
};