#include "../token.h"

using namespace sem3::Tokens;
template<typename T> using Array = sem2::ArraySequence<T>;

std::string Char::outputXML(unsigned indentation) const {
    return std::string(indentation, ' ') + "<Tokens::Char minEntries='" + std::to_string(minEntries) + "' maxEntries='" +
           std::to_string(maxEntries) +
           "' value='" + c + "' />";
}

Char::Char(char character) : c(character) {}

void Char::reset(std::string_view sample) {
    copiesCount = 0;
    counter = 1;
    for (unsigned i = 0; i < minEntries; i++) {
        if (sample.empty() || sample.front() != c) {
            return;
        }
        sample.remove_prefix(1);
    }
    for (unsigned i = minEntries; i <= maxEntries; i++) {
        if (sample.empty() || sample.front() != c) {
            copiesCount = i;
            counter = 0;
            return;
        }
        sample.remove_prefix(1);
    }
    copiesCount = maxEntries;
    counter = 0;
}

bool Char::hasNextPrefix() const {
    return counter + minEntries <= copiesCount;
}

unsigned Char::getNextPrefix() {
    return copiesCount - counter++;
}
