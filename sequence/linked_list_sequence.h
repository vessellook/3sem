#pragma once

#include "sequence.h"
#include "linked_list.h"

namespace sem2 {

    template<class T>
    class LinkedListSequence : public ISequence<T> {
    private:
        LinkedList<T> list;
    public:
        LinkedListSequence() = default;

        LinkedListSequence(const T *items, unsigned count) : list(items,
                                                                  count) {}

        explicit LinkedListSequence(LinkedList<T> &&list) : list(list) {}

        explicit LinkedListSequence(const ISequence<T> &other) :
                list(other.getItems(), other.getLength()) {}

        unsigned getLength() const override { return list.getLength(); }

        const T &getFirst() const override { return list.getFirst(); }

        const T &getLast() const override { return list.getLast(); }

        LinkedListSequence &set(unsigned index, T value) override {
            list.set(std::move(value), index);
            return *this;
        }

        const T &get(unsigned index) const override { return list.get(index); }

        T &get(unsigned index) override { return list.get(index); }

        T &getFirst() override { return list.getFirst(); }

        T &getLast() override { return list.getLast(); }

        LinkedListSequence *
        getSubsequence(unsigned beginIndex, unsigned endIndex) const override {
            return new LinkedListSequence(list.getSublist(beginIndex, endIndex));
        }

        LinkedListSequence &append(T item) override {
            list.append(std::move(item));
            return *this;
        }

        LinkedListSequence &prepend(T item) override {
            list.prepend(std::move(item));
            return *this;
        }

        LinkedListSequence &insertAt(T item, unsigned index) override {
            list.insert(std::move(item), index);
            return *this;
        }

        bool empty() const { return list.empty(); }

        T *getItems() const {
            T *items = new T[list.getLength()];
            unsigned i = 0;
            for (auto it = list.begin(), endIt = list.end();
                 it != endIt; ++it, ++i) {
                items[i] = *it;
            }
            return items;
        }

        void concat(const ISequence<T> &other) override {
            auto *items = other.getItems();
            unsigned len = other.getLength();
            for (unsigned i = 0; i < len; i++) {
                prepend(std::move(items[i]));
            }
        }

        LinkedListSequence &operator=(const ISequence<T> &other) override {
            LinkedListSequence<T> temp(other.getItems(), other.getLength());
            list = std::move(temp.list);
        }
    };

}
