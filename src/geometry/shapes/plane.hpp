#include "shapes.hpp"

struct Plane : public Shape {
    BoundingBox bounds_of() const override {
        return BoundingBox(Point(MIN_DOUBLE, 0, MIN_DOUBLE), Point(MAX_DOUBLE, 0, MAX_DOUBLE));
    }
private:
    Vector local_normal_at(const Point local_p, Intersection i) const override {
        return Vector(0, 1, 0);
    }

    IntersectionRecord local_intersect(const Ray local_r) const override {
        if (abs(local_r.dir.y) < EPSILON) {
            return IntersectionRecord();
        }

        double t = -local_r.origin.y / local_r.dir.y;
        return Intersection(t, this);
    }
};