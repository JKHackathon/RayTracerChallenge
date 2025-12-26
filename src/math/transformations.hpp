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

    static Transform translation(float x, float y, float z);

    static Transform scaling(float x, float y, float z);

    static Transform rotation_x(float rad);
    static Transform rotation_y(float rad);
    static Transform rotation_z(float rad);

    static Transform shearing(float x_y, float x_z, float y_x, float y_z,
                              float z_x, float z_y);

    // TODO: what does step 3 mean? (pg 99)
    static Transform view_transform(Point from, Point to, Vector up);
};