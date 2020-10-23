#pragma once

#include <functional>

namespace sem3 {
    template<typename T>
    class ISorting {
    public:
        virtual void sort(T *array, unsigned count) const = 0;

        virtual sem2::ISequence<T> *sort(const sem2::ISequence<T> *seq) const = 0;
    };
}
