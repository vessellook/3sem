#pragma once

#include "exceptions.h"

namespace sem2 {
    template<class T>
    class DynamicArray {
    public:
        using iterator = T *;
        using const_iterator = const T *;
    public:
        DynamicArray(const DynamicArray &other);

        DynamicArray(DynamicArray &&other);

        DynamicArray(const T *items, unsigned count);

        DynamicArray(unsigned size = 0);

        ~DynamicArray();

        const T &get(unsigned index) const;

        T &get(unsigned index);

        unsigned getSize() const { return size; };

        DynamicArray<T> &set(unsigned index, T value);

        void resize(unsigned new_size);

        void clear();

        iterator begin() { return data; }

        const_iterator begin() const { return data; }

        iterator end() { return data + size; }

        const_iterator end() const { return data + size; }

        T &operator[](unsigned index) { return get(index); };

        const T &operator[](unsigned index) const { return get(index); };

        DynamicArray<T> &operator=(DynamicArray<T> other) {
            if (data != nullptr) { delete[] data; }
            size = other.size;
            data = other.data;
            other.data = nullptr;
            other.size = 0;
            return *this;
        }

    private:
        unsigned size;
        T *data = nullptr;
    };


    template<class T>
    DynamicArray<T>::DynamicArray(const DynamicArray &other):size(other.size),
                                                             data(nullptr) {
        if (size > 0) {
            data = new T[size];
            for (unsigned i = 0; i < size; ++i) {
                data[i] = other[i];
            }
        }
    }

    template<class T>
    DynamicArray<T>::DynamicArray(DynamicArray &&other):size(other.size),
                                                        data(other.data) {}

    template<class T>
    DynamicArray<T>::DynamicArray(const T *items, unsigned count):size(count),
                                                                  data(nullptr) {
        if (count > 0) {
            data = new T[count];
            for (unsigned i = 0; i < count; ++i) {
                data[i] = items[i];
            }
        }
    }

    template<class T>
    DynamicArray<T>::DynamicArray(unsigned size): size(size),
                                                  data(nullptr) {
        if (size > 0) {
            data = new T[size];
            for (unsigned i = 0; i < size; ++i) {
                data[i] = T();
            }
        }
    }

    template<class T>
    DynamicArray<T>::~DynamicArray() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
    }

    template<class T>
    const T &DynamicArray<T>::get(unsigned index) const {
        if (size <= index) {
            std::string message = "size = " + std::to_string(size) + "; " +
                                  "index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        return data[index];
    }

    template<class T>
    T &DynamicArray<T>::get(unsigned index) {
        if (size <= index || index < 0) {
            std::string message = "size = " + std::to_string(size)
                                  + "; index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        return data[index];
    }


    template<class T>
    DynamicArray<T> &DynamicArray<T>::set(unsigned index, T value) {
        if (size <= index) {
            std::string message = "size = " + std::to_string(size)
                                  + "; index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        data[index] = value;
        return *this;
    }

    template<class T>
    void DynamicArray<T>::resize(unsigned new_size) {
        if (size == new_size) return;
        if (new_size == 0) {
            clear();
            return;
        }
        if (size == 0) {
            data = new T[new_size];
            size = new_size;
            return;
        }
        auto new_data = new T[new_size];
        for (unsigned i = 0; i < size; i++) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        size = new_size;
    }

    template<typename T>
    void DynamicArray<T>::clear() {
        size = 0;
        if (data != nullptr) {
            delete[] data;
        }
    }

    template<class T>
    DynamicArray<T>
    where(const DynamicArray<T> &array, bool(*check)(const T &)) {
        int new_size = 0;
        for (auto &item : array) {
            if (check(item)) {
                new_size++;
            }
        }
        auto new_array = DynamicArray<T>(new_size);
        auto it = begin(new_array);
        for (auto &item : array) {
            if (check(item)) {
                *it++ = item;
            }
        }
        return new_array;
    }
}
