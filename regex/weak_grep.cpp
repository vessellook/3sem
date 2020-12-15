#include <iostream>
#include <cstring>

#include "regex.h"

void help(std::string_view name) {
    std::cout << "NAME\n"
              << "\t" << name << " - print lines that match pattern\n"
              << "SYNOPSIS\n"
              << "\t" << name << " 'PATTERN'\n"
              << "\t" << name << " -i\n"
              << "OPTIONS\n"
              << "\t-i\n"
              << "\t\tinteractive mode. Prompt 'p>' for pattern (no changes on empty input). "
                 "Prompt 's>' for sample line. Print Y if sample matches pattern, otherwise N. Nothing for empty input"
              << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        help(argv[0]);
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help(argv[0]);
            return 0;
        }
    }

//#define DEBUG

#ifdef DEBUG

    std::string pattern = "\\( *( *[a-z0-9_]* *,?)* *( *[a-z_][a-z0-9_]* *= *[a-z_][a-z0-9_]*,?)*\\)";

    std::string sample = "def build_as_zip(builder=build):";

    sem3::RegEx grep(pattern);
    std::cout << grep.outputXML() << std::endl;
    std::cout << grep.check(sample) << std::endl;

#else
    if (strcmp(argv[1], "-i") == 0) {
        // interactive mode
        sem3::RegEx grep;
        std::string pattern, sample;
        bool flag = true;

        for (std::cout << "p>"; std::getline(std::cin, pattern); std::cout << "p>") {
            if (!pattern.empty()) {
                try {
                    grep.loadPattern(pattern);
                    break;
                } catch (sem3::invalidRegex &e) {
                    std::cerr << "invalid regex" << std::endl;
                }
            }
        }

        while (true) {
            for (std::cout << "s> "; std::getline(std::cin, sample); std::cout << "s> ") {
                if (!sample.empty()) {
                    break;
                }
            }
            if (std::cin.eof()) break;
            if (grep.check(sample)) {
                std::cout << sample << std::endl;
            }
            for (std::cout << "p> "; std::getline(std::cin, pattern); std::cout << "p> ") {
                if (!pattern.empty()) {
                    try {
                        grep.loadPattern(pattern);
                        break;
                    } catch (sem3::invalidRegex &e) {
                        std::cerr << "invalid regex" << std::endl;
                    }
                } else {
                    break;
                }
            }
            if (std::cin.eof()) break;
        }
    } else {
        sem3::RegEx grep;
        try {
            grep.loadPattern(argv[1]);
        } catch (sem3::invalidRegex &e) {
            std::cerr << "invalid regex" << std::endl;
            return 0;
        }
        std::string sample;
        while (std::getline(std::cin, sample)) {
            if (grep.check(sample)) {
                std::cout << sample << std::endl;
            }
        }
    }
#endif
}
