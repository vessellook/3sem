#include <iostream>
#include <climits>

#include "../token.h"

using namespace sem3::Tokens;
template<typename T> using Array = sem2::ArraySequence<T>;

std::string Anything::outputXML(unsigned indentation) const {
    return std::string(indentation, ' ') + "<Tokens::Anything minEntries='" + std::to_string(minEntries)
           + "' maxEntries='" + std::to_string(maxEntries) + "' />";
}

void Anything::reset(std::string_view sample) {
    if (sample.size() < minEntries) {
        counter = 0;
    } else if (sample.size() < maxEntries) {
        counter = sample.size() + 1;
    } else {
        counter = maxEntries < UINT_MAX ? maxEntries + 1 : maxEntries;
    }
}
