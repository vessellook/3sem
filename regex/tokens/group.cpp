#include <iostream>
#include <sstream>

#include "../token.h"

using namespace sem3::Tokens;
template<typename T> using Array = sem2::ArraySequence<T>;

template<typename T>
std::string to_string(const Array<T> &array) {
    std::ostringstream oss;
    oss << '[';
    for (unsigned i = 0; i < array.getLength(); i++) {
        oss << array[i] << ((i + 1 < array.getLength()) ? ", " : "");
    }
    oss << ']';
    return oss.str();
}

// настраиваем токены на строку, начиная с индекса index
bool Group::isMatch(std::string_view sample, unsigned int index) {
    auto *token = tokens[index].get();
    token->reset(sample);
    if (token == tokens.getLast().get()) {
        if (token->hasNextPrefix()) {
            prefixes[index] = token->getNextPrefix();
            return true;
        } else {
            return false;
        }
    }
    return tryUpdate(sample, index);
}

bool Group::tryUpdate(std::string_view sample, unsigned int index) {
    auto *token = tokens[index].get();
    while (token->hasNextPrefix()) {
        unsigned prefix = token->getNextPrefix();
        if (isMatch(sample.substr(prefix), index + 1)) {
            prefixes[index] = prefix;
            return true;
        }
    }
    return false;
}

void Group::update(std::string_view sample) {
    for (unsigned i = tokens.getLength(); i > 1; i--) {
        if (tokens[i - 1]->hasNextPrefix()) {
            isMatch(sample, i - 1);
            break;
        }
    }
}

bool Group::hasNextPrefix() const {
    return counter != 0;
}

unsigned Group::getNext(std::string_view sample) {
    unsigned groupPrefix = 0;
    for (auto i = 0; i < prefixes.getLength(); i++) {
        groupPrefix += prefixes[i];
    }
    update(sample);
    return groupPrefix;
}

unsigned Group::getNextPrefix() {
    counter--;
    return globalPrefixes[counter];
}

Group::Group(Array<std::shared_ptr<Token>> &&tokens) : prefixes(tokens.getLength()), tokens(std::move(tokens)) {}

std::string Group::outputXML(unsigned indentation) const {
    std::string inner;
    for (int i = 0; i < tokens.getLength(); i++) {
        inner += tokens[i]->outputXML(indentation + 2) + '\n';
    }
    std::string indentation_str = std::string(indentation, ' ');
    return indentation_str + "<Tokens::Group minEntries='" + std::to_string(minEntries) + "' maxEntries='" +
           std::to_string(maxEntries) + "' >\n" + inner + indentation_str + "</Token::Group>";
}

// настраиваем все дочерние токены на строку и записываем префиксы
void Group::reset(std::string_view sample) {
    globalPrefixes = Array<unsigned>();
    unsigned prefix = 0;
    counter = 0; /* сбрасываем счётчик префиксов */
    for (unsigned i = 0; i < minEntries; i++) {
        if (!isMatch(sample.substr(prefix), 0)) {
            return;
        }
        prefix += getNext(sample);
    }
    for (unsigned i = minEntries; i <= maxEntries; i++) {
        if (!isMatch(sample.substr(prefix), 0)) {
            globalPrefixes.append(prefix);
            break;
        } else {
            globalPrefixes.append(prefix);
            prefix += getNext(sample);
            if (!globalPrefixes.empty() && prefix == globalPrefixes.getLast()) break;
        }
    }
    counter = globalPrefixes.getLength(); /* устанавливаем счётчик префиксов */
}

sem2::ArraySequence<std::string_view> Group::getGroups(std::string_view sample) const {
    auto result = sem2::ArraySequence<std::string_view>();
    unsigned sum = 0;
    if (prefixes.empty()) return result;
    for (auto i = 0; i < prefixes.getLength(); i++) {
        sum += prefixes[i];
    }
    result.append(sample.substr(0, sum));
    for (unsigned index = 0; index < tokens.getLength(); index++) {
        result.concat(tokens[index]->getGroups(sample));
        sample.remove_prefix(prefixes[index]);
    }
    return result;
}
