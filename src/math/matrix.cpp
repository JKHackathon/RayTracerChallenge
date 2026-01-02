#include "matrix.hpp"

#include <cassert>
#include <format>
#include <iostream>

int Matrix::coords_to_index(int i, int j) const {
    assert(j < cols && j >= 0 && "invalid column index");
    assert(i < rows && i >= 0 && "invalid row index");
    return i * cols + j;
}

Matrix Matrix::operator*(const Matrix& other) const {
    assert(cols == other.rows && "invalid multiplication dimensions");
    Matrix m(rows, other.cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j_1 = 0; j_1 < cols; j_1++) {
            double ij_1 = (*this)(i, j_1);
            for (size_t j_2 = 0; j_2 < other.cols; j_2++) {
                m(i, j_2) += ij_1 * other(j_1, j_2);
            }
        }
    }
    return m;
}

// Note: Tuple is always a 4x1 in our definition
Tuple Matrix::operator*(const Tuple& other) const {
    Matrix b(4, 1);
    b = { other.x, other.y, other.z, other.w };
    Matrix prod = (*this) * b;
    return Tuple(prod(0, 0), prod(1, 0), prod(2, 0), prod(3, 0));
}

Matrix Matrix::transpose() const {
    Matrix transpose(cols, rows);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < rows; j++) {
            transpose(j, i) = (*this)(i, j);
        }
    }
    return transpose;
}

double Matrix::determinant() const {
    // if (rows != cols) {
    //     throw std::invalid_argument("cannot find determinant of non-square matrix");
    // }
    assert(rows == cols && "cannot find determinant of non-square matrix");
    if (rows == 2)
        return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);

    double determinant = 0;
    for (size_t j = 0; j < cols; j++) {
        determinant += (*this)(0, j) * this->cofactor(0, j);
    }
    return determinant;
}

Matrix Matrix::submatrix(size_t remove_row, size_t remove_col) const {
    // Typically would add safety to ensure row/col > 0
    Matrix submatrix(rows - 1, cols - 1);
    size_t sub_i = 0;
    for (size_t i = 0; i < rows; i++) {
        if (i == remove_row) continue;

        size_t sub_j = 0;
        for (size_t j = 0; j < cols; j++) {
            if (j == remove_col) continue;
            // std::cout << std::format("{} {}\n\n", sub_i, sub_j);
            submatrix(sub_i, sub_j) = (*this)(i, j);
            sub_j++;
        }
        sub_i++;
    }
    return submatrix;
}

double Matrix::minor(size_t i, size_t j) const {
    Matrix submatrix = this->submatrix(i, j);
    return submatrix.determinant();
}

double Matrix::cofactor(size_t i, size_t j) const {
    double minor = this->minor(i, j);
    return (i + j) % 2 == 0 ? minor : -minor;
}

Matrix Matrix::inverse() const {
    assert(this->is_invertible() && "matrix not invertible");
    Matrix M2(rows, cols);

    double det = this->determinant();
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double c = this->cofactor(i, j);
            M2(j, i) = c / det;
        }
    }
    return M2;
}