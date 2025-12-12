#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <cassert>

#include "matrix.hpp"

struct Transform final : public Matrix {
    Transform() : Matrix(identity_matrix4) {}

    explicit Transform(const Matrix& m) : Matrix(m) {
        assert(m.rows == 4 && m.cols == 4 && "Transform must be a 4x4 matrix");
    }

    Transform inverse() const { return Transform(Matrix::inverse()); }
    using Matrix::operator*; // expose overload
    Transform operator*(const Transform& other) const {
        return Transform(Matrix::operator*(other));
    }

    static Transform translation(float x, float y, float z) {
        Transform translation;
        translation(0, 3) = x;
        translation(1, 3) = y;
        translation(2, 3) = z;
        return translation;
    }

    static Transform scaling(float x, float y, float z) {
        Transform scaling;
        scaling(0, 0) = x;
        scaling(1, 1) = y;
        scaling(2, 2) = z;
        return scaling;
    }

    static Transform rotation_x(float rad) {
        Transform rotation_x;
        rotation_x(1, 1) = cos(rad);
        rotation_x(1, 2) = -sin(rad);
        rotation_x(2, 1) = sin(rad);
        rotation_x(2, 2) = cos(rad);
        return rotation_x;
    }

    static Transform rotation_y(float rad) {
        Transform rotation_y;
        rotation_y(0, 0) = cos(rad);
        rotation_y(0, 2) = sin(rad);
        rotation_y(2, 0) = -sin(rad);
        rotation_y(2, 2) = cos(rad);
        return rotation_y;
    }

    static Transform rotation_z(float rad) {
        Transform rotation_z;
        rotation_z(0, 0) = cos(rad);
        rotation_z(0, 1) = -sin(rad);
        rotation_z(1, 0) = sin(rad);
        rotation_z(1, 1) = cos(rad);
        return rotation_z;
    }

    static Transform shearing(float x_y, float x_z, float y_x, float y_z,
                              float z_x, float z_y) {
        Transform shearing;
        shearing(0, 1) = x_y;
        shearing(0, 2) = x_z;
        shearing(1, 0) = y_x;
        shearing(1, 2) = y_z;
        shearing(0, 2) = z_x;
        shearing(2, 1) = z_y;
        return shearing;
    }
};

#endif