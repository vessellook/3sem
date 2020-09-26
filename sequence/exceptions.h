#pragma once

#include <string>
#include <exception>

namespace sem2 {

    struct MyError : public std::exception {
        std::string message_;
        std::string file_;
        std::string func_;
        unsigned line_;

        explicit MyError(const std::string &message) {
            message_ = message;
        }

        MyError(const std::string &message,
                const std::string &file,
                const std::string &func,
                unsigned line) {
            message_ = message;
            file_ = file;
            func_ = func;
            line_ = line;
        }

        const char *what() const noexcept override {
            return (message_ + " [ERROR in " + file_ + " in " + func_ + ", line " + std::to_string(line_) +
                    "]").c_str();
        }
    };

    struct OutOfRangeError : public MyError {
        OutOfRangeError(const std::string &message,
                        const std::string &file,
                        const std::string &func,
                        unsigned line) : MyError(message, file, func, line) {};
    };

    struct MemoryAllocationError : public MyError {
        MemoryAllocationError(const std::string &message,
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

    struct ZeroSizeOfMatrixError : public MyError {
        ZeroSizeOfMatrixError(const std::string &message,
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