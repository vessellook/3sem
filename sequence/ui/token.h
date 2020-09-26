#pragma once

#include <string>

class Token {
private:
    std::string word_;
    std::string type_;
public:
    Token(): word_(), type_() {}
    Token(std::string word, std::string type): word_(std::move(word)), type_(std::move(type)) {}
    std::string getWord() { return word_; }
    std::string getType() { return type_; }
};

unsigned priority(Token token) {
    std::string word = token.getWord();
    if(word == "*" || word == "/") {
        return 4;
    }
    if(word == "+" || word == "-") {
        return 3;
    }
    if(word == "=") {
        return 2;
    }
    return 0;
};