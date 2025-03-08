#include "matrix.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
// Empty constructor
Matrix::Matrix() : rows(0), cols(0) {}

// Constructor with row and columsn with zero values
Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    content.resize(r, std::vector<double>(c, 0.0));
}


// Constructor: a matrix from a 2D vector.
Matrix::Matrix(const std::vector<std::vector<double>>& values): content(values), rows(values.size()), cols(values[0].size())
{
  //maybe check for dimensions are accurate
}

// Access for dimensions.
int Matrix::NRows() const { return rows; }
int Matrix::NCols() const { return cols; }


// returns a reference to the element at (i, j).
double Matrix::operator()(int i, int j) const{
    if (i >= rows || j >= cols)
        throw "Index out of range";
    return content[i][j];
}

double& Matrix::operator()(int i, int j) {
    if (i >= rows || j >= cols)
        throw "Index out of range";
    return content[i][j];
}

// Matrix multiplication.
Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows)
        throw "Matrix dimensions not compatible for multiplication";

    Matrix prod(rows, other.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < other.cols; j++) {
            double dot_p = 0;
            for (int k = 0; k < cols; k++) {
                dot_p += (*this)(i, k) * other(k, j);
            }
            prod(i, j) = dot_p;
        }
    }
    return prod;
}



// Transpose of the matrix.
Matrix Matrix::T() const {
    Matrix transpose(cols, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transpose(j, i) = (*this)(i, j);
        }
    }
    return transpose;
}


// Inverse - Gauss-Jordan elimination.
Matrix Matrix::inverse() const {
    if (rows != cols)
        throw "Not a square matrix: Matrix Inversion";

    int n = rows;
  
    Matrix augmented(n, 2 * n);
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            augmented(i, j) = (*this)(i, j);
        }
        for (int j = n; j < 2 * n; j++) {
            double fill =0.0;

            if (i == (j - n)){
                fill = 1;
            }
            augmented(i, j) = fill;
        }
    }

    // Gauss-Jordan elimination.
    for (int i = 0; i < n; i++) {
        int pivot = i;
        for (int r = i + 1; r < n; r++) {
            if (std::abs(augmented(r, i)) > std::abs(augmented(pivot, i)))
                pivot = r;
        }
        if (std::abs(augmented(pivot, i)) < 1e-50)
            throw "Matrix is singular and cannot be inverted. Perhaps increase lookback?";

        if (i != pivot)
            std::swap(augmented.content[i], augmented.content[pivot]);

        double pivotVal = augmented(i, i);
        for (int j = 0; j < 2 * n; j++)
            augmented(i, j) /= pivotVal;

        for (int r = 0; r < n; r++) {
            if (r != i) {
                double factor = augmented(r, i);
                for (int j = 0; j < 2 * n; j++)
                    augmented(r, j) -= factor * augmented(i, j);
            }
        }
    }

    Matrix inverted_mat(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inverted_mat(i, j) = augmented(i, j + n);
        }
    }
    return inverted_mat;
}

//Calculates retuns from N(time) - rows x M (assets) - cols matrix
Matrix Matrix::returns() const {
    if (rows <= 1)
        throw "Not enough data to calculate returns.";

    Matrix ret_mat(rows - 1, cols);
    for (int i = 1; i < rows ; i++) {
        for (int j = 0; j < cols; j++) {
            double price_curr = (*this)(i, j);
            double price_prev = (*this)(i-1, j);
            if (price_prev == 0)
                throw "Price zero : returns";
            ret_mat(i-1, j) = (price_curr - price_prev) / price_prev;
        }
    }
    return ret_mat;
}

// crate submatrix using row slicing 
Matrix Matrix::slicerows(int s, int e) const {
    
    int nrows = e - s ;
    Matrix sliced_mat(nrows, cols);

    for (int r = s; r < e; r++) {   //no <= to make it similar to python
        for (int c = 0; c < cols; c++) {
            sliced_mat(r - s, c) = (*this)(r, c);
        }
    }
    return sliced_mat;
}

// crate submatrix using column slicing 
Matrix Matrix::slicecols(int s, int e) const {
    
    int ncols = e - s ;
    Matrix sliced_mat(rows, ncols);

    for (int r = 0; r < rows; r++) {   //no <= to make it similar to python
        for (int c = s; c < e; c++) {
            sliced_mat(r, c-s) = (*this)(r, c);
        }
    }
    return sliced_mat;
}


// << print the matrix.
std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            os << matrix.content[i][j] << " ";
        }
        os << "\n";
    }
    return os;
}

// Multiply matrix by a scalar (Matrix * Scalar)
Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result(i, j) = (*this)(i, j) * scalar;
        }
    }
    return result;
}

// Multiply scalar by a matrix (Scalar * Matrix) - Friend function
Matrix operator*(double scalar, const Matrix& mat) {
    return mat * scalar;  // Use the existing Matrix * Scalar operator
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix dimensions must match for addition.");

    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result(i, j) = (*this)(i, j) + other(i, j);
        }
    }
    return result;
}

// Function to get the content of the matrix
std::vector<std::vector<double>> Matrix::get_content() const {
    return content;
}