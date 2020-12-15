#include <iostream>
#include <cstring>
#include <sstream>

#include "regex.h"

void help(std::string_view name) {
    std::cout << "NAME\n"
              << "\t" << name << " - print lines that match pattern\n"
              << "SYNOPSIS\n"
              << "\t" << name << " 'PATTERN' 'REPLACEMENT'" << std::endl;
}

std::string replace(std::string_view format, const sem2::ArraySequence<std::string_view> &values) {
    std::string result;
    for (auto i = format.find('\\'); i != std::string::npos; i = format.find('\\')) {
        if (i == format.size() - 1) {
            break;
        } else if ('0' <= format[i + 1] && format[i + 1] <= '9') {
            result += format.substr(0, i);
            if (format[i + 1] - '0' < values.getLength()) {
                result += values[format[i + 1] - '0'];
            }
            format.remove_prefix(i + 2);
        } else {
            result += format.substr(0, i);
            format.remove_prefix(i + 1);
        }
    }
    if (!format.empty()) result += format;
    return result;
}

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        help(argv[0]);
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help(argv[0]);
            return 0;
        }
    }

    std::string_view pattern = argv[1];
    std::string_view format = argv[2];
    sem3::RegEx sed(pattern);
    std::string sample;
    while (std::getline(std::cin, sample)) {
        if (sed.check(sample)) {
            auto groups = sed.split(sample);
            auto left = sed.getBorders().first;
            auto right = sed.getBorders().second;
            std::string replacement = replace(format, groups);
            std::cout << sample.substr(0, left) << replacement << sample.substr(right) << std::endl;
        }
    }
}
