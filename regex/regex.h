#pragma once

#include <string>
#include <memory>

#include "token.h"
#include "../structures/array_sequence.h"
#include "../structures/pair.h"

namespace sem3 {

class invalidRegex : public std::exception {};

    class Token;

    class RegEx {
        std::shared_ptr<Tokens::Root> root;

    public:
        RegEx(): root(nullptr) {}

        explicit RegEx(std::string_view pattern);

        void loadPattern(std::string_view pattern);

        bool check(std::string_view sample);

        sem2::ArraySequence<std::string_view> split(std::string_view sample);

        sem3::Pair<unsigned, unsigned> getBorders();

        std::string outputXML();

    private:
        static sem2::ArraySequence<std::shared_ptr<Token>> parse_pattern(std::string_view pattern);

        static unsigned defineEntries(std::string_view pattern, Token *token, unsigned index);

    };
}
