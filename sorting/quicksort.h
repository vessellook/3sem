#pragma once

#include "../sequence/array_sequence.h"
#include "sorting.h"
#include <algorithm>

namespace sem3 {
    template<typename T, typename Comparator = std::less<T>>
    class QuickSort : public ISorting<T> {
        unsigned partition(T *items, unsigned firstIndex, unsigned lastIndex) const {
            Comparator _less;
            T &pivot = items[(firstIndex + lastIndex) / 2];
            unsigned i = firstIndex;
            unsigned j = lastIndex - 1;
            while (i < j) {
                while (_less(items[i], pivot)) {
                    i++;
                }
                while (_less(pivot, items[j])) {
                    j--;
                }
                if (i < j) {
                    std::swap(items[i], items[j]);
                    i++;
                    j--;
                }
            }
            return i;
        }

        void sort(T *items, unsigned firstIndex, unsigned lastIndex) const {
            if (firstIndex + 1 < lastIndex) {
                unsigned p = partition(items, firstIndex, lastIndex);
                sort(items, firstIndex, p);
                sort(items, p, lastIndex);
            }
        }

    public:
        void sort(T *items, unsigned count) const override {
            sort(items, 0, count);
        }

        sem2::ISequence<T> *sort(const sem2::ISequence<T> *seq) const override {
            T *items = seq->getItems();
            unsigned len = seq->getLength();
            sort(items, len);
            return new sem2::ArraySequence<T>(items, len);
        }
    };
}