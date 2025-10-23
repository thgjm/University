// multiplier interface
// strategy pattern used

#pragma once
#include "Matrix.h"
#include <stdexcept>

/**
 * @brief Abstract base class for matrix multiplication algorithms
 *
 * This interface defines the contract for different matrix multiplication
 * implementations. It provides a common interface for both sequential
 * and parallel multiplication strategies.
 */
class MatrixMultiplier
{
protected:
    /**
     * @brief Validates matrices for multiplication
     * @param a First matrix
     * @param b Second matrix
     */
    void validateMatrices(const Matrix &a, const Matrix &b) const
    {
        if (a.getRows() == 0 || a.getCols() == 0 || b.getRows() == 0 || b.getCols() == 0)
        {
            throw std::invalid_argument("Cannot multiply empty matrices");
        }
        if (!Matrix::canMultiply(a, b))
        {
            throw std::invalid_argument("Matrix dimensions are not compatible for multiplication");
        }
    }

public:
    /**
     * @brief Multiplies two matrices
     * @param a First matrix
     * @param b Second matrix
     * @return Matrix - result of matrix multiplication
     */
    virtual Matrix multiply(const Matrix &a, const Matrix &b) = 0;

    /**
     * @brief Gets the name of the multiplication algorithm
     * @return const char* - string identifier for the algorithm
     */
    virtual const char *getName() const = 0;

    /**
     * @brief Virtual destructor
     */
    virtual ~MatrixMultiplier() = default;
};