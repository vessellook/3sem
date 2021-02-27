#include <iostream>

# include "../sorted_sequence.h"
#include "../../asserts.h"

using sem3::SortedSequence;
using tests::assert_equal;

void test1() {
    auto s = SortedSequence<unsigned>();
    ASSERT_EQUAL(s.getLength(), 0u);
    ASSERT_EQUAL(s.add(1), 0u);
    ASSERT_EQUAL(s.get(0), 1u);
    ASSERT_EQUAL(s.indexOf(1), 0);
    ASSERT_EQUAL(s.getFirst(), 1u);
    ASSERT_EQUAL(s.getLast(), 1u);
    ASSERT_EQUAL(s.isEmpty(), false);
    ASSERT_EQUAL(s.add(2), 1u);
    ASSERT_EQUAL(s.get(1), 2u);
    ASSERT_EQUAL(s.indexOf(2), 1);
    ASSERT_EQUAL(s.getFirst(), 1u);
    ASSERT_EQUAL(s.getLast(), 2u);
    ASSERT_EQUAL(s.isEmpty(), false);
    ASSERT_EQUAL(s.indexOf(7), -1);

}


void test2() {
    auto s = SortedSequence<unsigned>();
    ASSERT_EQUAL(s.getLength(), 0u);
    ASSERT_EQUAL(s.add(5), 0u);
    ASSERT_EQUAL(s.get(0), 5u);
    ASSERT_EQUAL(s.indexOf(5), 0);
    ASSERT_EQUAL(s.getFirst(), 5u);
    ASSERT_EQUAL(s.getLast(), 5u);
    ASSERT_EQUAL(s.isEmpty(), false);
    ASSERT_EQUAL(s.add(9), 1u);
    ASSERT_EQUAL(s.get(1), 9u);
    ASSERT_EQUAL(s.indexOf(2), 1);
    ASSERT_EQUAL(s.getFirst(), 1u);
    ASSERT_EQUAL(s.getLast(), 2u);
    ASSERT_EQUAL(s.isEmpty(), false);
    ASSERT_EQUAL(s.indexOf(7), -1);

}

int main () {

    std::cout << "FINISH";
}