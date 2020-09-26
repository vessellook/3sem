#pragma once

#include <string>

std::string the_same(char chr) { return std::string(1, chr); }

std::string lowercase(char chr) {
    if ('A' <= chr && chr <= 'Z') chr = chr + 'a' - 'A';
    return std::string(1, chr);
}

std::string empty(char) { return ""; }

bool is_space(char chr) { return chr == ' '; }

bool is_digit(char chr) { return '0' <= chr && chr <= '9'; }

bool is_letter(char chr) { return ('a' <= chr && chr <= 'z') || ('A' <= chr && chr <= 'Z'); }

bool is_letter_or_digit(char chr) {
    return ('a' <= chr && chr <= 'z') || ('A' <= chr && chr <= 'Z') || ('0' <= chr && chr <= '9');
}

bool is_plus(char chr) { return chr == '+'; }

bool is_minus(char chr) { return chr == '-'; }

bool is_asterisk(char chr) { return chr == '*'; }

bool is_slash(char chr) { return chr == '/'; }

bool is_vertical_line(char chr) { return chr == '|'; }

bool is_point(char chr) { return chr == '.'; }

bool is_apostrophe(char chr) { return chr == '\''; }

bool is_round_bracket(char chr) { return chr == '(' || chr == ')'; }

bool is_rectangle_bracket(char chr) { return chr == '[' || chr == ']'; }

bool is_curly_bracket(char chr) { return chr == '{' || chr == '}'; }

bool is_equals_sign(char chr) { return chr == '='; }

bool is_more_sign(char chr) { return chr == '>'; }

bool is_underscope(char chr) { return chr == '_'; }

bool is_comma(char chr) { return chr == ','; }

bool is_semicolon(char chr) { return chr == ';'; }

bool is_sign(char chr) {
    return is_apostrophe(chr)
        || is_asterisk(chr)
        || is_comma(chr)
        || is_equals_sign(chr)
        || is_plus(chr)
        || is_minus(chr)
        || is_slash(chr)
        || is_semicolon(chr)
        || is_rectangle_bracket(chr)
        || is_vertical_line(chr);
}
