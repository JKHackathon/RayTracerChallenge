#include "transformations.hpp"

Transform Transform::translation(double x, double y, double z) {
    Transform translation;
    translation(0, 3) = x;
    translation(1, 3) = y;
    translation(2, 3) = z;
    return translation;
}

Transform Transform::scaling(double x, double y, double z) {
    Transform scaling;
    scaling(0, 0) = x;
    scaling(1, 1) = y;
    scaling(2, 2) = z;
    return scaling;
}

Transform Transform::rotation_x(double rad) {
    Transform rotation_x;
    rotation_x(1, 1) = cos(rad);
    rotation_x(1, 2) = -sin(rad);
    rotation_x(2, 1) = sin(rad);
    rotation_x(2, 2) = cos(rad);
    return rotation_x;
}

Transform Transform::rotation_y(double rad) {
    Transform rotation_y;
    rotation_y(0, 0) = cos(rad);
    rotation_y(0, 2) = sin(rad);
    rotation_y(2, 0) = -sin(rad);
    rotation_y(2, 2) = cos(rad);
    return rotation_y;
}

Transform Transform::rotation_z(double rad) {
    Transform rotation_z;
    rotation_z(0, 0) = cos(rad);
    rotation_z(0, 1) = -sin(rad);
    rotation_z(1, 0) = sin(rad);
    rotation_z(1, 1) = cos(rad);
    return rotation_z;
}

Transform Transform::shearing(double x_y, double x_z, double y_x, double y_z,
    double z_x, double z_y) {
    Transform shearing;
    shearing(0, 1) = x_y;
    shearing(0, 2) = x_z;
    shearing(1, 0) = y_x;
    shearing(1, 2) = y_z;
    shearing(0, 2) = z_x;
    shearing(2, 1) = z_y;
    return shearing;
}

Transform Transform::view_transform(Point from, Point to, Vector up) {
    Vector forward = Vector(to - from).normalized();
    Vector left = forward.cross(up.normalized());
    Vector true_up = left.cross(forward);

    Matrix orientation(4, 4);
    orientation = { left.x,     left.y,     left.z,     0,
                   true_up.x,  true_up.y,  true_up.z,  0,
                   -forward.x, -forward.y, -forward.z, 0,
                   0,          0,          0,          1 };
    return Transform(orientation * translation(-from.x, -from.y, -from.z));
}