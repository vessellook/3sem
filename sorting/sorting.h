#pragma once

namespace sem3 {
    template<typename T>
    class Sorting {
    public:
        virtual void sort(T *array, unsigned count) const = 0;

        virtual sem2::ISequence<T> *sort(sem2::ISequence<T> &seq) const = 0;
    };
}
