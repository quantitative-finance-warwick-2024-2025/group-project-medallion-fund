# pragma once
#include <vector>
#include <iostream>

class Matrix {
private:
    std::vector<std::vector<double>> content;
    int rows;
    int cols;
public:
    // Constructor
    Matrix();
    Matrix(int rows, int cols);
    
    Matrix(const std::vector<std::vector<double>>& values);
    
    // Accessors
    int NRows() const;
    int NCols() const;
    
    double& operator()(int i, int j);
    double operator()(int i, int j) const;

    // Operations
    Matrix operator*(const Matrix& other) const;  // Matrix multiplication
    Matrix T() const;                     // Transpose matrix
    Matrix inverse() const;                       // Inverse 
    Matrix returns() const;    //Return of given price matrixCalculator
    Matrix slicerows(int s, int e) const;   // crate submatrix from matrix m from (start)sth row to (end)(e-1)th row just like python
    Matrix slicecols(int s, int e) const;
    // printing matrix (optional)
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

    Matrix operator*(double scalar) const;       // Matrix × Scalar

    // Friend function for Scalar × Matrix (to support 2.0 * A)
    friend Matrix operator*(double scalar, const Matrix& mat);
    Matrix operator+(const Matrix& other) const;
    std::vector<std::vector<double>> get_content() const;
};