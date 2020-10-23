#pragma once

#include "../sequence/array_sequence.h"
#include "sorting.h"
#include <algorithm>

namespace sem3 {
    template<typename T, typename Comparator = std::less<T>>
    class MergeSort : public ISorting<T> {
        void merge(T *input, unsigned begin, unsigned middle, unsigned end, T *output) const {
            unsigned outputIt = begin;
            Comparator _less;
            for (unsigned left = begin, right = middle; left != middle || right != end;) {
                if (right == end || left != middle && _less(input[left], input[right])) {
                    output[outputIt] = input[left];
                    outputIt++;
                    left++;
                } else {
                    output[outputIt] = input[right];
                    outputIt++;
                    right++;
                }
            }
        }

        void copy(T *from, unsigned begin, unsigned end, T *to) const {
            for (auto i = begin; i < end; i++) {
                to[i] = from[i];
            }
        }

        void sort(T *input, unsigned begin, unsigned end, T *output) const {
            if (end - begin == 1) return;
            unsigned middle = (begin + end) / 2;
            sort(input, begin, middle, output);
            sort(input, middle, end, output);
            merge(input, begin, middle, end, output);
            copy(output, begin, end, input);// there is no mistake in order of arguments
        }

    public:
        void sort(T *array, unsigned count) const override {
            T *helpArray = new T[count];
            sort(array, 0, count, helpArray);
        }

        sem2::ISequence<T> *sort(const sem2::ISequence<T> *seq) const override {
            T *items = seq->getItems();
            unsigned len = seq->getLength();
            sort(items, len);
            return new sem2::ArraySequence<T>(items, len);
        }
    };
}