#pragma once

#include "../structures/array_sequence.h"
#include "sorting.h"
#include <algorithm>

namespace sem3 {
    template<typename T, typename Comparator = std::less<T>>
    class HeapSort : public ISorting<T> {
        class Heap {
            unsigned size;
            T *items;
            unsigned count = 0;
        public:
            explicit Heap(unsigned maxCount) : size(maxCount),
                                               items(new T[size + 1]) {}

            void insert(T data) {
                count++;
                unsigned index = count;
                items[index] = data;
                Comparator _less;
                while (index != 1) {
                    if (!_less(items[index], items[index / 2])) {
                        break;
                    }
                    std::swap(items[index], items[index / 2]);
                    index /= 2;
                }
            }

            T extractMin() {
                std::swap(items[1], items[count]);
                unsigned index = 1;
                unsigned left, right;
                Comparator _less;
                while (true) {
                    left = index * 2;
                    right = index * 2 + 1;
                    if (left >= count || (!_less(items[left], items[index]) &&
                                          (!_less(items[right], items[index]) ||
                                           right >= count))) {
                        break;
                    } else if (right >= count ||
                               !_less(items[right], items[left])) {
                        std::swap(items[index], items[left]);
                        index = left;
                    } else {
                        std::swap(items[index], items[right]);
                        index = right;
                    }
                }
                return items[count--];
            }
        };

    public:
        void sort(T *items, unsigned count) const override {
            auto heap = Heap(count);
            for (int i = 0; i < count; i++) {
                heap.insert(items[i]);
            }
            for (int i = 0; i < count; i++) {
                items[i] = heap.extractMin();
            }
        }

        sem2::ISequence<T> *sort(const sem2::ISequence<T> *seq) const override {
            T *items = seq->getItems();
            unsigned len = seq->getLength();
            sort(items, len);
            return new sem2::ArraySequence<T>(items, len);
        }
    };


}