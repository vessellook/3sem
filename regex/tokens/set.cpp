#include "../token.h"

using namespace sem3::Tokens;
template<typename T> using Array = sem2::ArraySequence<T>;

Set::Set(std::string_view pattern) {
    if (!pattern.empty() && pattern.front() == '^') {
        isPositive = false;
        set = pattern.substr(1);
    } else {
        set = pattern;
    }
}

std::string Set::outputXML(unsigned indentation) const {
    return std::string(indentation, ' ') + "<Tokens::Set minEntries='" + std::to_string(minEntries) + "' maxEntries='" +
           std::to_string(maxEntries) +
           "' values='" + set + "' isPositive='" + (isPositive ? "true" : "false") + "' />";
}

void Set::reset(std::string_view sample) {
    copiesCount = 0;
    counter = 1;
    for (unsigned i = 0; i < minEntries; i++) {
        if (sample.empty() || this->has(sample.front()) != isPositive) {
            return;
        }
        sample.remove_prefix(1);
    }
    for (unsigned i = minEntries; i <= maxEntries; i++) {
        if (sample.empty() || this->has(sample.front()) != isPositive) {
            copiesCount = i;
            counter = 0;
            return;
        }
        sample.remove_prefix(1);
    }
    copiesCount = maxEntries;
    counter = 0;
}

bool Set::hasNextPrefix() const {
    return counter + minEntries <= copiesCount;
}

unsigned Set::getNextPrefix() {
    return copiesCount - counter++;
}

bool Set::has(char c) {
    if (c == '-') {
        return set.find(c) == 0 || set.find(c) == set.size() - 1;
    } else {
        if (set.find(c) != std::string::npos) {
            return true;
        } else {
            for (auto i = set.find('-'); i != set.size() - 1; i = set.find('-', i + 1)) {
                if (i == 0) {
                    continue;
                }
                if (i == std::string::npos) {
                    break;
                }
                if (set[i - 1] <= c && c <= set[i + 1]) {
                    return true;
                }
            }
            return false;
        }
    }
}
