#include <iostream>
#include <memory>

#include "../../asserts.h"

using tests::assert_equal;


#include "../array_sequence.h"
//#include "../linked_list_sequence.h"

using namespace std;
using namespace sem2;

#define TestSequence ArraySequence
//#define TestSequence LinkedListSequence


int main() {
    ISequence<int> *sequence1 = new TestSequence<int>();

    assert_equal<unsigned>(sequence1->getLength(), 0);

    sequence1->append(3);
    sequence1->append(2);
    sequence1->append(1);

    assert_equal<unsigned>(sequence1->getLength(), 3, __FILE__, __LINE__);

    assert_equal(sequence1->getFirst(), 1);
    assert_equal(sequence1->getLast(), 3, __FILE__, __LINE__);

    assert_equal(sequence1->get(0), 1);
    assert_equal(sequence1->get(1), 2);
    assert_equal(sequence1->get(2), 3, __FILE__, __LINE__);

    sequence1->prepend(5);
    sequence1->prepend(6);
    sequence1->insertAt(4, 3);

    assert_equal<unsigned>(sequence1->getLength(), 6);

    assert_equal(sequence1->get(0), 1);
    assert_equal(sequence1->get(1), 2);
    assert_equal(sequence1->get(2), 3);
    assert_equal(sequence1->get(3), 4);
    assert_equal(sequence1->get(4), 5, __FILE__, __LINE__);
    assert_equal(sequence1->get(5), 6);

    int *items = new int[5];
    items[0] = 1;
    items[1] = 2;
    items[2] = 4;
    items[3] = 8;
    items[4] = 16;

    ISequence<int> *sequence2 = new TestSequence<int>(items, 5);

    assert_equal<unsigned>(sequence2->getLength(), 5, __FILE__, __LINE__);

    assert_equal(sequence2->get(0), 1);
    assert_equal(sequence2->get(1), 2);
    assert_equal(sequence2->get(2), 4);
    assert_equal(sequence2->get(3), 8);
    assert_equal(sequence2->get(4), 16);

    ISequence<int> *sequence3 = new TestSequence<int>(*sequence2);
    delete sequence2;

    assert_equal<unsigned>(sequence3->getLength(), 5, __FILE__, __LINE__);

    assert_equal(sequence3->get(0), 1, __FILE__, __LINE__);
    assert_equal(sequence3->get(1), 2);
    assert_equal(sequence3->get(2), 4);
    assert_equal(sequence3->get(3), 8);
    assert_equal(sequence3->get(4), 16, __FILE__, __LINE__);

    (*sequence3)[0] = 7;
    (*sequence3)[1] = 8;
    (*sequence3)[2] = 9;
    (*sequence3)[3] = 11;
    (*sequence3)[3] = 10;
    (*sequence3)[4] = 11;

    assert_equal<unsigned>(sequence3->getLength(), 5, __FILE__, __LINE__);

    assert_equal(sequence3->get(0), 7);
    assert_equal(sequence3->get(1), 8);
    assert_equal(sequence3->get(2), 9);
    assert_equal(sequence3->get(3), 10);
    assert_equal(sequence3->get(4), 11);

    ISequence<int> *sequence4 = new TestSequence<int>(*sequence1);

    sequence4->concat(*sequence3);
    delete sequence1;

    assert_equal<unsigned>(sequence4->getLength(), 11, __FILE__, __LINE__);

    assert_equal(sequence4->get(0), 1);
    assert_equal(sequence4->get(1), 2);
    assert_equal(sequence4->get(2), 3);
    assert_equal(sequence4->get(3), 4);
    assert_equal(sequence4->get(4), 5, __FILE__, __LINE__);
    assert_equal(sequence4->get(5), 6);
    assert_equal(sequence4->get(6), 7);
    assert_equal(sequence4->get(7), 8);
    assert_equal(sequence4->get(8), 9);
    assert_equal(sequence4->get(9), 10);
    assert_equal(sequence4->get(10), 11, __FILE__, __LINE__);


    delete sequence3;
    delete sequence4;

    ISequence<int> *sequence5 = new TestSequence<int>();
    sequence5->append(2);

    TestSequence<std::shared_ptr<TestSequence<int>>> sequence7;
    cout << "FINISH";
    return 0;
}