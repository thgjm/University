#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../headers/doctest.h"
#include "../headers/SequentialMultiplier.h"
#include "../headers/ParallelMultiplier.h"
#include <vector>
#include <cmath>

// helper function to compare matrices
bool matricesAreEqual(const Matrix &a, const Matrix &b, double epsilon = 1e-10)
{
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
    {
        return false;
    }

    for (size_t i = 0; i < a.getRows(); ++i)
    {
        for (size_t j = 0; j < a.getCols(); ++j)
        {
            if (abs(a.at(i, j) - b.at(i, j)) > epsilon)
            {
                return false;
            }
        }
    }
    return true;
}

TEST_CASE("Matrix Construction and Basic Operations")
{
    SUBCASE("Matrix construction with dimensions")
    {
        Matrix m(2, 3);
        CHECK(m.getRows() == 2);
        CHECK(m.getCols() == 3);
        for (size_t i = 0; i < m.getRows(); ++i)
        {
            for (size_t j = 0; j < m.getCols(); ++j)
            {
                CHECK(m.at(i, j) == 0.0);
            }
        }
    }

    SUBCASE("Matrix construction with data")
    {
        vector<vector<double>> data = {{1.0, 2.0}, {3.0, 4.0}};
        Matrix m(data);
        CHECK(m.getRows() == 2);
        CHECK(m.getCols() == 2);
        CHECK(m.at(0, 0) == 1.0);
        CHECK(m.at(0, 1) == 2.0);
        CHECK(m.at(1, 0) == 3.0);
        CHECK(m.at(1, 1) == 4.0);
    }

    SUBCASE("Matrix randomization")
    {
        Matrix m(3, 3);
        m.randomize();
        bool hasNonZero = false;
        for (size_t i = 0; i < m.getRows(); ++i)
        {
            for (size_t j = 0; j < m.getCols(); ++j)
            {
                if (m.at(i, j) != 0.0)
                {
                    hasNonZero = true;
                    break;
                }
            }
        }
        CHECK(hasNonZero);
    }
}

TEST_CASE("Matrix Multiplication Validation")
{
    SUBCASE("Can multiply check")
    {
        Matrix a(2, 3); // 2×3 matrix
        Matrix b(3, 2); // 3×2 matrix
        Matrix c(2, 4); // 2×4 matrix
        Matrix d(4, 2); // 4×2 matrix

        // a(2×3) × b(3×2) = valid (3 == 3)
        CHECK(Matrix::canMultiply(a, b) == true);

        // b(3×2) × a(2×3) = invalid (2 == 2)
        CHECK(Matrix::canMultiply(b, a) == true);

        // a(2×3) × c(2×4) = invalid (3 != 2)
        CHECK(Matrix::canMultiply(a, c) == false);

        // c(2×4) × d(4×2) = valid (4 == 4)
        CHECK(Matrix::canMultiply(c, d) == true);
    }

    SUBCASE("Simple multiplication verification")
    {
        vector<vector<double>> dataA = {{1.0, 2.0}, {3.0, 4.0}};
        vector<vector<double>> dataB = {{5.0, 6.0}, {7.0, 8.0}};
        vector<vector<double>> expectedData = {{19.0, 22.0}, {43.0, 50.0}};

        Matrix a(dataA);
        Matrix b(dataB);
        Matrix expected(expectedData);

        SequentialMultiplier seqMult;
        ParallelMultiplier parMult;

        Matrix seqResult = seqMult.multiply(a, b);
        Matrix parResult = parMult.multiply(a, b);

        CHECK(matricesAreEqual(seqResult, expected));
        CHECK(matricesAreEqual(parResult, expected));
        CHECK(matricesAreEqual(seqResult, parResult));
    }
}

TEST_CASE("Performance Comparison")
{
    SUBCASE("Small matrix multiplication (5x5)")
    {
        Matrix a(5, 5);
        Matrix b(5, 5);
        a.randomize();
        b.randomize();

        SequentialMultiplier seqMult;
        ParallelMultiplier parMult;

        Matrix seqResult = seqMult.multiply(a, b);
        Matrix parResult = parMult.multiply(a, b);

        CHECK(matricesAreEqual(seqResult, parResult));
    }

    SUBCASE("Medium matrix multiplication (50x50)")
    {
        Matrix a(50, 50);
        Matrix b(50, 50);
        a.randomize();
        b.randomize();

        SequentialMultiplier seqMult;
        ParallelMultiplier parMult;

        Matrix seqResult = seqMult.multiply(a, b);
        Matrix parResult = parMult.multiply(a, b);

        CHECK(matricesAreEqual(seqResult, parResult));
    }
}

TEST_CASE("Edge Cases and Error Handling")
{
    SUBCASE("Empty matrices")
    {
        Matrix a(0, 0);
        Matrix b(0, 0);

        SequentialMultiplier seqMult;
        ParallelMultiplier parMult;

        CHECK_THROWS(seqMult.multiply(a, b));
        CHECK_THROWS(parMult.multiply(a, b));
    }

    SUBCASE("1x1 matrices")
    {
        vector<vector<double>> dataA = {{2.0}};
        vector<vector<double>> dataB = {{3.0}};
        Matrix a(dataA);
        Matrix b(dataB);

        SequentialMultiplier seqMult;
        ParallelMultiplier parMult;

        Matrix seqResult = seqMult.multiply(a, b);
        Matrix parResult = parMult.multiply(a, b);

        CHECK(seqResult.at(0, 0) == 6.0);
        CHECK(parResult.at(0, 0) == 6.0);
    }

    SUBCASE("Incompatible dimensions")
    {
        Matrix a(2, 3);
        Matrix b(2, 2);

        SequentialMultiplier seqMult;
        ParallelMultiplier parMult;

        CHECK_THROWS(seqMult.multiply(a, b));
        CHECK_THROWS(parMult.multiply(a, b));
    }
}