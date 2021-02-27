#pragma once

#include <functional>
#include <memory>
#include <sstream>
#include<stdexcept>
#include <iostream>

#include "sorted_sequence.h"
#include "hash_map.h"
#include "pair.h"

namespace sem3 {

    // https://intuit.ru/studies/courses/4447/983/lecture/14931?page=5
    template<typename T>
    class SparseMatrix {
        unsigned m;
        unsigned n;
        HashMap<unsigned, sem2::ArraySequence<T>> rowToValues;
        HashMap<unsigned, SortedSequence<unsigned>> rowToCols;
        T defaultValue = T();

        void add(unsigned col, unsigned row, T value) {
            set(col, row, value + get(col, row));
        }

        T getValue(const SparseMatrix<T> &other, unsigned row, unsigned otherRow) const {
            T value = defaultValue;
            auto colIndex1 = 0u;
            auto colIndex2 = 0u;
            while (colIndex1 < rowToCols[row].getLength() &&
                   colIndex2 < other.rowToCols[otherRow].getLength()) {
                if (unsigned col1 = rowToCols[row].get(colIndex1),
                            col2 = other.rowToCols[otherRow].get(colIndex2);
                        col1 == col2) {
                    value += rowToValues[row].get(colIndex1) * other.rowToValues[otherRow].get(colIndex2);
                    colIndex1++;
                    colIndex2++;
                } else if (col1 < col2) {
                    colIndex1++;
                } else {
                    colIndex2++;
                }
            }
            return value;
        }

    public:

        explicit SparseMatrix(unsigned m = 0, unsigned n = 0) : m(m), n(n) {}

        SparseMatrix(const SparseMatrix &) = default;

        SparseMatrix(SparseMatrix &&) noexcept = default;

        SparseMatrix &operator=(const SparseMatrix &other) = default;

        SparseMatrix &operator=(SparseMatrix &&other) noexcept = default;

        void setDefault(T defaultValue) {
            this->defaultValue = defaultValue;
        }

        Pair<unsigned, unsigned> GetSize() {
            return {m, n};
        }

        void set(unsigned col, unsigned row, T value) {
            if (m <= col || n <= row) {
                std::ostringstream oss;
                oss << "Index out of bound: col = " << col << ", row = " << row << "; m = " << m << ", n = " << n
                    << "; file " << __FILE__ << ", line " << __LINE__;
                throw std::runtime_error(oss.str());
            }
            if (!rowToCols.containsKey(row)) {
                rowToCols.add(row, SortedSequence<unsigned>());
                rowToValues.add(row, sem2::ArraySequence<T>());
                unsigned colIndex = rowToCols[row].add(col);
                rowToValues[row].insertAt(value, colIndex);
            } else {
                if (auto colIndex = rowToCols.get(row).indexOf(col); colIndex == -1) {
                    colIndex = rowToCols[row].add(col);
                    rowToValues[row].insertAt(value, colIndex);
                } else {
                    rowToValues[row][colIndex] = value;
                }
            }
        }

        const T &get(unsigned col, unsigned row) const {
            if (m <= col || n <= row) {
                std::ostringstream oss;
                oss << "Index out of bound: col = " << col << ", row = " << row << ", m = " << m << ", n = " << n
                    << ", file " << __FILE__ << ":" << __LINE__;
                throw std::runtime_error(oss.str());
            }
            if (!rowToCols.containsKey(row)) return defaultValue;
            if (int index = rowToCols[row].indexOf(col); index == -1) {
                return defaultValue;
            } else {
                return rowToValues[row][index];
            }
        }

        [[nodiscard]] const SortedSequence<unsigned> &getColsByRow(unsigned row) const {
            return rowToCols[row];
        }

        [[nodiscard]] SortedSequence<unsigned> getNotEmptyRows() const {
            auto result = SortedSequence<unsigned>();
            auto iterator = rowToCols.getIterator();
            while (iterator->next()) {
                if (result.indexOf(iterator->getCurrentItem().first) == -1) {
                    result.add(iterator->getCurrentItem().first);
                }
            }
            return result;
        }

        SparseMatrix<T> transpose() const {
            auto result = SparseMatrix<T>(n, m);
            auto iterator = rowToCols.getIterator();
            while (iterator->next()) {
                auto row = iterator->getCurrentItem().first;
                auto &cols = iterator->getCurrentItem().second;
                for (unsigned colIndex = 0; colIndex < cols.getLength(); colIndex++) {
                    auto col = cols.get(colIndex);
                    auto &value = rowToValues[row].get(colIndex);
                    result.set(row, col, value);
                }
            }
            return result;
        }

        SparseMatrix operator*(const T &factor) const {
            auto result = SparseMatrix(*this);
            if (factor == defaultValue) return result;
            auto iterator = rowToCols.getIterator();
            while (iterator->next()) {
                auto row = iterator->getCurrentItem().first;
                for (unsigned index = 0; index < result.rowToValues[row].getLength(); index++) {
                    result.rowToValues[row][index] *= factor;
                }
            }
            return result;
        }

        SparseMatrix<T> operator*(const SparseMatrix<T> &other) const {
            if (n != other.m) {
                throw std::runtime_error("matrices don't match for multiplication");
            }
            auto transposed = other.transpose();
            auto result = SparseMatrix<T>(m, other.n);
            auto it = rowToCols.getIterator();
            while (it->next()) {
                auto resultRow = it->getCurrentItem().first;
                auto &cols = it->getCurrentItem().second;
                auto transposedIt = transposed.rowToCols.getIterator();
                while (transposedIt->next()) {
                    auto resultCol = transposedIt->getCurrentItem().first;
                    auto &transposedCols = transposedIt->getCurrentItem().second;
                    result.set(resultCol, resultRow, getValue(transposed, resultRow, resultCol));
                }
            }
            return result;
        }

        SparseMatrix operator+(const SparseMatrix &other) const {
            if (m != other.m && n != other.n) {
                throw std::runtime_error("different sizes of matrices");
            }
            auto result = SparseMatrix<T>(other);
            auto iterator = rowToCols.getIterator();
            while (iterator->next()) {
                auto row = iterator->getCurrentItem().first;
                auto &cols = iterator->getCurrentItem().second;
                for (auto colIndex = 0u; colIndex < cols.getLength(); colIndex++) {
                    auto col = cols.get(colIndex);
                    result.add(col, row, rowToValues[row][colIndex]);
                }
            }
            return result;
        }
    };
}