#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "../structures/array_sequence.h"
#include "../structures/linked_list_sequence.h"
#include "quicksort.h"
#include "heapsort.h"
#include "mergesort.h"
#include "../asserts.h"
#include <functional>
#include "../benchmark.h"

using sem3::QuickSort;
using sem3::HeapSort;
using sem3::MergeSort;
using sem3::ISorting;
using tests::assert_equal;


void test_correctness(const sem3::ISorting<int> &sorting, const sem2::ISequence<int> *items,
                      const std::vector<int> &expected) {
    assert_equal<unsigned long>(items->getLength(), expected.size(), __FILE__, __LINE__);
    sem2::ISequence<int> *seq = sorting.sort(items);
    assert_equal<unsigned long>(seq->getLength(), expected.size(), __FILE__, __LINE__);
    for (unsigned i = 0; i < expected.size(); i++) {
        assert_equal(seq->get(i), expected[i], __FILE__, __LINE__);
    }
}

template<typename T>
void big_test(const sem3::ISorting<T> &sorting, unsigned count = 1'000'000, const std::string &hint = "sorting") {
    unsigned MAX_VALUE = 1'000'000;
    std::random_device randD;
    std::mt19937 randMT(randD());
    std::uniform_int_distribution<T> range(0, MAX_VALUE);

    auto *array = new T[count];
    for (unsigned i = 0; i < count; i++) {
        array[i] = range(randMT);
    }


    std::cerr << hint << ": " << benchmark(
            [&sorting, &array, count] {
                sorting.sort(array, count);
            }) << " ms\n";
}

int main() {
    test_correctness(QuickSort<int>(),
                     new sem2::ArraySequence{1, 3, 2, 4},
                     {1, 2, 3, 4});
    test_correctness(HeapSort<int>(),
                     new sem2::LinkedListSequence{1, 3, 2, 4},
                     {1, 2, 3, 4});
    test_correctness(MergeSort<int>(),
                     new sem2::ArraySequence{1, 4, 2, 3},
                     {1, 2, 3, 4});
    big_test(QuickSort<int>(), 10'000'000, "quicksort");
    big_test(HeapSort<int>(), 10'000'000, "heapsort");
    big_test(MergeSort<int>(), 10'000'000, "mergesort");
}