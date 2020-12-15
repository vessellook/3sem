#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "../catch2/catch2.h"

#include "quicksort.h"
#include "heapsort.h"

using namespace sem3;

TEST_CASE("test correctness") {
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


//TEST_CASE("Big array") {
//    unsigned COUNT = 1'000'000;
//    unsigned MAX_VALUE = 1'000'000;
//    std::random_device randD;
//    std::mt19937 randMT(randD());
//    std::uniform_int_distribution<int> range(0, MAX_VALUE);
//    auto *array = new int[COUNT];
//    for (unsigned i = 0; i < COUNT; i++) {
//        array[i] = range(randMT);
//    }
//
//    BENCHMARK("QuickSort") {
//            const QuickSort<int> sorting;
//            sorting.sort(array, COUNT);
//        };
//
//    BENCHMARK("HeapSort") {
//            const HeapSort<int> sorting;
//            sorting.sort(array, COUNT);
//        };
//}
