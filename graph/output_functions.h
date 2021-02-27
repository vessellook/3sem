#pragma once

#include <iostream>

#include "../structures/sequence.h"
#include "../structures/hash_map.h"
#include "../structures/hash_set.h"
#include "../structures/sorted_sequence.h"
#include "../structures/sparse_matrix.h"

namespace sem2 {
    template<typename T>
    std::ostream &operator<<(std::ostream &os, const ISequence<T> &seq) {
        auto it = seq.getIterator();
        os << "[";
        while (it->next()) {
            os << it->getCurrentItem() << ", ";
        }
        os << "]";
        return os;
    }

}
namespace sem3 {
    template<typename T>
    std::ostream &operator<<(std::ostream &os, const SortedSequence<T> &seq) {
        auto it = seq.getIterator();
        os << "[";
        while (it->next()) {
            os << it->getCurrentItem() << ", ";
        }
        os << "]";
        return os;
    }

    template<typename K, typename V>
    std::ostream &operator<<(std::ostream &os, const HashMap<K, V> &hashMap) {
        auto it = hashMap.getIterator();
        os << "{";
        while (it->next()) {
            os << it->getCurrentItem() << ", ";
        }
        os << "}";
        return os;
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &os, const HashSet<T> &hashSet) {
        auto it = hashSet.getIterator();
        os << "{";
        while (it->next()) {
            os << it->getCurrentItem() << ", ";
        }
        os << "}";
        return os;
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &os, const SparseMatrix<T> &matrix) {
        auto rows = matrix.getNotEmptyRows();
        auto rowIt = rows.getIterator();
        os << "________\n";
        while (rowIt->next()) {
            auto colIt = matrix.getColsByRow(rowIt->getCurrentItem()).getIterator();
            os << "|" << rowIt->getCurrentItem() << ": ";
            while (colIt->next()) {
                os << "(" << colIt->getCurrentItem() << ", " << matrix.get(colIt->getCurrentItem(),
                                                                           rowIt->getCurrentItem())
                   << ")";
            }
            os << "|\n";
        }
        os << "-------\n    ";
        return os;
    }

    template<typename F, typename S>
    std::ostream &operator<<(std::ostream &os, const Pair<F, S> &pair) {
        os << "(" << pair.first << ", " << pair.second << ")";
        return os;
    }
}