#ifndef TUPLES_HPP
#define TUPLES_HPP

#include <stdio.h>

#include <cassert>

#include "util.hpp"

// Forward declarations
struct Point;
struct Vector;

struct Tuple {
    float x;
    float y;
    float z;
    float w;

    Tuple(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    bool is_point() const { return float_equal(w, 1.0); }
    bool is_vector() const { return float_equal(w, 0.0); }

    static Tuple from_point(const Point& p);
    static Tuple from_vector(const Vector& v);

    operator Point() const;

    operator Vector() const;

    bool operator==(const Tuple& other) const {
        return float_equal(x, other.x) && float_equal(y, other.y) &&
               float_equal(z, other.z) && float_equal(w, other.w);
    }

    Tuple operator+(const Tuple& other) const {
        return Tuple(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Tuple operator-(const Tuple& other) const {
        return Tuple(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Tuple operator*(const float c) const {
        return Tuple(x * c, y * c, z * c, w * c);
    }

    Tuple operator/(const float c) const {
        return Tuple(x / c, y / c, z / c, w / c);
    }
};

inline Tuple operator*(const float c, const Tuple& tuple) {
    return Tuple(tuple.x * c, tuple.y * c, tuple.z * c, tuple.w * c);
}

// TODO: should include overloads for operator* and operator/???
struct Point : public Tuple {
    Point() : Tuple(0, 0, 0, 1) {}
    Point(float x, float y, float z) : Tuple(x, y, z, 1.0) {}
    // explicit Point(const Tuple& t) : Tuple(t.x, t.y, t.z, 1) {}
};

struct Vector : public Tuple {
    Vector() : Tuple(0, 0, 0, 0) {}
    Vector(float x, float y, float z) : Tuple(x, y, z, 0.0) {}
    // explicit Vector(const Tuple& t) : Tuple(t.x, t.y, t.z, 0) {}

    using Tuple::operator-;
    Vector operator-() const {  // slight deviation from book as points should
                                // not be allowed to be negated
        return Vector(-x, -y, -z);
    }

    float magnitude() const {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
    }

    Vector normalized() const {
        float magnitude = this->magnitude();
        return Vector(x / magnitude, y / magnitude, z / magnitude);
    }

    // Gives angle relationship between. 1 = same dir, -1 = opp dir, 0 = 90°
    float dot(const Vector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Gives perpendicular vector
    Vector cross(const Vector& other) const {
        return Vector(y * other.z - z * other.y, z * other.x - x * other.z,
                      x * other.y - y * other.x);
    }

    Vector reflect(const Vector& normal) const {
        return *this - 2 * this->dot(normal) * normal;
    }
};

inline Tuple Tuple::from_point(const Point& p) {
    return Tuple(p.x, p.y, p.z, 1.0);
}

inline Tuple Tuple::from_vector(const Vector& v) {
    return Tuple(v.x, v.y, v.z, 0.0);
}

inline Tuple::operator Point() const {
    assert(is_point() && "Cannot convert to a point");
    return Point(x, y, z);
}

inline Tuple::operator Vector() const {
    assert(is_vector() && "Cannot convert to a vector");
    return Vector(x, y, z);
}

#endif