#pragma once

#include "exceptions.h"

namespace sem2 {
    template<typename T>
    class LinkedList {

        template <typename K>
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
        Node *moveForward(int n, Node *node) const {
            for (int i = 0; i < n; i++) {
                node = node->next;
            }
            return node;
        }

        void tie(Node *first, Node *second) {
            first->next = second;
            second->prev = first;
        }

    public:
        class iterator {
            friend class LinkedList<T>;

            Node *node;

            iterator(Node *node) : node(node) {}

        public:

            iterator &operator++() {
                node = node->next;
                return *this;
            }

            iterator &operator--() {
                node = node->prev;
                return *this;
            }

            T &operator*() { return node->data; }

            T &operator->() { return node->data; }

            bool operator!=(iterator &other) { return node != other.node; }
        };

        class const_iterator {
            friend class LinkedList<T>;

            const Node *node;

            const_iterator(const Node *node) : node(node) {}

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

        LinkedList() : length(0), before(new Node()), after(new Node()) {
            tie(before, after);
        }

        LinkedList(const LinkedList &other) : length(other.length),
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

        LinkedList(LinkedList &&other) : length(other.length),
                                         before(other.before),
                                         after(other.after) {
            other.before = new Node();
            other.after = new Node();
            tie(other.before, other.after);
            other.length = 0;
        }

        LinkedList(const T *items, unsigned count) : length(count),
                                                     before(new Node()), after(new Node()) {
            auto *node = before;
            for (int i = 0; i < count; i++) {
                tie(node, new Node(items[i]));
                node = node->next;
            }
            tie(node, after);
        }

        ~LinkedList() {
            auto *node = before;
            auto *nextNode = before->next;
            for (unsigned i = 0; i < length; i++) {
                delete node;
                node = nextNode;
                nextNode = nextNode->next;
            }
            delete node;
            delete nextNode;
        }

        bool empty() const { return length == 0; }

        unsigned getLength() const { return length; }

        const T &get(unsigned index) const {
            if (length <= index) {
                std::string message =
                        "length = " + std::to_string(length) + "; " +
                        "index = " + std::to_string(index);
                throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
            }
            return moveForward(index + 1, before)->data;
        }

        T &get(unsigned index) {
            if (length <= index) {
                std::string message =
                        "length = " + std::to_string(length) + "; " +
                        "index = " + std::to_string(index);
                throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
            }
            return moveForward(index + 1, before)->data;
        }

        const T &getFirst() const {
            if (length == 0) {
                throw OutOfRangeError("empty list", __FILE__, __func__,
                                      __LINE__);
            }
            return before->next->data;
        }

        T &getFirst() {
            if (length == 0) {
                throw OutOfRangeError("empty list", __FILE__, __func__,
                                      __LINE__);
            }
            return before->next->data;
        }

        const T &getLast() const {
            if (length == 0) {
                throw OutOfRangeError("empty list", __FILE__, __func__,
                                      __LINE__);
            }
            return after->prev->data;
        }

        T &getLast() {
            if (length == 0) {
                throw OutOfRangeError("empty list", __FILE__, __func__,
                                      __LINE__);
            }
            return after->prev->data;
        }

        LinkedList getSublist(int beginIndex, int endIndex) const {
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

        LinkedList &set(T data, unsigned index) {
            if (length <= index) {
                std::string message =
                        "length = " + std::to_string(length) + "; " +
                        "index = " + std::to_string(index);
                throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
            }
            moveForward(index + 1, before)->data = std::move(data);
            return *this;
        }

        LinkedList &append(T data) {
            auto *newNode = new Node(std::move(data));
            tie(newNode, before->next);
            tie(before, newNode);
            length++;
            return *this;
        }

        LinkedList &prepend(T data) {
            auto *newNode = new Node(std::move(data));
            tie(after->prev, newNode);
            tie(newNode, after);
            length++;
            return *this;
        }

        LinkedList &insert(T data, unsigned index) {
            if (length < index) {
                std::string message =
                        "length = " + std::to_string(length) + "; " +
                        "index = " + std::to_string(index);
                throw OutOfRangeError(message, __FILE__, __func__, __LINE__);
            }
            auto *newNode = new Node(std::move(data));
            auto *prevNode = moveForward(index, before);
            tie(newNode, prevNode->next);
            tie(prevNode, newNode);
            length++;
            return *this;
        }

        void concat(const LinkedList &other) {
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
    };
}