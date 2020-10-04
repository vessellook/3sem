#pragma once

#include "../sequence/array_sequence.h"
#include "sorting.h"
#include <algorithm>

namespace sem3 {
    template<typename T>
    class MergeSort : public Sorting<T> {
        void merge(T *input, unsigned begin, unsigned middle, unsigned end, T *output) const {
            unsigned outputIt = begin;
            for (unsigned left = begin, right = middle; left != middle || right != end;) {
                if (right == end || left != middle && input[left] < input[right]) {
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

        void copy(T *input, unsigned begin, unsigned end, T *output) const {
            for (auto i = begin; i < end; i++) {
                output[i] = input[i];
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

        sem2::ISequence<T> *sort(sem2::ISequence<T> &sequence) const override {
            T *items = sequence.getItems();
            sort(items, sequence.getLength());
            return new sem2::ArraySequence<T>(items, sequence.getLength());
        }
    };
}