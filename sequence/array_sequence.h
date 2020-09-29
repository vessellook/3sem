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
            T item;
            unsigned index;
            ArraySequence *parent;

        public:
            static Item *convertArray(const T *array, unsigned count) {
                auto *result = new Item[count];
                for (unsigned i = 0; i < count; i++) {
                    result[i] = array[i];
                }
                return result;
            }

        public:
            // TODO: проверка на граничные значения
            Item *getNext() override {
                unsigned i = index + 1;
                if (i < parent->array.getSize()) {
                    return &(parent->array[i]);
                } else {
                    return nullptr;
                }
            }

            // TODO: проверка на граничные значения
            const Item *getNext() const override {
                unsigned i = index + 1;
                if (i < parent->array.getSize()) {
                    return &(parent->array[i]);
                } else {
                    return nullptr;
                }
            }

            // TODO: проверка на граничные значения
            Item *getPrev() override {
                unsigned i = index - 1;
                if (i < parent->array.getSize()) {
                    return &(parent->array[i]);
                } else {
                    return nullptr;
                }
            }

            // TODO: проверка на граничные значения
            const Item *getPrev() const override {
                unsigned i = index - 1;
                if (i < parent->array.getSize()) {
                    return &(parent->array[i]);
                } else {
                    return nullptr;
                }
            }

            operator T &() override {
                return item;
            }

            operator const T &() const override {
                return item;
            }

            Item operator=(const T &other) {
                item = other;
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
            return iterator(nullptr);
        }

        const_iterator end() const override {
            return const_iterator(nullptr);
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
            length(count), array(count) {}

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
            length(count), array(Item::convertArray(items, count), count) {}

    template<class T>
    ArraySequence<T>::ArraySequence(const ISequence<T> &other):
            length(other.getLength()), array(length) {
        auto *items = other.getItems();
        for (unsigned i = 0; i < length; i++) {
            array[i] = items[i];
        }
    }

    template<class T>
    const T &ArraySequence<T>::getFirst() const {
        if (length == 0) {
            throw OutOfRangeError("length == 0", __FILE__, __func__, __LINE__);
        }
        return array[0];
    }

    template<class T>
    T &ArraySequence<T>::getFirst() {
        if (length == 0) {
            throw OutOfRangeError("length == 0", __FILE__, __func__, __LINE__);
        }
        return array[0];
    }

    template<class T>
    const T &ArraySequence<T>::getLast() const {
        if (length == 0) {
            throw OutOfRangeError("length = 0", __FILE__,
                                  __func__, __LINE__);
        }
        return array[length - 1];
    }

    template<class T>
    T &ArraySequence<T>::getLast() {
        if (length == 0) {
            throw OutOfRangeError("length = 0", __FILE__,
                                  __func__, __LINE__);
        }
        return array[length - 1];
    }

    template<class T>
    const T &ArraySequence<T>::get(unsigned index) const {
        if (length <= index) {
            std::string message = "length = " + std::to_string(getLength())
                                  + "; index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        return array[index];
    }

    template<class T>
    T &ArraySequence<T>::get(unsigned index) {
        if (length <= index) {
            std::string message = "length = " + std::to_string(getLength())
                                  + "; index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        return array[index];
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
        if (new_size >= array.getSize()) {
            array.resize(new_size * 2);
        }
        length = new_size;
    }

    template<typename T>
    T *ArraySequence<T>::getItems() const {
        auto *items = new T[length];
        for (unsigned i = 0; i < length; i++) {
            items[i] = array[i];
        }
        return items;
    }

    template<class T>
    ArraySequence<T> &ArraySequence<T>::append(T item) {
        if (length >= array.getSize()) {
            array.resize(2 * length + 1);
        }
        auto i = array.begin() + length;
        auto j = i - 1;
        while (i != array.begin()) {
            *i-- = *j--;
        }
        length++;
        array[0] = item;
        return *this;
    }

    template<class T>
    ArraySequence<T> &ArraySequence<T>::prepend(T item) {
        if (length >= array.getSize()) {
            array.resize(2 * length);
        }
        array[length++] = item;
        return *this;
    }

    template<class T>
    ArraySequence<T> &ArraySequence<T>::insertAt(T item, unsigned index) {
        if (length < index) {
            throw OutOfRangeError(
                    "length = " + std::to_string(length) + "; index = " +
                    std::to_string(index), __FILE__, __func__, __LINE__);
        }
        if (length >= array.getSize()) {
            array.resize(2 * length);
        }
        auto i = array.begin() + length, j = i - 1;
        while (i != array.begin() + index) {
            *i-- = *j--;
        }
        length++;
        array[index] = item;
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
        auto *items = new T[endIndex - beginIndex];
        for (unsigned i = beginIndex, j = 0; i < endIndex; i++, j++) {
            items[j] = array[i];
        }
        return new ArraySequence<T>(items, endIndex - beginIndex);
    }

    template<class T>
    void ArraySequence<T>::concat(const ISequence<T> &list) {
        auto old_length = length;
        resize(length + list.getLength());
        auto new_length = length;
        auto *items = list.getItems();
        for (unsigned i = old_length, j = 0; i < new_length; i++, j++) {
            array[i] = items[j];
        }
    }
}
