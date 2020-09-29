cmake_minimum_required(VERSION 3.15)
project(3sem)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1 main.cpp catch2.h catch2.cpp)

add_executable(test_sorting sorting/test_sorting.cpp sorting/sorting_control.cpp)

add_executable(test_sorting_with_catch catch2.cpp sorting/test_sorting_with_catch.cpp sorting/sorting_control.cpp)

add_executable(test_array sequence/tests/test_dynamic_array.cpp)

add_executable(test_array_with_catch catch2.cpp sequence/tests/test_array_with_catch.cpp)

add_executable(test_sequence sequence/tests/test_sequence.cpp)