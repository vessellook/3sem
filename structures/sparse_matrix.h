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
        sem2::ArraySequence<sem2::ArraySequence<T>> values;
        sem2::ArraySequence<SortedSequence<unsigned>> cols;
        HashMap<unsigned, unsigned> rowToIndex;
        T defaultValue = T();

        void add(unsigned col, unsigned row, T value) {
            set(col, row, value + get(col, row));
        }

        T getValue(const SparseMatrix<T> &other, unsigned rowIndex, unsigned otherRowIndex) const {
            T value = defaultValue;
            auto colIndex1 = 0u;
            auto colIndex2 = 0u;
            while (colIndex1 < cols[rowIndex].getLength() &&
                   colIndex2 < other.cols[otherRowIndex].getLength()) {
                if (unsigned col1 = cols[rowIndex].get(colIndex1),
                            col2 = other.cols[otherRowIndex].get(colIndex2);
                        col1 == col2) {
                    value += values[rowIndex].get(colIndex1) * other.values[otherRowIndex].get(colIndex2);
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

        explicit SparseMatrix(unsigned m = 0, unsigned n = 0) : m(m), n(n), rowToIndex() {}

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
            if (!rowToIndex.containsKey(row)) {
                auto rowIndex = cols.getLength();
                rowToIndex.add(row, rowIndex);
                cols.append(SortedSequence<unsigned>());
                values.append(sem2::ArraySequence<T>());
                unsigned colIndex = cols[rowIndex].add(col);
                values[rowIndex].insertAt(value, colIndex);
            } else {
                auto rowIndex = rowToIndex.get(row);
                if (auto colIndex = cols.get(rowIndex).indexOf(col); colIndex == -1) {
                    colIndex = cols[rowIndex].add(col);
                    values[rowIndex].insertAt(value, colIndex);
                } else {
                    values[rowIndex].set(colIndex, value);
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
            if (!rowToIndex.containsKey(row)) return defaultValue;
            auto rowIndex = rowToIndex.get(row);
            if (int index = cols[rowIndex].indexOf(col); index == -1) {
                return defaultValue;
            } else {
                return values[rowIndex].get(index);
            }
        }

        [[nodiscard]] const SortedSequence<unsigned> &getColsByRow(unsigned row) const {
            return cols[rowToIndex.get(row)];
        }

        [[nodiscard]] SortedSequence<unsigned> getNotEmptyRows() const {
            auto result = SortedSequence<unsigned>();
            auto iterator = rowToIndex.getIterator();
            while (iterator->next()) {
                result.add(iterator->getCurrentItem().first);
            }
            return result;
        }

        SparseMatrix<T> transpose() const {
            auto result = SparseMatrix<T>(n, m);
            auto iterator = rowToIndex.getIterator();
            while (iterator->next()) {
                auto row = iterator->getCurrentItem().first;
                auto rowIndex = iterator->getCurrentItem().second;
                for (unsigned colIndex = 0; colIndex < cols[rowIndex].getLength(); colIndex++) {
                    auto col = cols[rowIndex].get(colIndex);
                    auto value = values[rowIndex].get(colIndex);
                    result.set(row, col, value);
                }
            }
            return result;
        }

        SparseMatrix operator*(const T &factor) const {
            auto result = SparseMatrix(*this);
            if (factor == defaultValue) return result;
            auto iterator = rowToIndex.getIterator();
            while (iterator->next()) {
                auto rowIndex = iterator->getCurrentItem().second;
                for (unsigned colIndex = 0; colIndex < result.values[rowIndex].getLength(); colIndex++) {
                    result.values[rowIndex][colIndex] *= factor;
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
            auto it = rowToIndex.getIterator();
            while (it->next()) {
                auto index = it->getCurrentItem().second;
                auto row = it->getCurrentItem().first;
                auto transposedIt = transposed.rowToIndex.getIterator();
                while (transposedIt->next()) {
                    auto col = transposedIt->getCurrentItem().first;
                    auto transposedIndex = transposedIt->getCurrentItem().second;
                    result.set(col, row, getValue(transposed,
                                                  index,
                                                  transposedIndex));
                }
            }
            return result;
        }

        SparseMatrix operator+(const SparseMatrix &other) const {
            if (m != other.m && n != other.n) {
                throw std::runtime_error("different sizes of matrices");
            }
            auto result = SparseMatrix<T>(other);
            auto iterator = rowToIndex.getIterator();
            while (iterator->next()) {
                auto row = iterator->getCurrentItem().first;
                auto rowIndex = iterator->getCurrentItem().second;
                for (auto colIndex = 0u; colIndex < cols[rowIndex].getLength(); colIndex++) {
                    auto col = cols[rowIndex].get(colIndex);
                    result.add(col, row, values[rowIndex][colIndex]);
                }
            }
            return result;
        }
    };
}