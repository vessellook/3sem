#pragma once

#include "exceptions.h"
#include "sequence.h"
#include "dynamic_array.h"

namespace sem2 {

    template<class T>
    class ArraySequence : public ISequence<T> {
    protected:
        class Item;

    private:
        unsigned length = 0;
        DynamicArray<Item> array;

    protected:
        class Item : public ISequence<T>::Item {
            unsigned index;
            ArraySequence *parent;

        public:
            Item() = default;

            Item(T item, unsigned index, ArraySequence *parent) : ISequence<T>::Item(std::move(item)), index(index), parent(parent) {}

            const Item *getNext() const override {
                return &*(parent->array.begin() + (index + 1));
            }

            const Item *getPrev() const override {
                return &*(parent->array.begin() + (index - 1));
            }

            Item &operator=(const T &other) {
                this->item = other;
                return *this;
            }
        };

        using iterator = typename ISequence<T>::iterator;
        using const_iterator = typename ISequence<T>::const_iterator;

        iterator begin() override {
            return iterator(&*array.begin());
        }

        const_iterator begin() const override {
            return const_iterator(&*array.begin());
        }

        iterator end() override {
            return iterator(&*array.begin() + (array.getSize() - 1));
        }

        const_iterator end() const override {
            return const_iterator(&*array.begin() + (array.getSize() - 1));
        }

    private:
        void resize(unsigned new_size);

        T *getItems() const override;

    public:
        explicit ArraySequence(unsigned count = 0);

        ArraySequence(const ArraySequence<T> &other);

        ArraySequence(ArraySequence<T> &&other);

        ArraySequence(const T *items, unsigned count);

        explicit ArraySequence(const ISequence<T> &other);

        ~ArraySequence() override = default;

        bool empty() const override { return length == 0; }

        unsigned getLength() const override { return length; }

        const T &getFirst() const override;

        const T &getLast() const override;

        const T &get(unsigned index) const override;

        T &getFirst() override;

        T &getLast() override;

        T &get(unsigned index) override;

        ArraySequence<T> &set(unsigned index, T value) override;

        ArraySequence<T> &append(T item) override;

        ArraySequence<T> &prepend(T item) override;

        ArraySequence<T> &insertAt(T item, unsigned index) override;

        ArraySequence<T>
        *getSubsequence(unsigned beginIndex, unsigned endIndex) const override;

        void concat(const ISequence<T> &list) override;

        T &operator[](unsigned index) override { return get(index); }

        T operator[](unsigned index) const override { return get(index); }

        ArraySequence<T> &operator=(const ISequence<T> &other) override {
            auto seq = ArraySequence<T>(other);
            array = std::move(seq.array);
            length = array.getSize();
            return *this;
        }
    };

    template<class T>
    ArraySequence<T>::ArraySequence(unsigned count):
            length(count), array(count + 1) {}

    template<class T>
    ArraySequence<T>::ArraySequence(const ArraySequence<T> &other):
            length(other.length), array(other.array) {}

    template<class T>
    ArraySequence<T>::ArraySequence(ArraySequence<T> &&other):
            length(other.length), array(std::move(other.array)) {
        other.length = 0;
    }

    template<class T>
    ArraySequence<T>::ArraySequence(const T *items, unsigned count):
            length(count), array(count + 1) {
        for (unsigned i = 0; i < count; i++) {
            array[i] = Item(items[i], i, this);
        }
    }

    template<class T>
    ArraySequence<T>::ArraySequence(const ISequence<T> &other):
            length(other.getLength()), array(length + 1) {
        auto it = other.begin();
        for (unsigned i = 0; i < length; ++i, ++it) {
            array[i] = Item(*it, i, this);
        }
    }

    template<class T>
    const T &ArraySequence<T>::getFirst() const {
        if (length == 0) {
            throw OutOfRangeError("length == 0", __FILE__, __func__, __LINE__);
        }
        return array[0].getValue();
    }

    template<class T>
    T &ArraySequence<T>::getFirst() {
        if (length == 0) {
            throw OutOfRangeError("length == 0", __FILE__, __func__, __LINE__);
        }
        return array[0].getValue();
    }

    template<class T>
    const T &ArraySequence<T>::getLast() const {
        if (length == 0) {
            throw OutOfRangeError("length = 0", __FILE__, __func__, __LINE__);
        }
        return array[length - 1].getValue();
    }

    template<class T>
    T &ArraySequence<T>::getLast() {
        if (length == 0) {
            throw OutOfRangeError("length = 0", __FILE__, __func__, __LINE__);
        }
        return array[length - 1].getValue();
    }

    template<class T>
    const T &ArraySequence<T>::get(unsigned index) const {
        if (length <= index) {
            std::string message = "length = " + std::to_string(getLength())
                                  + "; index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        return array[index].getValue();
    }

    template<class T>
    T &ArraySequence<T>::get(unsigned index) {
        if (length <= index) {
            std::string message = "length = " + std::to_string(getLength())
                                  + "; index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        return array[index].getValue();
    }

    template<class T>
    ArraySequence<T> &ArraySequence<T>::set(unsigned index, T value) {
        if (length <= index) {
            std::string message = "length = " + std::to_string(getLength())
                                  + "; index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        array[index] = std::move(value);
        return *this;
    }

    template<class T>
    void ArraySequence<T>::resize(unsigned new_size) {
        if (new_size + 1 >= array.getSize()) {
            array.resize(new_size * 2 + 2);
        }
        length = new_size;
    }

    template<typename T>
    T *ArraySequence<T>::getItems() const {
        auto *items = new T[length];
        for (unsigned i = 0; i < length; i++) {
            items[i] = array[i].getValue();
        }
        return items;
    }

    template<class T>
    ArraySequence<T> &ArraySequence<T>::append(T item) {
        if (length + 1 >= array.getSize()) {
            array.resize(2 * length + 2);
        }
        auto i = array.begin() + length;
        auto j = i - 1;
        while (i != array.begin()) {
            *i-- = *j--;
        }
        length++;
        array[0] = Item(item, 0, this);
        return *this;
    }

    template<class T>
    ArraySequence<T> &ArraySequence<T>::prepend(T item) {
        if (length + 1 >= array.getSize()) {
            array.resize(2 * length + 2);
        }
        length++;
        array[length] = Item(item, length, this);
        return *this;
    }

    template<class T>
    ArraySequence<T> &ArraySequence<T>::insertAt(T item, unsigned index) {
        if (length < index) {
            throw OutOfRangeError(
                    "length = " + std::to_string(length) + "; index = " +
                    std::to_string(index), __FILE__, __func__, __LINE__);
        }
        if (length + 1 >= array.getSize()) {
            array.resize(2 * length + 2);
        }
        auto i = array.begin() + length;
        auto j = i - 1;
        while (i != array.begin() + index) {
            i->getValue() = j->getValue();
            i--;
            j--;
        }
        length++;
        array[index] = Item(item, index, this);
        return *this;
    }

    template<class T>
    ArraySequence<T> *ArraySequence<T>::getSubsequence(unsigned beginIndex,
                                                       unsigned endIndex) const {
        if (length <= endIndex || endIndex < beginIndex) {
            throw OutOfRangeError(
                    "length = " + std::to_string(length) + "; beginIndex = " +
                    std::to_string(beginIndex) + "; endIndex" +
                    std::to_string(endIndex), __FILE__, __func__, __LINE__);
        }
        auto result = new ArraySequence<T>(endIndex - beginIndex);
        for (unsigned i = beginIndex, j = 0; i < endIndex; i++, j++) {
            result->set(j, this->get(i));
        }
        return result;
    }

    template<class T>
    void ArraySequence<T>::concat(const ISequence<T> &list) {
        auto old_length = length;
        resize(length + list.getLength() + 1);
        auto new_length = length;
        auto *items = list.getItems();
        for (unsigned i = old_length, j = 0; i < new_length; i++, j++) {
            array[i] = Item(items[j], i, this);
        }
    }
}
