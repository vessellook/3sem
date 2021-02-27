#pragma once

#include <string>

namespace sem3 {
    template<typename First, typename Second>
    class Pair {
    public:

        First first;
        Second second;

        Pair() = default;

        Pair(const Pair &pair) = default;

        Pair(Pair &&pair) noexcept = default;

        Pair &operator=(const Pair &pair) = default;

        Pair &operator=(Pair &&pair) noexcept = default;

        Pair(First first, Second second) : first(std::move(first)), second(std::move(second)) {}

        bool operator==(const Pair &other) const {
            return first == other.first && second == other.second;
        }
    };
}

namespace std {
    template <typename First, typename Second>
    std::string to_string(const sem3::Pair<First, Second> &pair) {
        return "<Pair first='" + to_string(pair.first) + "', second='" + to_string(pair.second) + "' />";
    }
}