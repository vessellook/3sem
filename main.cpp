#include <iostream>
#include "sequence/linked_list_sequence.h"
#include "sequence/array_sequence.h"

using sem2::LinkedList;
using sem2::DynamicArray;
using sem2::LinkedListSequence;

//template<typename T>
//void swap(T &a, T &b) {
//    auto t = std::move(a);
//    a = std::move(b);
//    b = std::move(t);
//}
//
//namespace sem2 {
//    template<typename T>
//    void reverse(LinkedList<T> &list) {
//        auto *node = list.before;
//        while (node != list.after) {
//            swap(node->next, node->prev);
//            node = node->prev;
//        }
//        swap(node->next, node->prev);
//        swap(list.before, list.after);
//    }
//
//}
//
//template<typename T>
//void reverse(DynamicArray<T> &array) {
//    auto len = array.getSize();
//    for (int i = 0; i * 2 < array.getSize(); i++) {
//        swap(array[i], array[len - i - 1]);
//    }
//}
//
//template<typename T>
//void print(DynamicArray<T> &array) {
//    for (auto &item: array) {
//        std::cout << item << " ";
//    }
//    std::cout << std::endl;
//}
//
//template<typename T>
//void print(LinkedList<T> &list) {
//    for (auto &item: list) {
//        std::cout << item << " ";
//    }
//    std::cout << std::endl;
//}
//
//int main() {
//    DynamicArray<int> array;
//    array.resize(5);
//    array
//            .set(0, 1)
//            .set(1, 2)
//            .set(2, 4)
//            .set(3, 8)
//            .set(4, 16);
//    print(array);
//    reverse(array);
//    print(array);
//    LinkedList<int> list;
//    list
//            .prepend(1)
//            .prepend(2)
//            .prepend(4)
//            .prepend(8)
//            .prepend(16);
//    print(list);
//    reverse(list);
//    print(list);
//    return 0;
//}

template<typename T>
struct Node {
    LinkedListSequence<Node *> children;
    T value;
};

template<typename T>
T goThroughTree(Node<T> *root, T(*f)(T, Node<T>)) {
    for (auto &node : root->children) {
        goThroughTree(node, f);
    }
    return f(*root);
}