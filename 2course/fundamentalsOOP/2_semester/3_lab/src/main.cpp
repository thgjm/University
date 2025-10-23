/**
 * @file main.cpp
 * @author Valeria
 * @brief Matrix multiplication performance comparison program
 * @version 0.1
 *
 * @copyright Copyright (c) 2025
 *
 * This program demonstrates and compares the performance of sequential
 * and parallel matrix multiplication algorithms. It allows users to
 * specify matrix sizes and the number of threads for parallel computation.
 */

#include "../headers/SequentialMultiplier.h"
#include "../headers/ParallelMultiplier.h"
#include <chrono>
#include <iomanip>
#include <memory>
#include <thread>
#include <limits>

const size_t MAX_PRINT = 15; // the maximum size of matrice that could be printed out

/**
 * @brief Prints the input matrices and their multiplication result
 *
 * @param a First input matrix
 * @param b Second input matrix
 * @param result Result of multiplying matrices a and b
 */
void printMatrixOperation(const Matrix &a, const Matrix &b, const Matrix &result)
{
    cout << "\nMatrix A:\n"
         << a;
    cout << "\nMatrix B:\n"
         << b;
    cout << "\nResult Matrix (A * B):\n"
         << result;
    cout << "\n";
}

/**
 * @brief Performs matrix multiplication and measures execution time
 *
 * @param a First input matrix
 * @param b Second input matrix
 * @param multiplier The multiplication algorithm to use
 */
void runTest(const Matrix &a, const Matrix &b, MatrixMultiplier &multiplier)
{
    auto start = chrono::high_resolution_clock::now();

    Matrix result = multiplier.multiply(a, b);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << multiplier.getName() << " Multiplication took: "
         << duration.count() << " ms\n";

    // print matrices if they're not too big
    if (a.getRows() <= MAX_PRINT && a.getCols() <= MAX_PRINT &&
        b.getRows() <= MAX_PRINT && b.getCols() <= MAX_PRINT)
    {
        printMatrixOperation(a, b, result);
    }
}

/**
 * @brief Gets valid input from user within specified range
 *
 * @tparam T Type of input (usually size_t)
 * @param prompt Message to display to user
 * @param min_val Minimum allowed value
 * @param max_val Maximum allowed value
 * @return T User's validated input
 */
template <typename T>
T getInput(const string &prompt, T min_val, T max_val)
{
    T value;
    while (true)
    {
        cout << prompt;
        if (cin >> value && value >= min_val && value <= max_val)
        {
            break;
        }
        cout << "Invalid input. Please try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

/**
 * @brief Main program entry point
 *
 * Allows user to input matrix size and thread count, then performs
 * both sequential and parallel matrix multiplication, comparing
 * their execution times.
 *
 * @return int Exit code (0 for success)
 */
int main()
{
    // get available CPU threads
    size_t max_threads = thread::hardware_concurrency();
    cout << "Your CPU has " << max_threads << " available threads.\n\n";

    // get matrix size from user
    size_t size = getInput<size_t>(
        "Enter matrix size (1-10000): ",
        1,
        10000);

    // get number of threads from user
    size_t numThreads = getInput<size_t>(
        "Enter number of threads (1-" + to_string(max_threads) + "): ",
        1,
        max_threads);

    cout << "\nConfiguration:\n"
         << "- Matrix size: " << size << "x" << size << "\n"
         << "- Number of threads: " << numThreads << "\n"
         << "- Available CPU threads: " << max_threads << "\n\n";

    // test matrices
    Matrix a(size, size);
    Matrix b(size, size);

    cout << "Initializing matrices...\n";
    a.randomize();
    b.randomize();

    // multipliers
    SequentialMultiplier seqMult;
    ParallelMultiplier parMult(numThreads);

    cout << "\nRunning sequential multiplication...\n";
    runTest(a, b, seqMult);

    cout << "\nRunning parallel multiplication...\n";
    runTest(a, b, parMult);

    return 0;
}