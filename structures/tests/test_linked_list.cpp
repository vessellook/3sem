#include <iostream>

#include "../../asserts.h"

#include "../linked_list.h"

#define TestLinkedList LinkedList


using namespace std;
using namespace sem2;
using tests::assert_equal;

int main() {
    auto list1 = new TestLinkedList<int>();

    ASSERT_EQUAL(list1->getLength(), (unsigned) 0);

    list1->append(3);
    list1->append(2);
    list1->append(1);

    ASSERT_EQUAL(list1->getLength(), (unsigned) 3);

    ASSERT_EQUAL(list1->getFirst(), 1);
    ASSERT_EQUAL(list1->getLast(), 3);

    ASSERT_EQUAL(list1->get(0), 1);
    ASSERT_EQUAL(list1->get(1), 2);
    ASSERT_EQUAL(list1->get(2), 3);

    list1->prepend(5);
    list1->prepend(6);
    list1->insert(4, 3);

    ASSERT_EQUAL(list1->getLength(), (unsigned) 6);

    ASSERT_EQUAL(list1->get(0), 1);
    ASSERT_EQUAL(list1->get(1), 2);
    ASSERT_EQUAL(list1->get(2), 3);
    ASSERT_EQUAL(list1->get(3), 4);
    ASSERT_EQUAL(list1->get(4), 5);
    ASSERT_EQUAL(list1->get(5), 6);

    int *items = new int[5];
    items[0] = 1;
    items[1] = 2;
    items[2] = 4;
    items[3] = 8;
    items[4] = 16;

    auto list2 = new TestLinkedList<int>(items, 5);

    ASSERT_EQUAL(list2->getLength(), (unsigned) 5);

    ASSERT_EQUAL(list2->get(0), 1);
    ASSERT_EQUAL(list2->get(1), 2);
    ASSERT_EQUAL(list2->get(2), 4);
    ASSERT_EQUAL(list2->get(3), 8);
    ASSERT_EQUAL(list2->get(4), 16);

    auto list3 = new TestLinkedList<int>(*list2);
    delete list2;

    ASSERT_EQUAL(list3->getLength(), (unsigned) 5);

    ASSERT_EQUAL(list3->get(0), 1);
    ASSERT_EQUAL(list3->get(1), 2);
    ASSERT_EQUAL(list3->get(2), 4);
    ASSERT_EQUAL(list3->get(3), 8);
    ASSERT_EQUAL(list3->get(4), 16);


    list3->get(0) = 7;
    list3->get(1) = 8;

    list3->set(9, 2);
    list3->set(11, 3);
    list3->set(10, 3);
    list3->set(11, 4);

    ASSERT_EQUAL(list3->getLength(), (unsigned) 5);

    ASSERT_EQUAL(list3->get(0), 7);
    ASSERT_EQUAL(list3->get(1), 8);
    ASSERT_EQUAL(list3->get(2), 9);
    ASSERT_EQUAL(list3->get(3), 10);
    ASSERT_EQUAL(list3->get(4), 11);

    auto *list4 = new TestLinkedList<int>(*list1);
    list4->concat(*list3);

    ASSERT_EQUAL(list4->getLength(), (unsigned) 11);

    ASSERT_EQUAL(list4->get(0), 1);
    ASSERT_EQUAL(list4->get(1), 2);
    ASSERT_EQUAL(list4->get(2), 3);
    ASSERT_EQUAL(list4->get(3), 4);
    ASSERT_EQUAL(list4->get(4), 5);
    ASSERT_EQUAL(list4->get(5), 6);
    ASSERT_EQUAL(list4->get(6), 7);
    ASSERT_EQUAL(list4->get(7), 8);
    ASSERT_EQUAL(list4->get(8), 9);
    ASSERT_EQUAL(list4->get(9), 10);
    ASSERT_EQUAL(list4->get(10), 11);

    delete list1;
    delete list3;
    delete list4;

    cout << "FINISH";

    return 0;
}