#include "../../asserts.h"
#include "../hash_map.h"

using sem3::HashMap;
using tests::assert_equal;

int main() {
    auto m = HashMap<unsigned, int>();
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

    std::cout << "FINISH";
}