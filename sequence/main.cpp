#include <iostream>
#include <cstdlib>

#include "array_sequence.h"
#include "linked_list_sequence.h"

using namespace std;
using namespace art;

#include "ui/simple_ui.h"
using simple_ui::show;

int main_hidden() {

    unsigned int size = 10;


    return 0;
//    auto sequence1 = new LinkedListSequence<int>();
//    sequence1->append(3);
//    sequence1->append(1);
//    sequence1->insertAt(2, 1);
//    sequence1->prepend(4);
//    show(sequence1, "sequence1");
//
//    int *items = (int *) malloc(sizeof(int) * 5);
//    items[0] = 1;
//    items[1] = 2;
//    items[2] = 3;
//    items[3] = 4;
//    items[4] = 5;
//
//    auto sequence2 = new LinkedListSequence<int>(items, 3);
//    show(sequence2, "sequence2");
//
//    auto list = new SinglyLinkedList<int>(items, 5);
//    show(list, "list");
//    cout << list->isPalindrom();
//
//    return 0;
//    auto matrix1 = new ArrayMatrix<int>(2);
//    auto matrix2 = new ArrayMatrix<int>(2);
//
//    show(matrix1, "empty1");
//    show(matrix2, "empty2");
//
//    matrix1->set(0, 0, 1);
//    matrix1->set(0, 1, 0);
//    matrix1->set(1, 1, 1);
//    matrix1->set(1, 0, 0);
//
//    matrix2->set(0, 0, 1);
//    matrix2->set(0, 1, 5);
//    matrix2->set(1, 1, 3);
//    matrix2->set(1, 0, 7);
//
//    show(matrix1, "1");
//    *matrix1 *= 2;
//    show(matrix1, "2");
//    show(matrix2, "matrix2");
//
//    auto sum = &(*matrix1 * *matrix2);
//
//    show(sum, "sum");
//
//    return 0;
}
