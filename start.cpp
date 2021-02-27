//std::string patternAndFormat = argv[1];
//if (patternAndFormat.size() < 2 || patternAndFormat[0] != 's') {
//std::cerr << "invalid pattern or output format" << std::endl;
//return 0;
//}
//char separator = patternAndFormat[1];
//
//std::string parsingPattern;
//{
//std::ostringstream oss;
//oss << "^s" << separator << "([^" << separator << "]*)" << separator << "([^" << separator << "]*)"
//<< separator << "$";
//parsingPattern = oss.str();
//}
//sem3::RegEx argParser(parsingPattern);
//if (!argParser.check(argv[1])) {
//std::cerr << "invalid pattern or output format" << std::endl;
//return 0;
//}
//auto argGroups = argParser.split(argv[1]);
//if (argGroups.getLength() < 2) {
//std::cerr << "invalid pattern or output format" << std::endl;
//return 0;
//}
//std::string_view pattern = argGroups.get(1);
//std::string_view format = argGroups.get(2);
//
//std::cout << pattern << "|" << format << std::endl;

#include <iostream>
#include <vector>

class NonCopiable {
public:
    NonCopiable() = default;

    NonCopiable(const NonCopiable &) = delete;

    NonCopiable(NonCopiable &&) = default;

    NonCopiable &operator=(const NonCopiable &) = delete;

    NonCopiable &operator=(NonCopiable &&) = default;
};



class Base2 {
public:
};

class Derived2 : public Base2 {
public:
};


template<typename T>
class Base {
public:
};

template<typename T>
class Derived : public Base<T> {
public:
};

int main() {
    Base<Derived2> l = Derived<Derived2>();
    Base<Base2> k = Base<Derived2>();
}
