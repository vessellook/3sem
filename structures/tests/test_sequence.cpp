#include <iostream>
#include <memory>

#include "../../asserts.h"

using tests::assert_equal;


#include "../array_sequence.h"
#include "../linked_list_sequence.h"

using namespace std;
using namespace sem2;

#define TestSequence ArraySequence
//#define TestSequence LinkedListSequence


int main() {
    ISequence<int> *sequence1 = new TestSequence<int>();

    ASSERT_EQUAL(sequence1->getLength(), 0u);

    sequence1->prepend(3);
    sequence1->prepend(2);
    sequence1->prepend(1);

    ASSERT_EQUAL(sequence1->getLength(), 3u);

    ASSERT_EQUAL(sequence1->getFirst(), 1);
    ASSERT_EQUAL(sequence1->getLast(), 3);

    ASSERT_EQUAL(sequence1->get(0), 1);
    ASSERT_EQUAL(sequence1->get(1), 2);
    ASSERT_EQUAL(sequence1->get(2), 3);

    sequence1->append(5);
    sequence1->append(6);
    sequence1->insertAt(4, 3);

    ASSERT_EQUAL(sequence1->getLength(), 6u);

    ASSERT_EQUAL(sequence1->get(0), 1);
    ASSERT_EQUAL(sequence1->get(1), 2);
    ASSERT_EQUAL(sequence1->get(2), 3);
    ASSERT_EQUAL(sequence1->get(3), 4);
    ASSERT_EQUAL(sequence1->get(4), 5);
    ASSERT_EQUAL(sequence1->get(5), 6);

    int *items = new int[5];
    items[0] = 1;
    items[1] = 2;
    items[2] = 4;
    items[3] = 8;
    items[4] = 16;

    ISequence<int> *sequence2 = new TestSequence<int>(items, 5);

    ASSERT_EQUAL(sequence2->getLength(), 5u);

    ASSERT_EQUAL(sequence2->get(0), 1);
    ASSERT_EQUAL(sequence2->get(1), 2);
    ASSERT_EQUAL(sequence2->get(2), 4);
    ASSERT_EQUAL(sequence2->get(3), 8);
    ASSERT_EQUAL(sequence2->get(4), 16);

    ISequence<int> *sequence3 = new TestSequence<int>(*sequence2);
    delete sequence2;

    ASSERT_EQUAL(sequence3->getLength(), 5u);

    ASSERT_EQUAL(sequence3->get(0), 1);
    ASSERT_EQUAL(sequence3->get(1), 2);
    ASSERT_EQUAL(sequence3->get(2), 4);
    ASSERT_EQUAL(sequence3->get(3), 8);
    ASSERT_EQUAL(sequence3->get(4), 16);

    (*sequence3)[0] = 7;
    (*sequence3)[1] = 8;
    (*sequence3)[2] = 9;
    (*sequence3)[3] = 11;
    (*sequence3)[3] = 10;
    (*sequence3)[4] = 11;

    ASSERT_EQUAL(sequence3->getLength(), 5u);

    ASSERT_EQUAL(sequence3->get(0), 7);
    ASSERT_EQUAL(sequence3->get(1), 8);
    ASSERT_EQUAL(sequence3->get(2), 9);
    ASSERT_EQUAL(sequence3->get(3), 10);
    ASSERT_EQUAL(sequence3->get(4), 11);

    ISequence<int> *sequence4 = new TestSequence<int>(*sequence1);

    sequence4->concat(*sequence3);
    delete sequence1;

    ASSERT_EQUAL(sequence4->getLength(), 11u);

    ASSERT_EQUAL(sequence4->get(0), 1);
    ASSERT_EQUAL(sequence4->get(1), 2);
    ASSERT_EQUAL(sequence4->get(2), 3);
    ASSERT_EQUAL(sequence4->get(3), 4);
    ASSERT_EQUAL(sequence4->get(4), 5);
    ASSERT_EQUAL(sequence4->get(5), 6);
    ASSERT_EQUAL(sequence4->get(6), 7);
    ASSERT_EQUAL(sequence4->get(7), 8);
    ASSERT_EQUAL(sequence4->get(8), 9);
    ASSERT_EQUAL(sequence4->get(9), 10);
    ASSERT_EQUAL(sequence4->get(10), 11);

    auto i = 1;
    for(auto it = sequence4->getIterator(); it->next();) {
        ASSERT_EQUAL(it->getCurrentItem(), i++);
    }

    delete sequence3;
    delete sequence4;

    ISequence<int> *sequence5 = new TestSequence<int>();
    sequence5->append(2);

    TestSequence<std::shared_ptr<TestSequence<int>>> sequence7;
    cout << "FINISH";
    return 0;
}