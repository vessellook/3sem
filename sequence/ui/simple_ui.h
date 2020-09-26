#pragma once

#include <iostream>
#include <memory>

#include "../dynamic_array.h"
#include "../linked_list.h"
#include "../sequence.h"
#include "../matrix/matrix.h"

namespace simple_ui {

    template<class T>
    void show(const art::ISequence<T> *sequence, const std::string &name = "") {
        unsigned length = sequence->getLength();
        if (name.empty()) {
            std::cout << "sequence (length " << length << "): ";
        } else {
            std::cout << "sequence \"" << name << "\" (length " << length << "): ";
        }

        for (unsigned i = 0; i < length; ++i) {
            std::cout << sequence->get(i) << " ";
        }
        std::cout << std::endl << std::endl;
    }

    template<class T>
    void show(const art::LinkedList<T> *list, const std::string &name = "") {
        unsigned length = list->getLength();
        if (name.empty()) {
            std::cout << "list (length " << length << "): ";
        } else {
            std::cout << "list \"" << name << "\" (length " << length << "): ";
        }

        for (unsigned i = 0; i < length; ++i) {
            std::cout << list->get(i) << " ";
        }
        std::cout << std::endl << std::endl;
    }

    template<class T>
    void show(const art::DynamicArray<T> *array, const std::string &name = "") {
        unsigned length = array->getSize();
        if (name.empty()) {
            std::cout << "array (length " << length << "): ";
        } else {
            std::cout << "array \"" << name << "\" (length " << length << "): ";
        }

        for (unsigned i = 0; i < length; ++i) {
            std::cout << array->get(i) << " ";
        }
        std::cout << std::endl << std::endl;
    }

    template<class T>
    void show(const art::IMatrix<T> *matrix, const std::string &name = "") {
        unsigned size = matrix->getSize();
        if (name.empty()) {
            std::cout << "matrix " << size << "x" << size << std::endl;
        } else {
            std::cout << "matrix \"" << name << "\" " << size << "x" << size << std::endl;
        }

        for (unsigned i = 0; i < size; ++i) {
            std::cout << "|";
            for (unsigned j = 0; j < size; ++j) {
                std::cout << " " << (*matrix)[j][i];
            }
            std::cout << "   |" << std::endl;
        }
        std::cout << std::endl;
    }

    template<class T>
    void show(std::shared_ptr<art::IMatrix<T>> matrix, const std::string &name = "") {
        unsigned size = matrix->getSize();
        if (name.empty()) {
            std::cout << "matrix " << size << "x" << size << std::endl;
        } else {
            std::cout << "matrix \"" << name << "\" " << size << "x" << size << std::endl;
        }

        for (unsigned i = 0; i < size; ++i) {
            std::cout << "|";
            for (unsigned j = 0; j < size; ++j) {
                std::cout << " " << (*matrix)[j][i];
            }
            std::cout << "   |" << std::endl;
        }
        std::cout << std::endl;
    }
}
