#include "../../catch2.h"

#include "../dynamic_array.h"

using sem2::DynamicArray;

TEMPLATE_TEST_CASE("check compatibility to different types and classes", "", int, double, std::shared_ptr<int>, std::shared_ptr<double>, DynamicArray<int>, DynamicArray<double>) {
    SECTION("empty") {
        auto arr = DynamicArray<int>();
        REQUIRE(arr.getSize() == 0);
    }

    SECTION("with 0 items") {
        auto arr = DynamicArray<int>(0);
        REQUIRE(arr.getSize() == 0);
    }

    SECTION("with 5 items") {
        auto arr = DynamicArray<int>(5);
        REQUIRE(arr.getSize() == 5);
    }
}


TEST_CASE("set & get") {
    SECTION("int") {
        auto array = DynamicArray<int>(10);
        array.set(0, 3);
        array.set(1, 5);
        array.set(2, 0);
        REQUIRE(array.get(0) == 3);
        REQUIRE(array.get(1) == 5);
        REQUIRE(array.get(2) == 0);

        array.get(0) = 1;
        array.get(1) = 2;
        array.get(2) = 3;
        REQUIRE(array.get(0) == 1);
        REQUIRE(array.get(1) == 2);
        REQUIRE(array.get(2) == 3);
    }
    SECTION("std::shared_ptr<DynamicArray<int>>") {
        auto array = DynamicArray<std::shared_ptr<DynamicArray<int>>>(10);

        auto smallArray = std::make_shared<DynamicArray<int>>(5);
        smallArray->set(0, 1);
        smallArray->set(1, 2);
        smallArray->set(2, 3);

        array.set(5, smallArray);
        REQUIRE(array.get(5)->get(2) == 3);
        smallArray->set(2, 4);
        REQUIRE(array.get(5)->get(2) == 4);
    }
}

TEST_CASE("operator[]") {
    auto array = DynamicArray<int>(10);
    array[0] = 3;
    array[1] = 5;
    array[2] = 0;
    REQUIRE(array.get(0) == 3);
    REQUIRE(array.get(1) == 5);
    REQUIRE(array.get(2) == 0);

    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    REQUIRE(array.get(0) == 1);
    REQUIRE(array.get(1) == 2);
    REQUIRE(array.get(2) == 3);
}

TEST_CASE("resize") {
    SECTION("from 10 to 10") {
        auto array = DynamicArray<int>(10);
        REQUIRE(array.getSize() == 10);
        array.resize(10);
        REQUIRE(array.getSize() == 10);
    }

    SECTION("from empty to 10") {
        auto array = DynamicArray<int>();
        REQUIRE(array.getSize() == 0);
        array.resize(10);
        REQUIRE(array.getSize() == 10);
    }

    SECTION("from 0 to 10") {
        auto array = DynamicArray<int>(0);
        REQUIRE(array.getSize() == 0);
        array.resize(10);
        REQUIRE(array.getSize() == 10);
    }

    SECTION("from 10 to 0") {
        auto array = DynamicArray<int>(10);
        REQUIRE(array.getSize() == 10);
        array.resize(0);
        REQUIRE(array.getSize() == 0);
    }

    SECTION("from 10 to 20") {
        auto array = DynamicArray<int>(10);
        REQUIRE(array.getSize() == 10);
        array.resize(20);
        REQUIRE(array.getSize() == 20);
    }

    SECTION("from 20 to 10") {
        auto array = DynamicArray<int>(20);
        REQUIRE(array.getSize() == 20);
        array.resize(10);
        REQUIRE(array.getSize() == 10);
    }
}
