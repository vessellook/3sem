#pragma once

#include "exceptions.h"
#include "sequence.h"
#include "dynamic_array.h"

namespace sem2 {

    template<class T>
    class ArraySequence : public ISequence<T> {
    private:
        unsigned length = 0;
        DynamicArray<T> array;

    private:
        void resize(unsigned new_size);

        T *getItems() const override;

    public:
        explicit ArraySequence(unsigned count = 0);

        ArraySequence(const ArraySequence<T> &other);

        ArraySequence(ArraySequence<T> &&other) noexcept ;

        ArraySequence(const T *items, unsigned count);

        ArraySequence(std::initializer_list<T> list);

        explicit ArraySequence(const ISequence<T> &other);

        ~ArraySequence() override = default;

        [[nodiscard]] bool empty() const override { return length == 0; }

        [[nodiscard]] unsigned getLength() const override { return length; }

        const T &getFirst() const override;

        const T &getLast() const override;

        const T &get(unsigned index) const override;

        T &getFirst() override;

        T &getLast() override;

        T &get(unsigned index) override;

        ArraySequence<T> *set(unsigned index, T value) override;

        ArraySequence<T> *append(T item) override;

        ArraySequence<T> *prepend(T item) override;

        ArraySequence<T> *insertAt(T item, unsigned index) override;

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
    ArraySequence<T>::ArraySequence(ArraySequence<T> &&other) noexcept:
            length(other.length), array(std::move(other.array)) {
        other.length = 0;
    }

    template<class T>
    ArraySequence<T>::ArraySequence(const T *items, unsigned count):
            length(count), array(count + 1) {
        for (unsigned i = 0; i < count; i++) {
            array[i] = items[i];
        }
    }

    template<class T>
    ArraySequence<T>::ArraySequence(const ISequence<T> &other):
            length(other.getLength()), array(other.getItems(), length) {}

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
            throw OutOfRangeError("length = 0", __FILE__, __func__, __LINE__);
        }
        return array[length - 1];
    }

    template<class T>
    T &ArraySequence<T>::getLast() {
        if (length == 0) {
            throw OutOfRangeError("length = 0", __FILE__, __func__, __LINE__);
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
    ArraySequence<T> *ArraySequence<T>::set(unsigned index, T value) {
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
            array.resize(new_size * 2 + 1);
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
    ArraySequence<T> *ArraySequence<T>::append(T item) {
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
    ArraySequence<T> *ArraySequence<T>::prepend(T item) {
        if (length >= array.getSize()) {
            array.resize(2 * length + 1);
        }
        array[length] = item;
        length++;
        return *this;
    }

    template<class T>
    ArraySequence<T> *ArraySequence<T>::insertAt(T item, unsigned index) {
        if (length < index) {
            throw OutOfRangeError(
                    "length = " + std::to_string(length) + "; index = " +
                    std::to_string(index), __FILE__, __func__, __LINE__);
        }
        if (length >= array.getSize()) {
            array.resize(2 * length + 1);
        }
        auto i = array.begin() + length;
        auto j = i - 1;
        while (i != array.begin() + index) {
            i = j;
            i--;
            j--;
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
        auto result = new ArraySequence<T>(endIndex - beginIndex);
        for (unsigned i = beginIndex, j = 0; i < endIndex; i++, j++) {
            result->set(j, this->get(i));
        }
        return result;
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

    template<class T>
    ArraySequence<T>::ArraySequence(std::initializer_list<T> list): length(list.end() - list.begin()),
                                                                    array(length) {
        unsigned i = 0;
        for (T item : list) {
            array[i] = std::move(item);
            i++;
        }
    }
}
