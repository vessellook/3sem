#pragma once

#include <memory>

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

        [[maybe_unused]] LinkedListSequence(std::initializer_list<T> list) : list(list) {}

        explicit LinkedListSequence(LinkedList<T> &&list) : list(list) {}

        [[maybe_unused]] explicit LinkedListSequence(const ISequence<T> &other) :
                list(other.getItems(), other.getLength()) {}

        [[nodiscard]] unsigned getLength() const override { return list.getLength(); }

        const T &getFirst() const override { return list.getFirst(); }

        const T &getLast() const override { return list.getLast(); }

        LinkedListSequence *set(unsigned index, T value) override;

        const T &get(unsigned index) const override { return list.get(index); }

        T &get(unsigned index) override { return list.get(index); }

        T &getFirst() override { return list.getFirst(); }

        T &getLast() override { return list.getLast(); }

        std::unique_ptr<ISequence<T>>
        getSubsequence(unsigned beginIndex, unsigned endIndex) const override;

        LinkedListSequence *append(T item) override;

        LinkedListSequence *prepend(T item) override;

        LinkedListSequence *insertAt(T item, unsigned index) override;

        [[nodiscard]] bool empty() const override { return list.empty(); }

        T *getItems() const override;

        void concat(const ISequence<T> &other) override;

        void remove(unsigned int index) override;

        LinkedListSequence &operator=(const ISequence<T> &other) override;

        LinkedListSequence &operator=(LinkedListSequence<T> &&other) noexcept {
            list = std::move(other.list);
        }

        class Iterator : public sem3::IIterator<T> {
            typename LinkedList<T>::const_iterator it;
            bool isStart = true;
            const LinkedListSequence<T> *parent;
        public:
            explicit Iterator(const LinkedListSequence<T> *parent) : parent(parent), it(parent->list.begin()) {}

            const T &getCurrentItem() override {
                if (it != parent->list.end()) return *it;
                throw std::runtime_error("No more items in LinkedListSequence!");
            }

            [[nodiscard]] bool hasNext() const override {
                auto nextIt = it;
                ++nextIt;
                return it != parent->list.end() && (isStart || nextIt != parent->list.end());
            }

            bool next() override {
                if (isStart) {
                    isStart = false;
                    return true;
                }
                if (hasNext()) {
                    ++it;
                    return true;
                }
                return false;
            }

            sem3::Pair<bool, const T *> tryGetCurrentItem() override {
                if (it != parent->list.end()) {
                    return {true, &*it};
                }
                return {false, nullptr};
            }
        };

        std::unique_ptr<sem3::IIterator<T>> getIterator() const override {
            return std::make_unique<Iterator>(this);
        }
    };

    template<class T>
    LinkedListSequence<T> *LinkedListSequence<T>::set(unsigned int index, T value) {
        list.set(std::move(value), index);
        return this;
    }

    template<class T>
    std::unique_ptr<ISequence<T>>
    LinkedListSequence<T>::getSubsequence(unsigned int beginIndex, unsigned int endIndex) const {
        return std::make_unique<LinkedListSequence>(list.getSublist(beginIndex, endIndex));
    }

    template<class T>
    LinkedListSequence<T> *LinkedListSequence<T>::append(T item) {
        list.append(std::move(item));
        return this;
    }

    template<class T>
    LinkedListSequence<T> *LinkedListSequence<T>::prepend(T item) {
        list.prepend(std::move(item));
        return this;
    }

    template<class T>
    LinkedListSequence<T> *LinkedListSequence<T>::insertAt(T item, unsigned int index) {
        list.insert(std::move(item), index);
        return this;
    }

    template<class T>
    T *LinkedListSequence<T>::getItems() const {
        T *items = new T[list.getLength()];
        unsigned i = 0;
        for (auto it = list.begin(), endIt = list.end();
             it != endIt; ++it, ++i) {
            items[i] = *it;
        }
        return items;
    }

    template<class T>
    void LinkedListSequence<T>::concat(const ISequence<T> &other) {
        auto *items = other.getItems();
        unsigned len = other.getLength();
        for (unsigned i = 0; i < len; i++) {
            prepend(std::move(items[i]));
        }
    }

    template<class T>
    LinkedListSequence<T> &LinkedListSequence<T>::operator=(const ISequence<T> &other) {
        LinkedListSequence<T> temp(other.getItems(), other.getLength());
        list = std::move(temp.list);
        return *this;
    }

    template<class T>
    void LinkedListSequence<T>::remove(unsigned int index) {
        if (list.getLength() <= index) {
            throw OutOfRangeError("length = " + std::to_string(list.getLength()) + "; index = " +
                                  std::to_string(index), __FILE__, __func__, __LINE__);
        }
        list.remove(index);
    }

}
