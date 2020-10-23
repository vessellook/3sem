#pragma once

#include <string>
#include <exception>

namespace sem2 {

    struct MyError : public std::exception {
        std::string message;

        [[maybe_unused]] explicit MyError(const std::string &message) {
            this->message = message;
        }

        MyError(const std::string &message,
                const std::string &file,
                const std::string &func,
                unsigned line) {
            this->message = message + " [ERROR in " + file + " in " + func + ", line " + std::to_string(line) + "]";
        }

        [[nodiscard]] const char *what() const noexcept override {
            return message.c_str();
        }
    };

    struct OutOfRangeError : public MyError {
        OutOfRangeError(const std::string &message,
                        const std::string &file,
                        const std::string &func,
                        unsigned line) : MyError(message, file, func, line) {};
    };

    struct [[maybe_unused]] MemoryAllocationError : public MyError {
        [[maybe_unused]] MemoryAllocationError(const std::string &message,
                              const std::string &file,
                              const std::string &func,
                              unsigned line) : MyError(message, file, func, line) {};
    };

    struct resizeToNegativeSizeError : public MyError {
        resizeToNegativeSizeError(const std::string &message,
                                  const std::string &file,
                                  const std::string &func,
                                  unsigned line) : MyError(message, file, func, line) {};
    };

    struct EmptyValueError : public MyError {
        EmptyValueError(const std::string &message,
                        const std::string &file,
                        const std::string &func,
                        unsigned line) : MyError(message, file, func, line) {};
    };
}