#pragma once

namespace sem2 {

    template<class T>
    class ISequence {
    public:
        class iterator;
        class const_iterator;
    protected:
        class Item {
            friend class ISequence<T>::iterator;
            friend class ISequence<T>::const_iterator;
        protected:
            T item;
        public:
            Item() = default;
            Item(T item): item(item) {}

            T &getValue() { return item; }

            const T &getValue() const { return item; }

            virtual const Item *getNext() const = 0;

            virtual const Item *getPrev() const = 0;
        };

    public:
        class iterator {
            Item *item;
        public:
            iterator(Item *item) : item(item) {}

            T &operator*() {
                return item->item;
            }

            T &operator->() {
                return item->item;
            }

            iterator &operator++() {
                item = item->getNext();
            }

            iterator &operator--() {
                item = item->getPrev();
            }
        };

        class const_iterator {
            const Item *item;
        public:
            const_iterator(const Item *item) : item(item) {}

            const T &operator*() const {
                return item->item;
            }

            const T &operator->() const {
                return item->item;
            }

            const_iterator &operator++() {
                item = item->getNext();
            }

            const_iterator &operator--() {
                item = item->getPrev();
            }
        };

        virtual iterator begin() = 0;

        virtual const_iterator begin() const = 0;

        virtual iterator end() = 0;

        virtual const_iterator end() const = 0;

    public:
        virtual ~ISequence() = default;

        virtual bool empty() const = 0;

        virtual unsigned getLength() const = 0;

        virtual const T &getFirst() const = 0;

        virtual const T &getLast() const = 0;

        virtual T *getItems() const = 0;

        virtual const T &get(unsigned index) const = 0;

        virtual T &getFirst() = 0;

        virtual T &getLast() = 0;

        virtual T &get(unsigned index) = 0;

        virtual void concat(const ISequence<T> &list) = 0;

        virtual ISequence<T>
        *getSubsequence(unsigned beginIndex, unsigned endIndex) const = 0;

        virtual ISequence<T> &set(unsigned index, T value) = 0;

        virtual ISequence<T> &append(T item) = 0;

        virtual ISequence<T> &prepend(T item) = 0;

        virtual ISequence<T> &insertAt(T item, unsigned index) = 0;

        virtual ISequence<T> &operator=(const ISequence<T> &other) = 0;

        virtual T &operator[](unsigned index) { return get(index); };

        virtual T operator[](unsigned index) const { return get(index); }
    };

// extra

    template<class T>
    bool
    operator==(const ISequence<T> &sequence1, const ISequence<T> &sequence2) {
        if (sequence1.getLength() != sequence2.getLength()) {
            return false;
        }
        for (unsigned i = 0; i < sequence1.getLength(); ++i) {
            if (sequence1.get(i) != sequence2.get(i)) {
                return false;
            }
        }
        return true;
    }

    template<class F, class T>
    T reduce(const ISequence<F> &sequence, F (*func)(T, F), T initial) {
        T result = initial;
        for (unsigned index = 1; index < sequence.getLength(); index++) {
            result = func(sequence->getData(), result);
        }
        return result;
    }

    template<class F, class T>
    T
    reduce(const ISequence<F> &sequence, F (*func)(T, F, unsigned), T initial) {
        T result = initial;
        for (unsigned index = 1; index < sequence.getLength(); index++) {
            result = func(sequence->getData(), result, index);
        }
        return result;
    }

}
