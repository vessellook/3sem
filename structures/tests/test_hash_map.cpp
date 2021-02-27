#include "../../asserts.h"
#include "../hash_map.h"

using sem3::HashMap;
using tests::assert_equal;

void test1() {
    HashMap<unsigned, int> m;
    auto it = m.getIterator();
    ASSERT_EQUAL(it->hasNext(), false);
    ASSERT_EQUAL(it->next(), false);

    ASSERT_EQUAL(m.getCount(), 0u);
    m.add(5, -7);
    ASSERT_EQUAL(m.containsKey(5), true);
    ASSERT_EQUAL(m.get(5), -7);
    it = m.getIterator();
    ASSERT_EQUAL(it->hasNext(), true);
    ASSERT_EQUAL(it->next(), true);
    ASSERT_EQUAL(it->next(), false);
    ASSERT_EQUAL(it->next(), false);
    m.remove(5);
    ASSERT_EQUAL(m.containsKey(5), false);
    m.add(0, 9);
    ASSERT_EQUAL(m.containsKey(0), true);
    ASSERT_EQUAL(m.get(0), 9);
    m.add(0, 10);
    ASSERT_EQUAL(m.containsKey(0), true);
    ASSERT_EQUAL(m.get(0), 10);

    it = m.getIterator();
    ASSERT_EQUAL(it->hasNext(), true);
    ASSERT_EQUAL(it->next(), true);
    ASSERT_EQUAL(it->getCurrentItem().first, 0u);
    ASSERT_EQUAL(it->getCurrentItem().second, 10);
    ASSERT_EQUAL(it->hasNext(), false);
    ASSERT_EQUAL(it->next(), false);
};

void test2() {
    HashMap<unsigned, int> m;
    m.add(1, 2);
    m.add(2, 2);
    m.add(3, 2);
    m.add(4, 2);
    m.add(5, 2);
    m.add(6, 2);
    m.add(7, 2);
    m.add(8, 2);
    m.add(9, 2);
    ASSERT_EQUAL(m.getCount(), 9u);
};


int main() {
    test1();
    test2();

    std::cout << "FINISH";
}