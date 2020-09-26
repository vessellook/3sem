#pragma once

namespace sem3 {
    template<typename T>
    void Swap(T &a, T &b) {
        T temp = std::move(a);
        a = std::move(b);
        b = std::move(temp);
    }
}