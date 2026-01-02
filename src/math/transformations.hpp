#pragma once

#include <cassert>

#include "matrix.hpp"

struct Transform final : public Matrix {
    Transform() : Matrix(identity_matrix4) {}

    explicit Transform(const Matrix& m) : Matrix(m) {
        assert(m.rows == 4 && m.cols == 4 && "Transform must be a 4x4 matrix");
    }

    Transform inverse() const { return Transform(Matrix::inverse()); }
    using Matrix::operator*;  // expose overload
    Transform operator*(const Transform& other) const {
        return Transform(Matrix::operator*(other));
    }

    static Transform translation(double x, double y, double z);

    static Transform scaling(double x, double y, double z);

    static Transform rotation_x(double rad);
    static Transform rotation_y(double rad);
    static Transform rotation_z(double rad);

    static Transform shearing(double x_y, double x_z, double y_x, double y_z,
        double z_x, double z_y);

    // TODO: what does step 3 mean? (pg 99)
    static Transform view_transform(Point from, Point to, Vector up);
};