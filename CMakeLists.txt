cmake_minimum_required(VERSION 3.15)
project(3sem)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1 main.cpp)

add_executable(test_sorting sorting/test_sorting.cpp sorting/sorting_control.cpp)

add_executable(quicktest quicktest.cpp)

add_executable(test_array sequence/tests/test_dynamic_array.cpp)