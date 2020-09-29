#pragma once

namespace sem3 {
    template<typename T>
    void swap(T &lhs, T &rhs) {
        T temp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(temp);
    }
}