#include <climits>
#include <memory>

#include "regex.h"
#include "token.h"
#include "../utils.h"

using namespace sem3;
using std::string;
using std::string_view;
using std::shared_ptr;
using std::make_shared;

template<typename T> using Array = sem2::ArraySequence<T>;


bool RegEx::check(string_view sample) {
    root->reset(sample);
    bool result = root->hasNextPrefix();
    if (result) {
        root->getNextPrefix(); // significant call
    }
    return result;
}

RegEx::RegEx(string_view pattern) { loadPattern(pattern); }

/**
 * Complex helper
 * call setMinEntries() and setMaxEntries() if necessary
 * return index in pattern associated with the first letter of next token
 * throws invalidRegex if there is some mistake in regex syntax (this function doesn't validate regex)
 */
unsigned RegEx::defineEntries(string_view pattern, Token *token, unsigned index) {
    if (index >= pattern.size() || !sem3::has("*?+{", pattern[index])) {
        return index;
    } else if (pattern[index] == '*') { // case *
        token->setEntries(0, UINT_MAX);
        return index + 1;
    } else if (pattern[index] == '?') { // case ?
        token->setEntries(0, 1);
        return index + 1;
    } else if (pattern[index] == '+') { // case +
        token->setEntries(1, UINT_MAX);
        return index + 1;
    } else { // cases {N,M} {N,} {,M} {N} {,}
        index++;
        unsigned N = 0, M = 0;
        while (index < pattern.size() && pattern[index] != ',' && pattern[index] != '}') {
            if (pattern[index] < '0' || '9' < pattern[index]) {
                throw invalidRegex();
            }
            N = N * 10 + pattern[index] - '0';
            index++;
        }
        if (pattern[index] == '}') {
            M = N;
        }
        if (pattern[index] == ',') {
            index++;
            if (index < pattern.size() && pattern[index] == '}') {
                M = UINT_MAX;
            }
            while (index < pattern.size() && pattern[index] != '}') {
                if (pattern[index] < '0' || '9' < pattern[index]) {
                    throw invalidRegex();
                }
                M = M * 10 + pattern[index] - '0';
                index++;
            }
        }
        if (pattern[index] != '}') {
            throw invalidRegex();
        }
        if (N > M) {
            throw invalidRegex();
        }
        token->setEntries(N, M);
        return index + 1;
    }
}

Array<shared_ptr<Token>> RegEx::parse_pattern(string_view pattern) {
    auto result = Array<shared_ptr<Token>>();
    unsigned index = 0;

    while (index < pattern.size()) {
        switch (pattern[index]) {
            case '\\':
                if (index + 1 < pattern.size()) {
                    result.append(make_shared<Tokens::Char>(pattern[index + 1]));
                    index += 2;
                } else throw invalidRegex();
                break;

            case '.': {// these curly brackets are used to create inner scope
                auto token = make_shared<Tokens::Anything>();
                index = defineEntries(pattern, token.get(), index + 1);
                result.append(std::move(token));
                break;
            }

            case '[': {// these curly brackets are used to create inner scope
                unsigned begin = index;
                while (index < pattern.size() && pattern[index] != ']') {
                    index++;
                }
                if (pattern[index] != ']') {
                    throw invalidRegex();
                }
                auto token = make_shared<Tokens::Set>(pattern.substr(begin + 1, index - begin - 1));
                index++;
                if (index >= pattern.size()) {
                    result.append(std::move(token));
                    break;
                }
                index = defineEntries(pattern, token.get(), index);
                result.append(std::move(token));
                break;
            }

            case '(': {// these curly brackets are used to create inner scope
                index++;
                unsigned begin = index;
                unsigned count = 1;
                while (index < pattern.size() && count != 0) {
                    if (pattern[index] == '(') count++;
                    if (pattern[index] == ')') count--;
                    index++;
                }
                if (count != 0) {
                    throw invalidRegex();
                }
                auto token = make_shared<Tokens::Group>(
                        std::move(parse_pattern(pattern.substr(begin, index - begin - 1))));
                if (index >= pattern.size()) {
                    result.append(std::move(token));
                    break;
                }
                index = defineEntries(pattern, token.get(), index);
                result.append(std::move(token));
                break;
            }

            default: {// these curly brackets are used to create inner scope
                auto token = make_shared<Tokens::Char>(pattern[index]);
                index = defineEntries(pattern, token.get(), index + 1);
                result.append(std::move(token));
                break;
            }
        }
    }
    return std::move(result);
}

std::string RegEx::outputXML() {
    return root->outputXML(0);
}

// run only after RegEx::check()
sem2::ArraySequence<std::string_view> RegEx::split(std::string_view sample) {
    return root->getGroups(sample);
}

void RegEx::loadPattern(std::string_view pattern) {
    std::string_view p = pattern;
    bool fromStart = !pattern.empty() && pattern.front() == '^';
    bool toEnd = !pattern.empty() && pattern.back() == '$';
    if (fromStart) {
        p.remove_prefix(1);
    }
    if (toEnd) {
        p.remove_suffix(1);
    }
    root = std::make_shared<Tokens::Root>(std::move(parse_pattern(p)), fromStart, toEnd);
}

sem3::Pair<unsigned, unsigned> RegEx::getBorders() {
    return root->getBorders();
}
