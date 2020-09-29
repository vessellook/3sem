#include <iostream>

#include "../../asserts.h"

#include "../linked_list.h"

#define TestLinkedList LinkedList


using namespace std;
using namespace art;
using tests::assert_equal;

int main() {
    auto list1 = new TestLinkedList<int>();

    assert_equal(list1->getLength(), (unsigned) 0);

    list1->append(3);
    list1->append(2);
    list1->append(1);

    assert_equal(list1->getLength(), (unsigned) 3);

    assert_equal(list1->getFirst(), 1);
    assert_equal(list1->getLast(), 3);

    assert_equal(list1->get(0), 1, __FILE__, __LINE__);
    assert_equal(list1->get(1), 2);
    assert_equal(list1->get(2), 3);

    list1->prepend(5);
    list1->prepend(6);
    list1->insert(4, 3);

    assert_equal(list1->getLength(), (unsigned) 6);

    assert_equal(list1->get(0), 1, __FILE__, __LINE__);
    assert_equal(list1->get(1), 2);
    assert_equal(list1->get(2), 3, __FILE__, __LINE__);
    assert_equal(list1->get(3), 4);
    assert_equal(list1->get(4), 5);
    assert_equal(list1->get(5), 6);

    int *items = new int[5];
    items[0] = 1;
    items[1] = 2;
    items[2] = 4;
    items[3] = 8;
    items[4] = 16;

    auto list2 = new TestLinkedList<int>(items, 5);

    assert_equal(list2->getLength(), (unsigned) 5);

    assert_equal(list2->get(0), 1, __FILE__, __LINE__);
    assert_equal(list2->get(1), 2);
    assert_equal(list2->get(2), 4);
    assert_equal(list2->get(3), 8);
    assert_equal(list2->get(4), 16);

    auto list3 = new TestLinkedList<int>(*list2);
    delete list2;

    assert_equal(list3->getLength(), (unsigned) 5);

    assert_equal(list3->get(0), 1, __FILE__, __LINE__);
    assert_equal(list3->get(1), 2);
    assert_equal(list3->get(2), 4);
    assert_equal(list3->get(3), 8);
    assert_equal(list3->get(4), 16);


    list3->get(0) = 7;
    list3->get(1) = 8;

    list3->set(9, 2);
    list3->set(11, 3);
    list3->set(10, 3);
    list3->set(11, 4);

    assert_equal(list3->getLength(), (unsigned) 5);

    assert_equal(list3->get(0), 7, __FILE__, __LINE__);
    assert_equal(list3->get(1), 8);
    assert_equal(list3->get(2), 9);
    assert_equal(list3->get(3), 10);
    assert_equal(list3->get(4), 11);

    auto *list4 = new TestLinkedList<int>(*list1);
    list4->concat(*list3);

    assert_equal(list4->getLength(), (unsigned) 11);

    assert_equal(list4->get(0), 1, __FILE__, __LINE__);
    assert_equal(list4->get(1), 2);
    assert_equal(list4->get(2), 3);
    assert_equal(list4->get(3), 4);
    assert_equal(list4->get(4), 5);
    assert_equal(list4->get(5), 6);
    assert_equal(list4->get(6), 7, __FILE__, __LINE__);
    assert_equal(list4->get(7), 8);
    assert_equal(list4->get(8), 9);
    assert_equal(list4->get(9), 10);
    assert_equal(list4->get(10), 11);

    delete list1;
    delete list3;
    delete list4;

    cout << "FINISH";

    return 0;
}