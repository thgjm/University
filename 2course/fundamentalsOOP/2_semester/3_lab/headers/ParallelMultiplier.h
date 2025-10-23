#pragma once
#include "MatrixMultiplier.h"
#include <thread>
#include <stdexcept>

/**
 * @brief Parallel implementation of matrix multiplication
 *
 * This class implements matrix multiplication using multiple threads.
 * The matrix is divided into horizontal strips, with each strip being
 * processed by a separate thread. The number of threads can be specified
 * at construction time.
 */
class ParallelMultiplier : public MatrixMultiplier
{
private:
    size_t numThreads; // Number of threads to use for multiplication

    /**
     * @brief Multiplies a portion of matrices
     * @param a First matrix
     * @param b Second matrix
     * @param result Output matrix to store results
     * @param startRow Starting row for this thread's work
     * @param endRow Ending row (exclusive) for this thread's work
     */
    void multiplyRange(const Matrix &a, const Matrix &b, Matrix &result,
                       size_t startRow, size_t endRow);

public:
    /**
     * @brief Construct a new Parallel Multiplier object
     *
     * @param numThreads
     */
    explicit ParallelMultiplier(size_t numThreads = thread::hardware_concurrency());

    /**
     * @brief Multiplies two matrices in parallel
     * @param a First matrix
     * @param b Second matrix
     * @return Matrix - result of matrix multiplication
     */
    Matrix multiply(const Matrix &a, const Matrix &b) override;

    /**
     * @brief Gets the name of the multiplication algorithm
     * @return  const char* - "Parallel" as the algorithm identifier
     */
    const char *getName() const override { return "Parallel"; }
};

ParallelMultiplier::ParallelMultiplier(size_t numThreads)
    : numThreads(numThreads) {}

void ParallelMultiplier::multiplyRange(const Matrix &a, const Matrix &b,
                                       Matrix &result, size_t startRow, size_t endRow)
{
    for (size_t i = startRow; i < endRow; ++i)
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
}

Matrix ParallelMultiplier::multiply(const Matrix &a, const Matrix &b)
{
    validateMatrices(a, b);

    Matrix result(a.getRows(), b.getCols());
    vector<thread> threads;

    // calculate rows per thread
    size_t rowsPerThread = a.getRows() / numThreads;
    size_t remainingRows = a.getRows() % numThreads;

    size_t startRow = 0;

    // create and launch threads
    for (size_t i = 0; i < numThreads; ++i)
    {
        size_t threadRows = rowsPerThread + (i < remainingRows ? 1 : 0);
        size_t endRow = startRow + threadRows;

        threads.emplace_back(&ParallelMultiplier::multiplyRange, this,
                             ref(a), ref(b), ref(result),
                             startRow, endRow);

        startRow = endRow;
    }

    // wait for all threads to complete
    for (auto &thread : threads)
    {
        thread.join();
    }

    return result;
}