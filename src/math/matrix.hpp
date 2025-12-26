#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

#include "tuples.hpp"

template <typename T>
concept TupleType = std::is_base_of_v<Tuple, T>;

struct Matrix {
    std::vector<float> data;
    size_t rows;
    size_t cols;

    Matrix(int i, int j) : cols(j), rows(i), data(i * j, 0) {}
    // what is the most efficient

    int coords_to_index(int i, int j) const;

    Matrix& operator=(std::initializer_list<float> values) {
        if (values.size() != rows * cols)
            throw std::runtime_error(
                "Initializer list size does not match matrix size");
        data.assign(values.begin(), values.end());
        return *this;
    }

    float operator()(int i, int j) const { return data[coords_to_index(i, j)]; }
    float& operator()(int i, int j) { return data[coords_to_index(i, j)]; }

    // Faster methods: hash and compare, memcmp w/ vectors
    bool operator==(const Matrix& other) const {
        for (size_t i = 0; i < rows * cols; i++) {
            if (!float_equal(data[i], other.data[i])) {
                return false;
            }
        }
        return true;
    }

    Matrix operator*(const Matrix& other) const;
    Tuple operator*(const Tuple& other) const;

    // template <TupleType T>
    // // Note: Tuple is always a 4x1 in our definition
    // T operator*(const T& other) const {
    //     Matrix b(4, 1);
    //     b = {other.x, other.y, other.z, other.w};
    //     Matrix prod = (*this) * b;
    //     return T(Tuple(prod(0, 0), prod(1, 0), prod(2, 0), prod(3, 0)));
    // } 

    Matrix transpose() const;
    float determinant() const;
    Matrix submatrix(size_t remove_row, size_t remove_col) const;
    float minor(size_t i, size_t j) const;
    float cofactor(size_t i, size_t j) const;

    // Not the most efficient as will require calc determinant twice. Either
    // return det or accept cause book
    bool is_invertible() const { return this->determinant() != 0; }

    Matrix inverse() const;
};

inline const Matrix identity_matrix4 = [] {
    Matrix id(4, 4);
    id = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    return id;
}();

#endif