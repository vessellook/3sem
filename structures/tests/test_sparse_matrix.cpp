#include <random>

#include "../sparse_matrix.h"
#include "../../asserts.h"
#include "../../benchmark.h"

using tests::assert_equal;
using sem3::SparseMatrix;

void test_correctness() {
    auto matrix = SparseMatrix<int>(1000, 1000);
    ASSERT_EQUAL(matrix.GetSize(), sem3::Pair(1000u, 1000u));

    ASSERT_EQUAL(matrix.get(0, 0), 0);
    ASSERT_EQUAL(matrix.get(700, 999), 0);

    matrix.set(700, 800, -5);
    auto rows = matrix.getNotEmptyRows();
    ASSERT_EQUAL(rows.getLength(), 1u);
    ASSERT_EQUAL(rows.get(0), 800u);
    auto cols = matrix.getColsByRow(800);
    ASSERT_EQUAL(cols.getLength(), 1u);
    ASSERT_EQUAL(cols.get(0), 700u);
    ASSERT_EQUAL(matrix.get(700, 800), -5);

    auto transposed = matrix.transpose();
    ASSERT_EQUAL(transposed.get(800, 700), -5);
    ASSERT_EQUAL(transposed.get(700, 800), 0);

    auto sum = matrix + transposed;
    ASSERT_EQUAL(sum.get(800, 700), -5);
    ASSERT_EQUAL(sum.get(700, 800), -5);

    auto product = matrix * transposed;
    auto r = product.getNotEmptyRows();
    ASSERT_EQUAL(r.get(0), 800u);
    const auto &c = product.getColsByRow(800);
    ASSERT_EQUAL(c.getLength(), 1u);
    ASSERT_EQUAL(c.get(0), 800u);
    ASSERT_EQUAL(product.get(800, 800), 25);

    auto product2 = matrix * 2;
    ASSERT_EQUAL(product2.get(700, 800), -10);

    matrix.set(700, 800, 5);
    ASSERT_EQUAL(matrix.get(700, 800), 5);
    matrix.set(700, 300, 9);
    ASSERT_EQUAL(matrix.get(700, 300), 9);
    matrix.set(0, 200, 18);
    ASSERT_EQUAL(matrix.get(0, 200), 18);
}

void sum_matrices() {
    unsigned MAX_VALUE = 1'000'000;
    std::random_device randD;
    std::mt19937 randMT(randD());
    std::uniform_int_distribution<unsigned> range(0, MAX_VALUE);

    auto first = SparseMatrix<unsigned>(MAX_VALUE + 1, MAX_VALUE + 1);
    for (auto i = 0; i < MAX_VALUE; i++) {
        auto col = range(randMT);
        auto row = range(randMT);
        auto value = range(randMT);
        first.set(col, row, value);
    }

    auto second = SparseMatrix<unsigned>(MAX_VALUE + 1, MAX_VALUE + 1);
    for (auto i = 0; i < MAX_VALUE; i++) {
        auto col = range(randMT);
        auto row = range(randMT);
        auto value = range(randMT);
        second.set(col, row, value);
    }

    std::cout << benchmark([&]() {
        auto sum = first + second;
    }) << " ms" << std::endl;
}

void product_matrices() {
    unsigned MAX_VALUE = 10'000;
    std::random_device randD;
    std::mt19937 randMT(randD());
    std::uniform_int_distribution<unsigned> range(0, MAX_VALUE);

    auto first = SparseMatrix<unsigned>(MAX_VALUE + 1, MAX_VALUE + 1);
    for (auto i = 0; i < MAX_VALUE; i++) {
        auto col = range(randMT);
        auto row = range(randMT);
        auto value = range(randMT);
        first.set(col, row, value);
    }

    auto second = SparseMatrix<unsigned>(MAX_VALUE + 1, MAX_VALUE + 1);
    for (auto i = 0; i < MAX_VALUE; i++) {
        auto col = range(randMT);
        auto row = range(randMT);
        auto value = range(randMT);
        second.set(col, row, value);
    }

    std::cout << benchmark([&]() {
        auto sum = first * second;
    }) << " ms" << std::endl;
}

int main() {
    test_correctness();
//    sum_matrices();
//    product_matrices();
    std::cout << "FINISH";

}