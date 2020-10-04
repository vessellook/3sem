#include <iostream>
#include <random>
#include <string>
#include <ctime>
#include "quicksort.h"
#include "heapsort.h"
#include "mergesort.h"
#include "../asserts.h"
#include <chrono>
#include <functional>
#include "sorting_control.h"

using sem3::QuickSort;
using sem3::HeapSort;
using sem3::MergeSort;
using tests::assert_equal;

void test_correctness(const sem3::Sorting<int> &sorting) {
    int *items = new int[5];
    items[0] = 1;
    items[1] = 16;
    items[2] = 4;
    items[3] = 8;
    items[4] = 2;

    sorting.sort(items, 5);
    assert_equal(items[0], 1, __FILE__, __LINE__);
    assert_equal(items[1], 2, __FILE__, __LINE__);
    assert_equal(items[2], 4);
    assert_equal(items[3], 8);
    assert_equal(items[4], 16);

    items[0] = 16;
    items[1] = 1;
    items[2] = 4;
    items[3] = 8;
    items[4] = 2;

    sem2::ISequence<int> *seq = new sem2::ArraySequence<int>(items, 5);
    seq = sorting.sort(*seq);
    std::cout << seq->getLength() << std::endl;
    std::cout << seq->get(0) << " "
              << seq->get(1) << " "
              << seq->get(2) << " "
              << seq->get(3) << " "
              << seq->get(4) << std::endl;
    assert_equal(seq->get(0), 1, __FILE__, __LINE__);
    assert_equal(seq->get(1), 2, __FILE__, __LINE__);
    assert_equal(seq->get(2), 4);
    assert_equal(seq->get(3), 8);
    assert_equal(seq->get(4), 16);
}

void benchmark(const std::function<void(void)> &callback, const std::string &hint = std::string()) {
    auto start = std::chrono::steady_clock::now();
    callback();
    auto finish = std::chrono::steady_clock::now();
    std::cerr << "benchmark: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << " milliseconds";
    if (!hint.empty()) {
        std::cerr << " (hint: " << hint << ")";
    }
    std::cerr << std::endl;
}

template<typename T>
void big_test(const sem3::Sorting<T> &sorting, unsigned count = 1'000'000, std::string hint = std::string()) {
    unsigned MAX_VALUE = 1'000'000;
    std::random_device randD;
    std::mt19937 randMT(randD());
    std::uniform_int_distribution<T> range(0, MAX_VALUE);

    auto *array = new T[count];
    for (unsigned i = 0; i < count; i++) {
        array[i] = range(randMT);
    }


    benchmark(
            [&sorting, &array, count] {
                sorting.sort(array, count);
            }, hint);
}

int main() {
    test_correctness(QuickSort<int>());
    test_correctness(HeapSort<int>());
    test_correctness(MergeSort<int>());
    big_test(QuickSort<int>(), 1'000'000, "quicksort");
    big_test(HeapSort<int>(), 1'000'000, "heapsort");
    big_test(MergeSort<int>(), 1'000'000, "mergesort");
}