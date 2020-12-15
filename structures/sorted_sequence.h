#pragma once

#include <functional>

#include "array_sequence.h"

namespace sem3 {
    template<typename TElement>
    class SortedSequence {
        sem2::ArraySequence<TElement> sequence;
        std::function<bool(TElement, TElement)> less;

    public:
        explicit SortedSequence(std::function<bool(TElement, TElement)> less = std::less<TElement>()) : less(less) {}

        using Iterator = typename sem2::ArraySequence<TElement>::Iterator;

        [[nodiscard]] const sem2::ISequence<unsigned> &getRaw() const { return sequence; }

        unsigned lowerOrEqualIndex(const TElement &element) const {
            unsigned left = 0;
            unsigned right = sequence.getLength();
            unsigned middle;
            while (left + 1 < right) {
                middle = (left + right) / 2;
                if (less(element, sequence[middle])) {
                    right = middle;
                } else {
                    left = middle;
                }
            }
            return left;
        }

        std::unique_ptr<IIterator < TElement>> getIterator() const { return sequence.getIterator(); }

        [[nodiscard]] unsigned getLength() const {
            return sequence.getLength();
        }

        int indexOf(const TElement &element) const {
            if (sequence.empty()) return -1;
            unsigned index = lowerOrEqualIndex(element);
            if (element != sequence[index]) return -1;
            return static_cast<int>(index);
        }

        SortedSequence<TElement> getSubsequence(unsigned startIndex, unsigned finishIndex) {
            return sequence.getSubsequence(startIndex, finishIndex);
        }

        [[nodiscard]] bool isEmpty() const {
            return sequence.getLength() == 0;
        }

        unsigned add(TElement element) {
            if (sequence.empty()) {
                sequence.append(element);
                return 0;
            }
            unsigned index = lowerOrEqualIndex(element);
            unsigned len = sequence.getLength();
            while (index < len && element <= sequence[index]) {
                index++;
            }
            if (index == len) {
                sequence.append(element);
            } else {
                index++;
                sequence.insertAt(element, index);
            }
            return index;
        }

        void remove(unsigned index) {
            sequence.remove(index);
        }

        const TElement &get(unsigned index) const {
            return sequence.get(index);
        }

        const TElement &getFirst() const {
            return get(0);
        }

        const TElement &getLast() const {
            return get(sequence.getLength() - 1);
        }

        void merge(const SortedSequence<TElement> &other) {
            for (auto i = 0u; i < other.sequence.getLength(); i++) {
                if (indexOf(other.get(i)) == -1) add(other.get(i));
            }
        }
    };
}