#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <iomanip>
#include <stdexcept>

using namespace std;

/**
 * @brief A class representing a 2D matrix with basic operations
 *
 * This class implements a matrix using a 2D vector and provides basic
 * matrix operations including construction, element access, and multiplication checks.
 * The class is designed to work with both sequential and parallel multiplication algorithms.
 */
class Matrix
{
private:
    vector<vector<double>> data; // The 2D vector storing matrix elements
    size_t rows;                 // Number of rows in the matrix
    size_t cols;                 // Number of columns in the matrix

public:
    /**
     * @brief Construct a new Matrix object
     * @param rows Number of rows in the matrix
     * @param cols Number of columns in the matrix
     */
    Matrix(size_t rows, size_t cols);

    /**
     * @brief Construct a new Matrix object
     *
     * @param data
     */
    Matrix(const vector<vector<double>> &data);

    /**
     * @brief Gets the number of rows in the matrix
     * @return size_t - number of rows
     */
    size_t getRows() const { return rows; }

    /**
     * @brief Gets the number of columns in the matrix
     * @return size_t - number of columns
     */
    size_t getCols() const { return cols; }

    /**
     * @brief Accesses matrix element at specified position
     * @param i Row index (0-based)
     * @param j Column index (0-based)
     * @return Reference to the element at position (i,j)
     * @throw std::out_of_range if indices are out of bounds
     */
    double &at(size_t i, size_t j) { return data[i][j]; }

    /**
     * @brief Accesses matrix element at specified position (const version)
     * @param i Row index (0-based)
     * @param j Column index (0-based)
     * @return const double&
     */
    const double &at(size_t i, size_t j) const { return data[i][j]; }

    /**
     * @brief Fills the matrix with random values between 0 and 1
     */
    void randomize();

    /**
     * @brief Prints the matrix to standard output
     */
    void print() const;

    /**
     * @brief Checks if two matrices can be multiplied
     * @param a First matrix
     * @param b Second matrix
     * @return true if matrices can be multiplied (a.cols == b.rows), false otherwise
     */
    static bool canMultiply(const Matrix &a, const Matrix &b);

    /**
     * @brief Stream insertion operator for Matrix
     *
     * @param os
     * @param matrix
     * @return ostream&
     */
    friend ostream &operator<<(ostream &os, const Matrix &matrix);
};

Matrix::Matrix(size_t rows, size_t cols)
    : rows(rows), cols(cols), data(rows, vector<double>(cols, 0.0)) {}

Matrix::Matrix(const vector<vector<double>> &data)
    : data(data), rows(data.size()), cols(data[0].size()) {}

void Matrix::randomize()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            data[i][j] = dis(gen);
        }
    }
}

void Matrix::print() const
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            cout << fixed << setprecision(2) << data[i][j] << " ";
        }
        cout << "\n";
    }
}

bool Matrix::canMultiply(const Matrix &a, const Matrix &b)
{
    return a.getCols() == b.getRows();
}

ostream &operator<<(ostream &os, const Matrix &matrix)
{
    for (size_t i = 0; i < matrix.rows; ++i)
    {
        for (size_t j = 0; j < matrix.cols; ++j)
        {
            os << fixed << setprecision(2) << matrix.data[i][j] << " ";
        }
        os << "\n";
    }
    return os;
}