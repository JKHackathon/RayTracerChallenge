#include "bounding_box.hpp"

void BoundingBox::add_point(Point p) {
    min.x = std::min(min.x, p.x);
    min.y = std::min(min.y, p.y);
    min.z = std::min(min.z, p.z);
    max.x = std::max(max.x, p.x);
    max.y = std::max(max.y, p.y);
    max.z = std::max(max.z, p.z);
}

void BoundingBox::add_BB(BoundingBox bb) {
    add_point(bb.min);
    add_point(bb.max);
}

bool BoundingBox::contains_point(Point p) const {
    return p.x >= min.x && p.x <= max.x &&
        p.y >= min.y && p.y <= max.y &&
        p.z >= min.z && p.z <= max.z;
}

bool BoundingBox::contains_bb(BoundingBox bb) const {
    return contains_point(bb.max) && contains_point(bb.min);
}

BoundingBox BoundingBox::transform(Transform transformation) const {
    BoundingBox bb;
    bb.add_point(transformation * min);
    bb.add_point(transformation * max);
    bb.add_point(transformation * Point(min.x, min.y, max.z));
    bb.add_point(transformation * Point(min.x, max.y, min.z));
    bb.add_point(transformation * Point(min.x, max.y, max.z));
    bb.add_point(transformation * Point(max.x, min.y, min.z));
    bb.add_point(transformation * Point(max.x, min.y, max.z));
    bb.add_point(transformation * Point(max.x, max.y, min.z));
    return bb;
}

// Identify where ray intersects planes offset by 1 from axis
std::pair<double, double> check_axis(double origin, double direction, double min, double max) {
    double tmin_numerator = min - origin;
    double tmax_numerator = max - origin;

    double tmin, tmax;
    if (abs(direction) >= EPSILON) {
        tmin = tmin_numerator / direction;
        tmax = tmax_numerator / direction;
    }
    else {
        tmin = tmin_numerator * INFINITY;
        tmax = tmax_numerator * INFINITY;
    }

    if (tmin > tmax) {
        double temp = tmin;
        tmin = tmax;
        tmax = temp;
    }
    return std::make_pair(tmin, tmax);
}

bool BoundingBox::intersects(Ray local_r) const {
    auto const [xtmin, xtmax] = check_axis(local_r.origin.x, local_r.dir.x, min.x, max.x);
    auto const [ytmin, ytmax] = check_axis(local_r.origin.y, local_r.dir.y, min.y, max.y);
    auto const [ztmin, ztmax] = check_axis(local_r.origin.z, local_r.dir.z, min.z, max.z);

    double tmin = std::max({ xtmin, ytmin, ztmin });
    double tmax = std::min({ xtmax, ytmax, ztmax });

    return tmin <= tmax;
}

std::pair<BoundingBox, BoundingBox> BoundingBox::split_bounds() const {
    double dx = max.x - min.x;
    double dy = max.y - min.y;
    double dz = max.z - min.z;

    double longest_axis = std::max(std::max(dx, dy), dz);

    auto mid_min = min;
    auto mid_max = max;

    if (double_equal(longest_axis, dx)) {
        mid_max.x = min.x + dx / 2;
        mid_min.x = mid_max.x;
    }
    else if (double_equal(longest_axis, dy)){
        mid_max.y = min.y + dy / 2;
        mid_min.y = mid_max.y;
    }
    else {
        mid_max.z = min.z + dz / 2;
        mid_min.z = mid_max.z;
    }

    auto left = BoundingBox(min, mid_max);
    auto right = BoundingBox(mid_min, max);
    return { left,right };
}