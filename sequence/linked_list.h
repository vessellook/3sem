#pragma once

#include "exceptions.h"

namespace sem2 {
    template<typename T>
    class LinkedList {

        template<typename K>
        friend void reverse(LinkedList<K> &);

        struct Node {
            T data;
            Node *prev;
            Node *next;

            Node() : data(), prev(nullptr), next(nullptr) {}

            Node(T data) : data(data), prev(nullptr), next(nullptr) {}
        };

        unsigned length;
        Node *before;
        Node *after;

    private:
        // unsafe function
        Node *moveForward(int n, Node *node) const;

        void tie(Node *first, Node *second);

        void tie(Node *first, Node *second, Node *third);

    public:
        class iterator {
            friend class LinkedList<T>;

            Node *node;

            explicit iterator(Node *node) : node(node) {}

        public:
            iterator &operator++();

            iterator &operator--();

            T &operator*() { return node->data; }

            T &operator->() { return node->data; }

            bool operator!=(iterator &other) { return node != other.node; }
        };

        class const_iterator {
            friend class LinkedList<T>;

            const Node *node;

            explicit const_iterator(const Node *node) : node(node) {}

        public:

            const_iterator &operator++() {
                node = node->next;
                return *this;
            }

            const_iterator &operator--() {
                node = node->prev;
                return *this;
            }

            const T &operator*() { return node->data; }

            const T &operator->() { return node->data; }

            bool operator!=(const_iterator &other) { return node != other.node; }
        };

        iterator begin() { return iterator(before->next); }

        const_iterator begin() const { return const_iterator(before->next); }

        iterator end() { return iterator(after); }

        const_iterator end() const { return const_iterator(after); }

    public:

        LinkedList() : length(0), before(new Node()), after(new Node()) { tie(before, after); }

        LinkedList(const LinkedList &other);

        LinkedList(LinkedList &&other) noexcept;

        LinkedList(const T *items, unsigned count);

        LinkedList(std::initializer_list<T> list);

        ~LinkedList();

        void clear();

        [[nodiscard]] bool empty() const { return length == 0; }

        [[nodiscard]] unsigned getLength() const { return length; }

        const T &get(unsigned index) const;

        T &get(unsigned index);

        const T &getFirst() const;

        T &getFirst();

        const T &getLast() const;

        T &getLast();

        LinkedList getSublist(int beginIndex, int endIndex) const;

        LinkedList &set(T data, unsigned index);

        LinkedList &append(T data);

        LinkedList &prepend(T data);

        LinkedList &insert(T data, unsigned index);

        void concat(const LinkedList &other);

        LinkedList &operator=(const LinkedList &other);

        LinkedList &operator=(LinkedList &&other) noexcept ;
    };

    template<typename T>
    typename LinkedList<T>::iterator &LinkedList<T>::iterator::operator++() {
        node = node->next;
        return *this;
    }

    template<typename T>
    typename LinkedList<T>::iterator &LinkedList<T>::iterator::operator--() {
        node = node->prev;
        return *this;
    }

    template<typename T>
    typename LinkedList<T>::Node *LinkedList<T>::moveForward(int n, LinkedList::Node *node) const {
        for (int i = 0; i < n; i++) {
            node = node->next;
        }
        return node;
    }

    template<typename T>
    void LinkedList<T>::tie(LinkedList::Node *first, LinkedList::Node *second) {
        first->next = second;
        second->prev = first;
    }

    template<typename T>
    void LinkedList<T>::tie(LinkedList::Node *first, LinkedList::Node *second, LinkedList::Node *third) {
        tie(first, second);
        tie(second, third);
    }

    template<typename T>
    LinkedList<T>::LinkedList(const LinkedList &other) : length(other.length),
                                                         before(new Node()) {
        auto *otherNode = other.before;
        auto otherAfter = other.after;
        auto *node = before;
        while (otherNode != otherAfter) {
            otherNode = otherNode->next;
            tie(node, new Node(otherNode->data));
            node = node->next;
        }
        after = node;
    }

    template<typename T>
    LinkedList<T>::LinkedList(LinkedList &&other) noexcept : length(other.length),
                                                             before(other.before),
                                                             after(other.after) {
        other.before = new Node();
        other.after = new Node();
        tie(other.before, other.after);
        other.length = 0;
    }

    template<typename T>
    LinkedList<T>::LinkedList(const T *items, unsigned int count) : length(count),
                                                                    before(new Node()), after(new Node()) {
        auto *node = before;
        for (int i = 0; i < count; i++) {
            tie(node, new Node(items[i]));
            node = node->next;
        }
        tie(node, after);
    }

    template<typename T>
    LinkedList<T>::LinkedList(std::initializer_list<T> list) : length(list.end() - list.begin()),
                                                               before(new Node()), after(new Node()) {
        auto *node = before;
        for (T item : list) {
            tie(node, new Node(std::move(item)));
            node = node->next;
        }
        tie(node, after);
    }

    template<typename T>
    LinkedList<T>::~LinkedList() {
        clear();
        delete before;
        delete after;
    }

    template<typename T>
    void LinkedList<T>::clear() {
        auto *node = before;
        auto *nextNode = node->next;
        while (nextNode != after) {
            node = nextNode;
            nextNode = nextNode->next;
            delete node;
        }
        tie(before, after);
    }

    template<typename T>
    const T &LinkedList<T>::get(unsigned int index) const {
        if (length <= index) {
            std::string message =
                    "length = " + std::to_string(length) + "; " +
                    "index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        return moveForward(index + 1, before)->data;
    }

    template<typename T>
    T &LinkedList<T>::get(unsigned int index) {
        return const_cast<T &>(static_cast<const LinkedList<T> *>(this)->get(index));
    }

    template<typename T>
    const T &LinkedList<T>::getFirst() const {
        if (length == 0) {
            throw OutOfRangeError("empty list", __FILE__, __func__,
                                  __LINE__);
        }
        return before->next->data;
    }

    template<typename T>
    T &LinkedList<T>::getFirst() {
        return const_cast<T &>(static_cast<const LinkedList<T> *>(this)->getFirst());
    }

    template<typename T>
    const T &LinkedList<T>::getLast() const {
        if (length == 0) {
            throw OutOfRangeError("empty list", __FILE__, __func__,
                                  __LINE__);
        }
        return after->prev->data;
    }

    template<typename T>
    T &LinkedList<T>::getLast() {
        return const_cast<T &>(static_cast<const LinkedList<T> *>(this)->getLast());
    }

    template<typename T>
    LinkedList<T> LinkedList<T>::getSublist(int beginIndex, int endIndex) const {
        if (length <= endIndex || endIndex < beginIndex) {
            std::string message =
                    "length = " + std::to_string(length) +
                    "; beginIndex = " +
                    std::to_string(beginIndex) + "; endIndex = " +
                    std::to_string(endIndex);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        auto *node = moveForward(beginIndex, before);
        auto sublist = LinkedList();
        for (int i = beginIndex; i < endIndex; i++) {
            sublist.prepend(node->data);
            node = node->next;
        }
        return sublist;
    }

    template<typename T>
    LinkedList<T> &LinkedList<T>::set(T data, unsigned int index) {
        if (length <= index) {
            std::string message =
                    "length = " + std::to_string(length) + "; " +
                    "index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        moveForward(index + 1, before)->data = std::move(data);
        return *this;
    }

    template<typename T>
    LinkedList<T> &LinkedList<T>::append(T data) {
        auto *newNode = new Node(std::move(data));
        tie(before, newNode, before->next);
        length++;
        return *this;
    }

    template<typename T>
    LinkedList<T> &LinkedList<T>::prepend(T data) {
        auto *newNode = new Node(std::move(data));
        tie(after->prev, newNode, after);
        length++;
        return *this;
    }

    template<typename T>
    LinkedList<T> &LinkedList<T>::insert(T data, unsigned int index) {
        if (length < index) {
            std::string message =
                    "length = " + std::to_string(length) + "; " +
                    "index = " + std::to_string(index);
            throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
        }
        auto *newNode = new Node(std::move(data));
        auto *prevNode = moveForward(index, before);
        tie(prevNode, newNode, prevNode->next);
        length++;
        return *this;
    }

    template<typename T>
    void LinkedList<T>::concat(const LinkedList &other) {
        auto *otherNode = other.before;
        auto otherTail = other.after;
        auto *node = after->prev;
        while (otherNode != otherTail) {
            otherNode = otherNode->next;
            tie(node, new Node(otherNode->data));
            node = node->next;
        }
        length += other.length;
    }

    template<typename T>
    LinkedList<T> &LinkedList<T>::operator=(const LinkedList &other) {
        LinkedList temp(other);
        tie(before, temp.before->next);
        tie(temp.after->prev, after);
        tie(temp.before, temp.after);
        length = temp.length;
    }

    template<typename T>
    LinkedList<T> &LinkedList<T>::operator=(LinkedList &&other) noexcept {
        LinkedList temp(other);
        tie(before, temp.before->next);
        tie(temp.after->prev, after);
        tie(temp.before, temp.after);
        length = temp.length;
        return *this;
    }
}