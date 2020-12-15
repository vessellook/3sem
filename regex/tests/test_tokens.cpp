#include <climits>
#include "../token.h"
#include "../../asserts.h"

using namespace sem3;
using tests::assert_equal;


void assert_sequence(Token &token, unsigned min, unsigned max, const std::string &file, int line) {
    for (unsigned j = max - min + 1, i = max; j > 0; i--, j--) {
        try {
            assert_equal(token.hasNextPrefix(), true, file, line);
        } catch (tests::AssertError &error) {
            throw tests::AssertError(error._message + " (has) for i = " + std::to_string(i));
        }
        try {
            assert_equal(token.getNextPrefix(), i, file, line);
        } catch (tests::AssertError &error) {
            throw tests::AssertError(error._message + " (get) for i = " + std::to_string(i));
        }
    }
    assert_equal(token.hasNextPrefix(), false, file, line);
}

#define ASSERT_SEQUENCE(token, min_len, max_len) assert_sequence(token, min_len, max_len, __FILE__, __LINE__)
#define ASSERT_ONE_PREFIX(token, len) assert_sequence(token, len, len, __FILE__, __LINE__)
#define ASSERT_ONE(token, sample, len) token.reset(sample); ASSERT_ONE_PREFIX(token, len)
#define ASSERT_SEQ(token, sample, min_len, max_len) token.reset(sample); ASSERT_SEQUENCE(token, min_len, max_len)
#define ASSERT_NONE(token, sample) token.reset(sample); ASSERT_EQUAL(token.hasNextPrefix(), false)

void test_anything() {
    auto token = Tokens::Anything();
    // retries: 1 - 1
    ASSERT_ONE(token, "a", 1u);
    ASSERT_NONE(token, "");
    ASSERT_ONE(token, "abc", 1u);
    // retries: 0 - max
    token.setEntries(0, UINT_MAX);
    ASSERT_SEQ(token, "haha", 0u, 4u);
}

void test_char() {
    auto token = Tokens::Char('c');
    // retries: 1 - 1
    ASSERT_ONE(token, "c", 1u);
    ASSERT_NONE(token, "");
    ASSERT_ONE(token, "ccc", 1u);
    ASSERT_NONE(token, "abc");
    // retries: 0 - max
    token.setEntries(0, UINT_MAX);
    ASSERT_SEQ(token, "cccc", 0u, 4u);
    ASSERT_ONE(token, "abc", 0u);
    ASSERT_SEQ(token, "caha", 0u, 1u);
}

void test_set_1() {
    auto token = Tokens::Set("abc");
    // retries: 1 - 1
    ASSERT_ONE(token, "a", 1u);
    ASSERT_NONE(token, "");
    ASSERT_ONE(token, "abc", 1u);
    // retries: 0 - max
    token.setEntries(0, UINT_MAX);
    ASSERT_SEQ(token, "abcd",0u,  3u);

    auto token2 = Tokens::Set("^abc");
    ASSERT_NONE(token2, "a");
    ASSERT_NONE(token2, "c");
    ASSERT_ONE(token2, "A", 1u);
    ASSERT_ONE(token2, "^", 1u);
}

void test_set_2() {
    auto token = Tokens::Set("a-z");
    // retries: 1 - 1
    ASSERT_ONE(token, "a", 1u);
    ASSERT_NONE(token, "");
    ASSERT_NONE(token, "_");
    ASSERT_NONE(token, "-");
    ASSERT_ONE(token, "cd", 1u);
    // retries: 0 - max
    token.setEntries(0, UINT_MAX);
    ASSERT_SEQ(token, "haha", 0u, 4u);

    auto token2 = Tokens::Set("a-");
    // retries: 1 - 1
    ASSERT_ONE(token2, "a",1u);
    ASSERT_ONE(token2, "-",1u);
    ASSERT_NONE(token2, "c");

    auto token3 = Tokens::Set("-9");
    // retries: 1 - 1
    ASSERT_ONE(token3, "9",1u);
    ASSERT_ONE(token3, "-",1u);
    ASSERT_NONE(token3, "1");
}

int main() {
    test_anything();
    test_char();
    test_set_1();
    test_set_2();
}