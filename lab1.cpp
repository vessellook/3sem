#include <iostream>
#include <sstream>
#include <memory>
#include "structures/linked_list_sequence.h"
#include "structures/array_sequence.h"
#include "sorting/heapsort.h"
#include "sorting/quicksort.h"
#include "sorting/mergesort.h"
#include "benchmark.h"
#include "structures/sparse_matrix.h"

using sem2::LinkedList;
using sem2::DynamicArray;
using sem2::LinkedListSequence;

enum class SortingType {
    QUICKSORT,
    HEAPSORT,
    MERGESORT
};

template<typename T>
sem3::ISorting<T> *getSorting(SortingType type) {
    switch (type) {
        case SortingType::QUICKSORT:
            return new sem3::QuickSort<T>();
        case SortingType::HEAPSORT:
            return new sem3::HeapSort<T>();
        case SortingType::MERGESORT:
            return new sem3::MergeSort<T>();
        default:
            throw std::logic_error("Unsupportable SortingType");
    }
}

SortingType getType(const std::string &str) {
    if (str.empty() || str == "quick") {
        return SortingType::QUICKSORT;
    } else if (str == "heap") {
        return SortingType::HEAPSORT;
    } else if (str == "merge") {
        return SortingType::MERGESORT;
    } else {
        throw std::invalid_argument("strange sorting type");
    }
}

struct Request {
    SortingType type;
    double *array;
    unsigned count;
};

std::istream &operator>>(std::istream &is, Request &request) {
    std::string typeString;
    is >> typeString;
    SortingType type = getType(typeString);
    LinkedListSequence<double> seq;
    for (double item; is >> item;) {
        seq.prepend(item);
    }
    request.type = type;
    request.array = seq.getItems();
    request.count = seq.getLength();
    return is;
}

std::ostream &operator<<(std::ostream &os, const Request &request) {
    for (unsigned i = 0; i < request.count; i++) {
        os << request.array[i] << ' ';
    }
    return os;
}

int main() {
    sem2::ArraySequence<std::unique_ptr<int>> a;

    while (true) {
        std::cout << ">> ";
        std::string input;
        getline(std::cin, input);
        if (input == "exit") break;
        if (input.empty()) continue;
        try {
            std::istringstream is(input);
            Request request{};
            is >> request;
            std::cout << "your array: " << request << std::endl;

            getSorting<double>(request.type)->sort(request.array, request.count);
//            std::cout << "sorting time: "<< benchmark([&request]() {
//                getSorting<double>(request.type)->sort(request.array, request.count);
//            }) << " ms\n";
            std::cout << "sorted array: " << request << std::endl;
        } catch (std::invalid_argument &unused) {
            std::cout << "program: invalid input. Use the following schema\n"
                         "[sort type] [array values separated by space (' ')]\n"
                         "Example: quick 1 4 3 2\n\n"
                         "Sorting types:\n"
                         "  quick\n    uses algorithm QuickSort\n"
                         "  heap\n     uses algorithm HeapSort\n"
                         "  merge\n    uses algorithm MergeSort" << std::endl;

        }
    }
}