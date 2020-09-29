#pragma once

#include "sequence.h"
#include "linked_list.h"

namespace sem2 {

    template<class T>
    class LinkedListSequence : public ISequence<T> {
    protected:
        class Item;
    public:
        using iterator = typename ISequence<T>::iterator;
        using const_iterator = typename ISequence<T>::const_iterator;
    private:
        LinkedList<Item> list;
    protected:

        class Item : public ISequence<T>::Item {
            typename LinkedList<Item>::iterator it;

        public:
            static Item *convertArray(const T *array, unsigned count) {
                auto *result = new Item[count];
                for (unsigned i = 0; i < count; i++) {
                    result[i] = Item(array[i], i);
                }
                return result;
            }

        public:
            Item(T item, typename LinkedList<T>::iterator it) : ISequence<T>::Item(std::move(item)), it(it) {}

            const Item *getNext() const override {
                auto nextIt = it;
                nextIt++;
                return *nextIt;
            }

            const Item *getPrev() const override {
                auto prevIt = it;
                prevIt--;
                return *prevIt;
            }

            explicit operator T &() override {
                return this->item;
            }

            explicit operator const T &() const override {
                return this->item;
            }

            Item &operator=(const T &other) {
                this->item = other;
                return *this;
            }
        };

        iterator begin() override {
            return iterator(&*list.begin());
        }

        const_iterator begin() const override {
            return const_iterator(&*list.begin());
        }

        iterator end() override {
            return iterator(&*list.begin() + (list.getSize() - 1));
        }

        const_iterator end() const override {
            return const_iterator(&*list.begin() + (list.getSize() - 1));
        }

    public:
        LinkedListSequence() = default;

        LinkedListSequence(const T *items, unsigned count) : list(items,
                                                                  count) {}

        explicit LinkedListSequence(LinkedList<T> &&list) : list(list) {}

        explicit LinkedListSequence(const ISequence<T> &other) :
                list(other.getItems(), other.getLength()) {
            auto it = list.begin();
            auto finish = list.end();
            for(; it != finish; it++) {
                *it = Item(*it, it, this);
            }
        }

        unsigned getLength() const override { return list.getLength(); }

        const T &getFirst() const override { return list.getFirst(); }

        const T &getLast() const override { return list.getLast(); }

        LinkedListSequence<T> &set(unsigned index, const T &value) override {
            list.set(value, index);
            return *this;
        }

        LinkedListSequence<T> &set(unsigned index, T value) override {
            list.set(std::move(value), index);
            return *this;
        }

        const T &get(unsigned index) const override { return list.get(index); }

        T &get(unsigned index) override { return list.get(index); }

        T &getFirst() override { return list.getFirst(); }

        T &getLast() override { return list.getLast(); }

        LinkedListSequence<T> *
        getSubsequence(unsigned beginIndex, unsigned endIndex) const override {
            return new LinkedListSequence(
                    list.getSublist(beginIndex, endIndex));
        }

        LinkedListSequence<T> &append(T item) override {
            auto it = list.append(std::move(item));
            *it = Item(*it, it, this);
            return *this;
        }

        LinkedListSequence<T> &prepend(T item) override {
            auto it = list.prepend(std::move(item));
            *it = Item(*it, it, this);
            return *this;
        }

        LinkedListSequence<T> &insertAt(T item, unsigned index) override {
            auto it = list.insert(std::move(item), index);
            *it = Item(*it, it, this);
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
    };

}
