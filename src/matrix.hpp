#pragma once
#include <vector>
#include <iostream>

class Matrix
{
private:
    std::vector<std::vector<double>> content;
    int rows;
    int cols;

public:
    // Constructor
    Matrix();
    Matrix(int rows, int cols);
    Matrix(const std::vector<std::vector<double>> &values);

    // Accessors
    int NRows() const;
    int NCols() const;
    double &operator()(int i, int j);
    double operator()(int i, int j) const;
    std::vector<double> getrow(int row) const; // Access row of a matrix in vector format
    std::vector<double> getcol(int col) const; // Access column of a matrix
    std::vector<std::vector<double>> get_content() const;

    // Operations
    Matrix operator*(const Matrix &other) const; // Matrix multiplication
    Matrix operator+(const Matrix &other) const; // Matrix addition
    Matrix operator-(const Matrix &other) const; // Matrix subtraction
    Matrix T() const;                            // Transpose matrix
    Matrix inverse() const;                      // Inverse
    Matrix returns() const;                      // Return of given price matrixCalculator
    Matrix slicerows(int s, int e) const;        // crate submatrix from matrix m from (start)sth row to (end)(e-1)th row just like python
    Matrix slicecols(int s, int e) const;        // crate submatrix from matrix m from (start)sth column to (end)(e-1)th column just like python
    Matrix operator*(double scalar) const;       // scalar multiplication
    friend Matrix operator*(double scalar, const Matrix &mat);

    // printing matrix (optional)
    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);
};