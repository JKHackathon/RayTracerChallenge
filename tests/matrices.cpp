#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../src/math/matrix.hpp"

TEST_CASE("Constructing and inspecting a 4x4 matrix", "[matrices]") {
    Matrix m(4, 4);
    m = {1, 2, 3, 4, 5.5, 6.5, 7.5, 8.5, 9, 10, 11, 12, 13.5, 14.5, 15.5, 16.5};

    REQUIRE(float_equal(m(0, 0), 1));
    REQUIRE(float_equal(m(0, 3), 4));
    REQUIRE(float_equal(m(1, 0), 5.5));
    REQUIRE(float_equal(m(1, 2), 7.5));
    REQUIRE(float_equal(m(2, 2), 11));
    REQUIRE(float_equal(m(3, 0), 13.5));
    REQUIRE(float_equal(m(3, 2), 15.5));
}

TEST_CASE("A 2x2 matrix ought to be representable", "[matrices]") {
    Matrix m(2, 2);
    m = {-3, 5, 1, -2};

    REQUIRE(float_equal(m(0, 0), -3));
    REQUIRE(float_equal(m(0, 1), 5));
    REQUIRE(float_equal(m(1, 0), 1));
    REQUIRE(float_equal(m(1, 1), -2));
}

TEST_CASE("A 3x3 matrix ought to be representable", "[matrices]") {
    Matrix m(3, 3);
    m = {-3, 5, 0, 1, -2, -7, 0, 1, 1};

    REQUIRE(float_equal(m(0, 0), -3));
    REQUIRE(float_equal(m(1, 1), -2));
    REQUIRE(float_equal(m(2, 2), 1));
}

TEST_CASE("Matrix equality with identical matrices", "[matrices]") {
    Matrix A(4, 4);
    Matrix B(4, 4);
    std::initializer_list<float> values = {1, 2, 3, 4, 5, 6, 7, 8,
                                           9, 8, 7, 6, 5, 4, 3, 2};
    A = values;
    B = values;

    REQUIRE(A == B);
}

TEST_CASE("Matrix equality with different matrices", "[matrices]") {
    Matrix A(4, 4);
    Matrix B(4, 4);
    A = {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2};
    B = {2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    REQUIRE(A != B);
}

TEST_CASE("Multiplying two matrices", "[matrices]") {
    Matrix A(4, 4);
    A = {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2};
    Matrix B(4, 4);
    B = {-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8};

    Matrix product(4, 4);
    product = {20, 22, 50,  48,  44, 54, 114, 108,
               40, 58, 110, 102, 16, 26, 46,  42};

    REQUIRE(A * B == product);
}

TEST_CASE("A matrix multiplied by a tuple", "[matrices]") {
    Matrix A(4, 4);
    A = {1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1};
    Tuple b = Tuple(1, 2, 3, 1);

    REQUIRE(A * b == Tuple(18, 24, 33, 1));
}

TEST_CASE("Multiplying a matrix by the identity matrix", "[matrices]") {
    Matrix A(4, 4);
    A = {0, 1, 2, 4, 1, 2, 4, 8, 2, 4, 8, 16, 4, 8, 16, 32};

    REQUIRE(A * identity_matrix4 == A);
}

TEST_CASE("Transposing a matrix", "[matrices]") {
    Matrix A(4, 4);
    A = {0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8};
    Matrix A_transpose(4, 4);
    A_transpose = {0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8};

    REQUIRE(A.transpose() == A_transpose);
}

TEST_CASE("Transposing the identity matrix", "[matrices]") {
    REQUIRE(identity_matrix4.transpose() == identity_matrix4);
}

TEST_CASE("Calculating the determinant of a 2x2 matrix", "[matrices]") {
    Matrix A(2, 2);
    A = {1, 5, -3, 2};
    REQUIRE(A.determinant() == 17);
}

TEST_CASE("A submatrix of a 3x3 matrix is a 2x2 matrix", "[matrices]") {
    Matrix A(3, 3);
    A = {1, 5, 0, -3, 2, 7, 0, 6, -3};
    Matrix A_sub(2, 2);
    A_sub = {-3, 2, 0, 6};

    REQUIRE(A.submatrix(0, 2) == A_sub);
}

TEST_CASE("A submatrix of a 4x4 matrix is a 3x3 matrix", "[matrices]") {
    Matrix A(4, 4);
    A = {-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1};
    Matrix A_sub(3, 3);
    A_sub = {-6, 1, 6, -8, 8, 6, -7, -1, 1};

    REQUIRE(A.submatrix(2, 1) == A_sub);
}

TEST_CASE("Calculating a minor of a 3x3 matrix", "[matrices]") {
    Matrix A(3, 3);
    A = {3, 5, 0, 2, -1, -7, 6, -1, 5};
    Matrix B = A.submatrix(1, 0);

    REQUIRE(B.determinant() == 25);
    REQUIRE(A.minor(1, 0) == 25);
}

TEST_CASE("Calculating a cofactor of a 3x3 matrix", "[matrices]") {
    Matrix A(3, 3);
    A = {3, 5, 0, 2, -1, -7, 6, -1, 5};

    REQUIRE(A.minor(0, 0) == -12);
    REQUIRE(A.cofactor(0, 0) == -12);
    REQUIRE(A.minor(1, 0) == 25);
    REQUIRE(A.cofactor(1, 0) == -25);
}

TEST_CASE("Calculating the determinant of a 3x3 matrix", "[matrices]") {
    Matrix A(3, 3);
    A = {1, 2, 6, -5, 8, -4, 2, 6, 4};

    REQUIRE(A.cofactor(0, 0) == 56);
    REQUIRE(A.cofactor(0, 1) == 12);
    REQUIRE(A.cofactor(0, 2) == -46);
    REQUIRE(A.determinant() == -196);
}

TEST_CASE("Calculating the determinant of a 4x4 matrix", "[matrices]") {
    Matrix A(4, 4);
    A = {-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9};

    REQUIRE(A.cofactor(0, 0) == 690);
    REQUIRE(A.cofactor(0, 1) == 447);
    REQUIRE(A.cofactor(0, 2) == 210);
    REQUIRE(A.cofactor(0, 3) == 51);
    REQUIRE(A.determinant() == -4071);
}

TEST_CASE("Testing an invertible matrix for invertibility", "[matrices]") {
    Matrix A(4, 4);
    A = {6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6};

    REQUIRE(A.determinant() == -2120);
    REQUIRE(A.is_invertible());
}

TEST_CASE("Testing a noninvertible matrix for invertibility", "[matrices]") {
    Matrix A(4, 4);
    A = {-4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0, 0};

    REQUIRE(A.determinant() == 0);
    REQUIRE(!A.is_invertible());
}

TEST_CASE("Calculating the inverse of a matrix", "[matrices]") {
    Matrix A(4, 4);
    A = {-5, 2, 6, -8, 1, -5, 1, 8, 7, 7, -6, -7, 1, -3, 7, 4};
    Matrix B = A.inverse();
    Matrix A_inverse(4, 4);
    A_inverse = {
        0.21805,  0.45113,  0.24060,  -0.04511, -0.80827, -1.45677,
        -0.44361, 0.52068,  -0.07895, -0.22368, -0.05263, 0.19737,
        -0.52256, -0.81391, -0.30075, 0.30639,
    };

    REQUIRE(A.determinant() == 532);
    REQUIRE(A.cofactor(2, 3) == -160);
    REQUIRE(float_equal(B(3, 2), -160.0f / 532));
    REQUIRE(A.cofactor(3, 2) == 105);
    REQUIRE(float_equal(B(2, 3), 105.0f / 532));
    REQUIRE(B == A_inverse);
}

TEST_CASE("Calculating the inverse of 2nd matrix", "[matrices]") {
    Matrix A(4, 4);
    A = {8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4};
    Matrix B = A.inverse();
    Matrix A_inverse(4, 4);
    A_inverse = {-0.15385, -0.15385, -0.28205, -0.53846, -0.07692, 0.12308,
                 0.02564,  0.03077,  0.35897,  0.35897,  0.43590,  0.92308,
                 -0.69231, -0.69231, -0.76923, -1.92308};
    REQUIRE(B == A_inverse);
}

TEST_CASE("Calculating the inverse of 3rd matrix", "[matrices]") {
    Matrix A(4, 4);
    A = {9, 3, 0, 9, -5, -2, -6, -3, -4, 9, 6, 4, -7, 6, 6, 2};
    Matrix B = A.inverse();
    Matrix A_inverse(4, 4);
    A_inverse = {-0.04074, -0.07778, 0.14444,  -0.22222, -0.07778, 0.03333,
                 0.36667,  -0.33333, -0.02901, -0.14630, -0.10926, 0.12963,
                 0.17778,  0.06667,  -0.26667, 0.33333};
    REQUIRE(B == A_inverse);
}

TEST_CASE("Multiplying a product by its inverse", "[matrices]") {
    Matrix A(4, 4);
    A = {3, -9, 7, 3, 3, -8, 2, -9, -4, 4, 4, 1, -6, 5, -1, 1};
    Matrix B(4, 4);
    B = {8, 2, 2, 2, 3, -1, 7, 0, 7, 0, 5, 4, 6, -2, 0, 5};
    Matrix C = A * B;

    REQUIRE(C * B.inverse() == A);
}