#pragma once
#include "MatrixMultiplier.h"
#include <stdexcept>

/**
 * @brief Sequential implementation of matrix multiplication
 *
 * This class implements matrix multiplication using a standard sequential
 * algorithm. It serves as a baseline for comparing performance with
 * parallel implementations.
 */
class SequentialMultiplier : public MatrixMultiplier
{
public:
    /**
     * @brief Multiplies two matrices sequentially
     * @param a First matrix
     * @param b Second matrix
     * @return Matrix - result of matrix multiplication
     */
    Matrix multiply(const Matrix &a, const Matrix &b) override;

    /**
     * @brief Gets the name of the multiplication algorithm
     * @return  const char* - "Sequential" as the algorithm identifier
     */
    const char *getName() const override { return "Sequential"; }
};

Matrix SequentialMultiplier::multiply(const Matrix &a, const Matrix &b)
{
    validateMatrices(a, b);

    Matrix result(a.getRows(), b.getCols());

    for (size_t i = 0; i < a.getRows(); ++i)
    {
        for (size_t j = 0; j < b.getCols(); ++j)
        {
            double sum = 0.0;
            for (size_t k = 0; k < a.getCols(); ++k)
            {
                sum += a.at(i, k) * b.at(k, j);
            }
            result.at(i, j) = sum;
        }
    }

    return result;
}