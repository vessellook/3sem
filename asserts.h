#pragma once

#include <string>
#include <exception>

namespace tests {
    struct AssertError : public std::exception {
        std::string _message;

        explicit AssertError(const std::string &message) { _message = message; }

        const char *what() const noexcept override { return _message.c_str(); }
    };

//    template<class T>
//    bool ASSERT_EQUAL(T value, T expected_value) {
//        if (value == expected_value) return true;
//        throw AssertError("expected " + std::to_string(expected_value) + "; got " + std::to_string(value));
//    }

    template<class T>
    bool assert_equal(T value, T expected_value, const std::string &file, int line) {
        if (value == expected_value) return true;
        throw AssertError("expected " + std::to_string(expected_value) + "; got " + std::to_string(value)
                          + " [" + file + ", line " + std::to_string(line) + ']');
    }

#define ASSERT_EQUAL(a, b) tests::assert_equal(a, b, __FILE__, __LINE__)
}

