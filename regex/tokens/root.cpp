#include <climits>

#include "../token.h"

using namespace sem3::Tokens;
template<typename T> using Array = sem2::ArraySequence<T>;

Root::Root(Array<std::shared_ptr<Token>> &&tokens, bool fromStart, bool toFinish) :
        Group(std::move(tokens)), fromStart(fromStart), toFinish(toFinish) {
    if (!fromStart) {
        auto start = std::make_shared<Anything>();
        start->setEntries(0, UINT_MAX);
        this->tokens.prepend(std::move(start));
        prefixes.append(0);
    }
    if (!toFinish) {
        auto finish = std::make_shared<Anything>();
        finish->setEntries(0, UINT_MAX);
        this->tokens.append(std::move(finish));
        prefixes.append(0);
    }
}

std::string Root::outputXML(unsigned int indentation) const {
    std::string inner;
    for (int i = 0; i < tokens.getLength(); i++) {
        inner += tokens[i]->outputXML(indentation + 2) + '\n';
    }

    std::string indentation_str = std::string(indentation, ' ');
    return indentation_str + "<Token::Root minEntries=\"" + std::to_string(minEntries) + "\" maxEntries=\"" +
           std::to_string(maxEntries) + "\">\n" + inner +
           indentation_str + "</Token::Root>";
}

sem3::Pair<unsigned, unsigned> Root::getBorders() {
    if (!fromStart && !toFinish) {
        unsigned sum = 0;
        for (unsigned i = 0; i < prefixes.getLength() - 1; i++) {
            sum += prefixes.get(i);
        }
        return sem3::Pair(prefixes.getFirst(), sum);
    } else if (!fromStart) {
        return sem3::Pair(prefixes.getFirst(), 0u);
    } else if (!toFinish) {
        unsigned sum = 0;
        for (unsigned i = 0; i < prefixes.getLength() - 1; i++) {
            sum += prefixes.get(i);
        }
        return sem3::Pair(0u, sum);
    } else {
        return sem3::Pair(0u, 0u);
    }

}
