cmake_minimum_required(VERSION 3.15)
project(3sem)

#add_compile_options(-static-libgcc -static-libstdc++)
set(CMAKE_CXX_STANDARD 17)

add_executable(lab3 lab3_task/lab3.cpp)

add_executable(weak_grep regex/weak_grep.cpp regex/regex.cpp regex/tokens/anything.cpp regex/tokens/group.cpp regex/tokens/char.cpp regex/tokens/set.cpp regex/tokens/root.cpp)

add_executable(weak_sed
        regex/regex.cpp
        regex/tokens/anything.cpp
        regex/tokens/group.cpp
        regex/tokens/char.cpp
        regex/tokens/set.cpp
        regex/tokens/root.cpp
        regex/weak_sed.cpp)

add_executable(test_tokens
        regex/tests/test_tokens.cpp
        regex/tokens/root.cpp
        regex/tokens/group.cpp
        regex/tokens/set.cpp
        regex/tokens/anything.cpp
        regex/tokens/char.cpp)

add_executable(matrix
        regex/regex.cpp
        regex/tokens/anything.cpp
        regex/tokens/group.cpp
        regex/tokens/char.cpp
        regex/tokens/set.cpp
        regex/tokens/root.cpp
        matrix/matrix_repl.cpp
        matrix/table_printer.cpp)

add_executable(test_sorted_sequence structures/tests/test_sorted_sequence.cpp)

add_executable(test_hash_map structures/tests/test_hash_map.cpp)

add_executable(test_sparse_matrix structures/tests/test_sparse_matrix.cpp)

#add_executable(test_sorting sorting/test_sorting.cpp sorting/sorting_control.cpp)

#add_executable(test_sorting_with_catch catch2.cpp sorting/test_sorting_with_catch.cpp sorting/sorting_control.cpp)

#add_executable(test_array structures/tests/test_dynamic_array.cpp)

#add_executable(test_array_with_catch catch2.cpp structures/tests/test_array_with_catch.cpp)

#add_executable(test_sequence structures/tests/test_sequence.cpp)

add_executable(start start.cpp)