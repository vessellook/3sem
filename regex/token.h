#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>


#include "../structures/array_sequence.h"
#include "../structures/pair.h"

namespace sem3 {
    bool has(const std::string &set, char c);

    class Token {
    protected:
        unsigned minEntries = 1;
        unsigned maxEntries = 1;

    public:

        Token(Token &&) = default;

        Token(const Token &) = delete;

        Token() = default;

        virtual ~Token() = default;

        virtual void reset(std::string_view sample) = 0;

        [[nodiscard]] virtual bool hasNextPrefix() const = 0;

        virtual unsigned getNextPrefix() = 0;

        [[nodiscard]] virtual sem2::ArraySequence<std::string_view> getGroups(std::string_view) const {
            return sem2::ArraySequence<std::string_view>();
        }

    public:

        void setEntries(unsigned min_entries, unsigned max_entries) {
            this->minEntries = min_entries;
            this->maxEntries = max_entries;
        }

        [[nodiscard]] virtual std::string outputXML(unsigned indentation) const = 0;
    };

    namespace Tokens {
        class Char : public Token {
            char c;
            unsigned copiesCount = 0;
            unsigned counter = 0;
        public:
            explicit Char(char character);

            void reset(std::string_view sample) override;

            [[nodiscard]] bool hasNextPrefix() const override;

            unsigned getNextPrefix() override;

            [[nodiscard]] std::string outputXML(unsigned indentation) const override;
        };

        class Anything : public Token {
            unsigned counter = 0;
        public:

            Anything() = default;

            void reset(std::string_view sample) override;

            [[nodiscard]] bool hasNextPrefix() const override { return minEntries < counter; }

            unsigned getNextPrefix() override { return --counter; }

            [[nodiscard]] std::string outputXML(unsigned indentation) const override;
        };

        class Set : public Token {
            std::string set;
            bool isPositive = true;

            unsigned copiesCount = 0;
            unsigned counter = 0;
        public:
            explicit Set(std::string_view pattern);

            void reset(std::string_view sample) override;

            [[nodiscard]] bool hasNextPrefix() const override;

            unsigned getNextPrefix() override;

            [[nodiscard]] std::string outputXML(unsigned indentation) const override;

        private:
            bool has(char c);
        };

        class Group : public Token {
        public:
            explicit Group(sem2::ArraySequence<std::shared_ptr<Token>> &&tokens);

            void reset(std::string_view sample) override;

            [[nodiscard]] bool hasNextPrefix() const override;

            unsigned getNextPrefix() override;

            [[nodiscard]] sem2::ArraySequence<std::string_view> getGroups(std::string_view sample) const override;

        protected:
            sem2::ArraySequence<unsigned> prefixes;
            sem2::ArraySequence<std::shared_ptr<Token>> tokens;
            unsigned counter = 0;
            sem2::ArraySequence<unsigned> globalPrefixes;

        protected:

            void update(std::string_view sample);

            bool tryUpdate(std::string_view sample, unsigned index);

            unsigned getNext(std::string_view sample);

            bool isMatch(std::string_view sample, unsigned index);

        public:
            [[nodiscard]] std::string outputXML(unsigned indentation) const override;
        };

        class Root : public Group {
            bool fromStart;
            bool toFinish;
        public:
            explicit Root(sem2::ArraySequence<std::shared_ptr<Token>> &&tokens, bool fromStart, bool toFinish);

            [[nodiscard]] std::string outputXML(unsigned indentation) const override;

            sem3::Pair<unsigned, unsigned> getBorders();

        };
    }
}


