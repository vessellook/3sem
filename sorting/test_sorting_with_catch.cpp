#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "../catch2.h"

#include "quicksort.h"
#include "heapsort.h"

using namespace sem3;

void test_correctness(const sem3::Sorting<int> &sorting) {

    items[0] = 16;
    items[1] = 1;
    items[2] = 4;
    items[3] = 8;
    items[4] = 2;

    sem2::ISequence<int> *seq = new sem2::ArraySequence<int>(items, 5);
    seq = sorting.sort(*seq);
    assert_equal(seq->get(0), 1, __FILE__, __LINE__);
    assert_equal(seq->get(1), 2, __FILE__, __LINE__);
    assert_equal(seq->get(2), 4);
    assert_equal(seq->get(3), 8);
    assert_equal(seq->get(4), 16);
}

template<typename T>
void big_test(const sem3::Sorting<T> &sorting, unsigned count = 1'000'000, std::string hint = std::string()) {


}

TEST_CASE() {
    int *items = new int[5];
    items[0] = 1;
    items[1] = 16;
    items[2] = 4;
    items[3] = 8;
    items[4] = 2;

    SECTION("QuickSort") {
        auto sorting = QuickSort<int>();
        sorting.sort(items, 5);
        REQUIRE(items[0] == 1);
        REQUIRE(items[1] == 2);
        REQUIRE(items[2] == 4);
        REQUIRE(items[3] == 8);
        REQUIRE(items[4] == 16);
    }

    SECTION("HeapSort") {
        auto sorting = HeapSort<int>();
        sorting.sort(items, 5);
        REQUIRE(items[0] == 1);
        REQUIRE(items[1] == 2);
        REQUIRE(items[2] == 4);
        REQUIRE(items[3] == 8);
        REQUIRE(items[4] == 16);
    }
}


TEST_CASE("Big array") {
    unsigned COUNT = 1'000'000;
    unsigned MAX_VALUE = 1'000'000;
    std::random_device randD;
    std::mt19937 randMT(randD());
    std::uniform_int_distribution<int> range(0, MAX_VALUE);
    auto *array = new int[COUNT];
    for (unsigned i = 0; i < COUNT; i++) {
        array[i] = range(randMT);
    }

    BENCHMARK_ADVANCED("QuickSort")(const QuickSort<int> &sorting) {
            sorting.sort(array, COUNT);
        };

    BENCHMARK_ADVANCED("HeapSort")(const HeapSort<int> &sorting) {
            sorting.sort(array, COUNT);
        };
}
