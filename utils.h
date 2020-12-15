#pragma once

#include <string>
#include <stdexcept>

namespace sem3 {
    bool has(const std::string &set, char c) {
        return set.find(c) != std::string::npos;
    }
}