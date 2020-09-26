//

#include <iostream>
#include <string>
#include <memory>
#include <utility>

#include "ui/fsm.h"
#include "ui/callbacks.h"
#include "ui/token.h"
#include "ui/treenode.h"
#include "linked_list_sequence.h"
#include "linked_list.h"
#include "matrix/array_matrix.h"
#include "matrix/list_matrix.h"
#include "ui/simple_ui.h"

using namespace std;
using art::SinglyLinkedList;
using art::ArrayMatrix;
using art::ListMatrix;
using art::IMatrix;
using simple_ui::show;

string get_line() {
    string result;
    getline(cin, result);
    result += "\n";
    return result;
}

void initDefaultMatrix(FSM &machine) {
    // space
    machine.setEdge(0, 0, is_space, empty);
    machine.setEdge(0, 0, is_comma, empty);
    machine.setEdge(0, 0, is_vertical_line, empty);
//    machine.setEdge(0, 1, is_space, the_same);
//    machine.setEdge(1, 1, is_space, empty);

    // ID
    machine.setEdge(0, 2, is_letter, lowercase);
    machine.setEdge(2, 2, is_letter, lowercase);
    machine.setEdge(2, 2, is_digit, the_same);
    machine.setEdge(2, 2, is_underscope, the_same);

    // NUMBER
    machine.setEdge(0, 7, is_minus, the_same);
    machine.setEdge(7, 3, is_digit, the_same);
    machine.setEdge(0, 3, is_digit, the_same);
    machine.setEdge(3, 3, is_digit, the_same);
    machine.setEdge(3, 4, is_point, the_same);
    machine.setEdge(4, 5, is_digit, the_same);
    machine.setEdge(5, 5, is_digit, the_same);

    // SIGN
    machine.setEdge(0, 6, is_equals_sign, the_same);
    machine.setEdge(0, 6, is_plus, the_same);
//    machine.setEdge(0, 6, is_minus, the_same);
    machine.setEdge(0, 6, is_asterisk, the_same);
    machine.setEdge(0, 6, is_semicolon, the_same);
    machine.setEdge(0, 6, is_slash, the_same);
    machine.setEdge(0, 6, is_apostrophe, the_same);
    machine.setEdge(0, 6, is_round_bracket, the_same);
    machine.setEdge(0, 6, is_rectangle_bracket, the_same);
//    machine.setEdge(0, 6, is_curly_bracket, the_same);
}

std::shared_ptr<SinglyLinkedList<std::string>> get_words(const std::string &line) {
    std::shared_ptr<std::string> buffer = std::make_shared<std::string>();
    auto machine = FSM(buffer);
    initDefaultMatrix(machine);
    auto words = make_shared<SinglyLinkedList<std::string>>();
    for (char chr : line) {
        if (!machine.input(chr)) {
            words->prepend(*buffer);
            machine.restart();
            if (chr == '\n') break;
            if (!machine.input(chr)) {
                // введён недопустимый символ
                throw std::exception();
            }
        }
    }
    return words;
}

shared_ptr<SinglyLinkedList<Token>> get_tokens(shared_ptr<SinglyLinkedList<string>> words) {
    unsigned len = words->getLength();
    if (len == 0) { return make_shared<SinglyLinkedList<Token>>(); }
    auto tokens = make_shared<SinglyLinkedList<Token>>();
    {
        string word = words->get(0);
        if (is_sign(word[0])) {
            tokens->prepend(Token(word, "sign"));
        } else if (is_letter(word[0])) {
            tokens->prepend(Token(word, "id"));
        } else if (is_digit(word[0])) {
            if (word.find('.') == string::npos) {
                tokens->prepend(Token(word, "int"));
            } else {
                tokens->prepend(Token(word, "real"));
            }
        } else { throw exception(); }
    }
    if (len == 1) return tokens;
    string currWord;
    string prevWord = words->get(0);
    for (unsigned index = 1; index < len; index++) {
        currWord = words->get(index);
        if(currWord.empty()) {
            continue;
        }
        if (currWord.length() > 1 && is_minus(currWord[0])) {
            if (tokens->getLast().getType() == "int" || tokens->getLast().getType() == "real" ||
                is_letter(prevWord[0])) {
                tokens->prepend(Token("-", "sign"));
                currWord = currWord.substr(1);
                if (currWord.find('.') == string::npos) {
                    tokens->prepend(Token(currWord, "int"));
                } else {
                    tokens->prepend(Token(currWord, "real"));
                }
            } else {
                if (currWord.find('.') == string::npos) {
                    tokens->prepend(Token(currWord, "int"));
                } else {
                    tokens->prepend(Token(currWord, "real"));
                }
            }
        } else if (is_sign(currWord[0])) {
            tokens->prepend(Token(currWord, "sign"));
        } else if (is_digit(currWord[0])) {
            if (currWord.find('.') == string::npos) {
                tokens->prepend(Token(currWord, "int"));
            } else {
                tokens->prepend(Token(currWord, "real"));
            }
        } else if (is_letter(currWord[0])) {
            tokens->prepend(Token(currWord, "id"));
        } else {
            throw exception();
        }
        prevWord = currWord;
    }
    return tokens;
}

void show(const shared_ptr<SinglyLinkedList<Token>> &tokens) {
    unsigned length = tokens->getLength();
    for (unsigned i = 0; i < length; ++i) {
        cout << tokens->getRef(i).getWord() << '|';
    }
    cout << endl;
}

void show(const shared_ptr<SinglyLinkedList<string>> &words) {
    unsigned length = words->getLength();
    for (unsigned i = 0; i < length; ++i) {
        cout << words->getRef(i) << '|';
    }
    cout << endl;
}

template<class T>
struct Var {
    string name;
    T value;

    Var() : name(), value() {}

    Var(string name, T value) : name(std::move(name)), value(std::move(value)) {}
};

bool is_not_temp(shared_ptr<Var<shared_ptr<IMatrix<int>>>> var) {
    return var->name[0] != '$';
}

bool is_not_temp(shared_ptr<Var<shared_ptr<IMatrix<double>>>> var) {
    return var->name[0] != '$';
}

class Scope {
public:
    static Scope global;
public:
    SinglyLinkedList<shared_ptr<Var<shared_ptr<IMatrix<int   >>>>> list_of_int_matrix;
    SinglyLinkedList<shared_ptr<Var<shared_ptr<IMatrix<double>>>>> list_of_double_matrix;

    void removeTemps() {
        list_of_int_matrix.where(is_not_temp);
        list_of_double_matrix.where(is_not_temp);
    }

    bool hasIntMatrix(const string &name) {
        unsigned len = list_of_int_matrix.getLength();
        for (unsigned index = 0; index < len; index++) {
            if (list_of_int_matrix.getRef(index)->name == name) return true;
        }
        return false;
    }

    bool hasDoubleMatrix(const string &name) {
        unsigned len = list_of_double_matrix.getLength();
        for (unsigned index = 0; index < len; index++) {
            if (list_of_double_matrix.getRef(index)->name == name) return true;
        }
        return false;
    }

    shared_ptr<Var<shared_ptr<IMatrix<int>>>> getIntMatrix(const string &name) {
        unsigned len = list_of_int_matrix.getLength();
        shared_ptr<Var<shared_ptr<IMatrix<int>>>> candidate;
        for (unsigned index = 0; index < len; index++) {
            candidate = list_of_int_matrix.getRef(index);
            if (candidate->name == name) return candidate;
        }
        throw exception();
    }

    shared_ptr<Var<shared_ptr<IMatrix<double>>>> getDoubleMatrix(const string &name) {
        unsigned len = list_of_double_matrix.getLength();
        shared_ptr<Var<shared_ptr<IMatrix<double>>>> candidate;
        for (unsigned index = 0; index < len; index++) {
            candidate = list_of_double_matrix.getRef(index);
            if (candidate->name == name) return candidate;
        }
        throw exception();
    }

    void removeIntMatrix(const string &name) {
        unsigned len = list_of_int_matrix.getLength();
        for (unsigned index = 0; index < len; index++) {
            if (list_of_int_matrix.getRef(index)->name == name) {
                list_of_int_matrix.remove(index);
            }
        }
    }

    void removeDoubleMatrix(const string &name) {
        unsigned len = list_of_double_matrix.getLength();
        for (unsigned index = 0; index < len; index++) {
            if (list_of_double_matrix.getRef(index)->name == name) {
                list_of_double_matrix.remove(index);
                break;
            }
        }
    }
};

Scope Scope::global = Scope();

void make_matrices(shared_ptr<SinglyLinkedList<Token>> &tokens) {
    bool is_in_matrix_definition = false;
    bool is_list_matrix = false;
    bool is_int = false;
    unsigned leftindex = 0;
    unsigned rightindex = 0;
    unsigned size = 0;
    unsigned col_index = 0;
    unsigned row_index = 0;
    unsigned len = tokens->getLength();
    unsigned index = 0;
    string name;
    Token token;
    shared_ptr<IMatrix<int   >> m_i;
    shared_ptr<IMatrix<double>> m_d;

    while (index < len) {
        token = tokens->get(index);
        if (!is_in_matrix_definition) {
            if (token.getWord() == "ma_d") {
                is_in_matrix_definition = true;
                leftindex = index;
            } else if (token.getWord() == "ma_i") {
                is_in_matrix_definition = true;
                is_int = true;
                leftindex = index;
            } else if (token.getWord() == "ml_d") {
                is_in_matrix_definition = true;
                is_list_matrix = true;
                leftindex = index;
            } else if (token.getWord() == "ml_i") {
                is_in_matrix_definition = true;
                is_int = true;
                is_list_matrix = true;
                leftindex = index;
            }
        } else {
            if (token.getWord() == "[") {
            } else if (token.getWord() == "]") {
                if (is_int) {
                    name = "$i" + to_string(Scope::global.list_of_int_matrix.getLength());
                    Scope::global.list_of_int_matrix.append(make_shared<Var<shared_ptr<IMatrix<int>>>>(name, m_i));
                    m_i.reset();
                } else {
                    name = "$d" + to_string(Scope::global.list_of_double_matrix.getLength());
                    Scope::global.list_of_double_matrix.append(
                            make_shared<Var<shared_ptr<IMatrix<double>>>>(name, m_d));
                    m_d.reset();
                }
                rightindex = index;
                index = leftindex;
                tokens->cut(leftindex + 1, rightindex);
                tokens->set(leftindex, Token(name, "id"));
                is_in_matrix_definition = false;
                is_int = false;
                is_list_matrix = false;
                size = 0;
                len = tokens->getLength();
            } else if (size == 0 && (token.getType() == "int" || token.getType() == "real")) {
                size = stoi(token.getWord());
                if (!is_int && !is_list_matrix) {
                    m_d = make_shared<ArrayMatrix<double>>(size);
                    for (unsigned i = 0; i < size; i++) for (unsigned j = 0; j < size; j++) m_d->set(i, j, 0);
                } else if (!is_list_matrix) {
                    m_i = make_shared<ArrayMatrix<int>>(size);
                    for (unsigned i = 0; i < size; i++) for (unsigned j = 0; j < size; j++) m_i->set(i, j, 0);
                } else if (!is_int) {
                    m_d = make_shared<ListMatrix<double>>(size);
                    for (unsigned i = 0; i < size; i++) for (unsigned j = 0; j < size; j++) m_d->set(i, j, 0);
                } else {
                    m_i = make_shared<ListMatrix<int>>(size);
                    for (unsigned i = 0; i < size; i++) for (unsigned j = 0; j < size; j++) m_i->set(i, j, 0);
                }
            } else if (size == 0) {
                throw exception();
            } else if ((is_int && token.getType() == "int") || (!is_int && token.getType() == "real")) {
                if (col_index == size) {
                    col_index = 0;
                    row_index++;
                }
                if (row_index == size) {
                    throw exception();
                }
                if (is_int) {
                    m_i->set(col_index, row_index, stoi(token.getWord()));
                } else {
                    m_d->set(col_index, row_index, stod(token.getWord()));
                }
                col_index++;
            } else if (token.getWord() == ";") {
                col_index = 0;
                row_index++;
            } else {
                throw exception();
            }
        }
        index++;
    }
}

void execute_methods(shared_ptr<SinglyLinkedList<Token>> &tokens) {
    Token matrix_token;
    Token token;
    Token method;
    unsigned len = tokens->getLength();
    for (unsigned index = 0; index < len; index++) {
        token = tokens->getRef(index);
        if (token.getWord() == "'") {
            matrix_token = tokens->getRef(index - 1);
            method = tokens->getRef(index + 1);
            if (matrix_token.getType() != "id" || method.getType() != "id") throw exception();
            if (Scope::global.hasIntMatrix(matrix_token.getWord())) {
                auto matrix = Scope::global.getIntMatrix(matrix_token.getWord());
                if (method.getWord() == "transpose") {
                    matrix->value->transpose();
                    tokens->cut(index, index + 1);
                    len = tokens->getLength();
                    index = index - 1;
                } else if (method.getWord() == "clone") {
                    string name = "$i" + to_string(Scope::global.list_of_int_matrix.getLength());
                    auto matrix_ptr = shared_ptr<IMatrix<int>>(matrix->value->clone());
                    auto ptr = make_shared<Var<shared_ptr<IMatrix<int>>>>(name, matrix_ptr);
                    Scope::global.list_of_int_matrix.append(ptr);
                    tokens->cut(index, index + 1);
                    len = tokens->getLength();
                    index = index - 1;
                    tokens->getRef(index) = Token(name, "id");
                } else if (method.getWord() == "set") {
                    Token row_index = tokens->getRef(index + 2);
                    Token col_index = tokens->getRef(index + 3);
                    Token value = tokens->getRef(index + 4);
                    matrix->value->set(stoi(col_index.getWord()) - 1, stoi(row_index.getWord()) - 1,
                                       stoi(value.getWord()));
                    tokens->cut(index, index + 4);
                    len = tokens->getLength();
                    index = index - 1;
                } else if (method.getWord() == "print") {
                    show(matrix->value);
                    tokens->cut(index, index + 1);
                    len = tokens->getLength();
                    index = index - 1;
                }
            } else if (Scope::global.hasDoubleMatrix(matrix_token.getWord())) {
                auto matrix = Scope::global.getDoubleMatrix(matrix_token.getWord());
                if (method.getWord() == "transpose") {
                    matrix->value->transpose();
                    tokens->cut(index, index + 1);
                    len = tokens->getLength();
                    index = index - 1;
                } else if (method.getWord() == "clone") {
                    string name = "$d" + to_string(Scope::global.list_of_double_matrix.getLength());
                    auto matrix_ptr = shared_ptr<IMatrix<double>>(matrix->value->clone());
                    auto ptr = make_shared<Var<shared_ptr<IMatrix<double>>>>(name, matrix_ptr);
                    Scope::global.list_of_double_matrix.append(ptr);
                    tokens->cut(index, index + 1);
                    len = tokens->getLength();
                    index = index - 1;
                    tokens->getRef(index) = Token(name, "id");
                } else if (method.getWord() == "set") {
                    Token row_index = tokens->getRef(index + 2);
                    Token col_index = tokens->getRef(index + 3);
                    Token value = tokens->getRef(index + 4);
                    matrix->value->set(stoi(col_index.getWord()) - 1, stoi(row_index.getWord()) - 1,
                                       stod(value.getWord()));
                    tokens->cut(index, index + 4);
                    len = tokens->getLength();
                    index = index - 1;
                } else if (method.getWord() == "print") {
                    show(matrix->value);
                    tokens->cut(index, index + 1);
                    len = tokens->getLength();
                    index = index - 1;
                }
            } else throw exception();
        }
    }
}

void check_func(const shared_ptr<TreeNode> &root) {
    if(root == nullptr) return;
    cout << root->getToken().getWord();
    auto children = root->getChildren();
    if(children->getLength() == 0) return;
    cout << "[";
    check_func(children->getRef(0));
    cout << ",";
    if (children->getLength() > 1) {
        check_func(children->getRef(1));
    }
    cout << "]";
}

shared_ptr<TreeNode> build_tree(const shared_ptr<SinglyLinkedList<Token>> &tokens, unsigned &index) {
    unsigned length = tokens->getLength();
    Token token;
    shared_ptr<TreeNode> root;
    shared_ptr<TreeNode> operand;
    shared_ptr<TreeNode> previous_operation;
    while (index < length) {
        token = tokens->getRef(index);
        if (token.getType() != "sign") {
            operand = make_shared<TreeNode>(token);
            if(root == nullptr) root = operand;
        } else if (token.getWord() == "(") {
            shared_ptr<TreeNode> subroot(nullptr);
            index++;
            operand = build_tree(tokens, index);
        } else if (token.getWord() == ")") {
            if (previous_operation != nullptr) previous_operation->addChild(operand);
            return root;
        } else {
            unsigned current_priority = priority(token);
            auto current_operation = make_shared<TreeNode>(Token(token));
            while (true) {
                if(operand == nullptr) {
                    throw exception();
                }
                if(previous_operation == nullptr) {
                    previous_operation = current_operation;
                    root = current_operation;
                    current_operation->addChild(operand);
                    operand->setParent(current_operation);
                    break;
                } else if (priority(previous_operation->getToken()) < current_priority) { // случай 1 + 2 *
                    previous_operation->addChild(current_operation);
                    current_operation->setParent(previous_operation);
                    current_operation->addChild(operand);
                    operand->setParent(current_operation);
                    previous_operation = current_operation;
                    break;
                } else { // случай 1 * 2 +
                    previous_operation->addChild(operand);
                    operand->setParent(previous_operation);
                    operand = previous_operation;
                    if (previous_operation->getParent() == nullptr) {// достигли корня
                        current_operation->addChild(previous_operation);
                        previous_operation->setParent(current_operation);
                        root = current_operation;
                        previous_operation = current_operation;
                        break;
                    }
                    previous_operation = previous_operation->getParent();
                    previous_operation->removeLastChild();
                }
            }
        }
        index++;
    }
    if (previous_operation != nullptr) previous_operation->addChild(operand);
    return root;
}

shared_ptr<TreeNode> build_tree(const shared_ptr<SinglyLinkedList<Token>> &tokens) {
    unsigned start_index = 0;
    return build_tree(tokens, start_index);
}

void get_var(const string& name, shared_ptr<Var<shared_ptr<IMatrix<int>>>>& int_buffer, shared_ptr<Var<shared_ptr<IMatrix<double>>>>& double_buffer, int& flag) {
    flag = 0;
    unsigned len = Scope::global.list_of_int_matrix.getLength();
    for(unsigned i = 0; i < len; i++) {
        shared_ptr<Var<shared_ptr<IMatrix<int>>>>& current_matrix = Scope::global.list_of_int_matrix.getRef(i);
        if(name == current_matrix->name) {
            flag = 1;
            int_buffer = current_matrix;
            return;
        }
    }
    len = Scope::global.list_of_double_matrix.getLength();
    for(unsigned i = 0; i < len; i++) {
        shared_ptr<Var<shared_ptr<IMatrix<double>>>>& current_matrix = Scope::global.list_of_double_matrix.getRef(i);
        if(name == current_matrix->name) {
            flag = -1;
            double_buffer = current_matrix;
            return;
        }
    }
}

void execute_treenode(shared_ptr<TreeNode>& node) {
    if(node == nullptr) return;
    auto children = node->getChildren();
    for(unsigned i = 0; i < children->getLength(); i++) {
        execute_treenode(children->getRef(i));
    }
    string word = node->getToken().getWord();
    if(word == "=") {
        auto left_token = children->getRef(1)->getToken();
        auto right_token = children->getRef(0)->getToken();
        if(left_token.getType() != "id") throw exception();
        if(right_token.getType() != "id") throw exception();
        shared_ptr<Var<shared_ptr<IMatrix<int>>>> right_int_matrix;
        shared_ptr<Var<shared_ptr<IMatrix<double>>>> right_double_matrix;
        int right_flag;
        get_var(right_token.getWord(), right_int_matrix, right_double_matrix, right_flag);
        if(right_flag == 0) throw exception();
        shared_ptr<Var<shared_ptr<IMatrix<int>>>> left_int_matrix;
        shared_ptr<Var<shared_ptr<IMatrix<double>>>> left_double_matrix;
        int left_flag;
        get_var(left_token.getWord(), left_int_matrix, left_double_matrix, left_flag);
        if(right_flag == left_flag && right_flag == 1) {
            left_int_matrix->value = right_int_matrix->value;
        } else if(right_flag == left_flag && right_flag == -1) {
            left_double_matrix->value = right_double_matrix->value;
        } else if (right_flag == 1 && left_flag == 0) {
            Scope::global.list_of_int_matrix.prepend(make_shared<Var<shared_ptr<IMatrix<int>>>>(left_token.getWord(), right_int_matrix->value));
        } else if(right_flag == -1 && left_flag == 0) {
            Scope::global.list_of_double_matrix.prepend(make_shared<Var<shared_ptr<IMatrix<double>>>>(left_token.getWord(), right_double_matrix->value));
        } else throw exception();
        node->setToken(Token(left_token.getWord(), "id"));
    } else if(word == "+" || word == "-" || word == "*" || word == "/") {
        auto left_token = children->getRef(1)->getToken();
        auto right_token = children->getRef(0)->getToken();
        bool right_operand_is_matrix = right_token.getType() == "id";
        if(left_token.getType() != "id") throw exception();
        if(!right_operand_is_matrix && right_token.getType() != "int") throw exception();

        if(right_operand_is_matrix) {
            shared_ptr<Var<shared_ptr<IMatrix<int>>>> right_int_matrix;
            shared_ptr<Var<shared_ptr<IMatrix<double>>>> right_double_matrix;
            int right_flag;
            get_var(right_token.getWord(), right_int_matrix, right_double_matrix, right_flag);
            if(right_flag == 0) throw exception();
            shared_ptr<Var<shared_ptr<IMatrix<int>>>> left_int_matrix;
            shared_ptr<Var<shared_ptr<IMatrix<double>>>> left_double_matrix;
            int left_flag;
            get_var(left_token.getWord(), left_int_matrix, left_double_matrix, left_flag);
            string name;
            if(right_flag == left_flag && right_flag == 1) {
                shared_ptr<IMatrix<int>> new_matrix;
                if(word == "+") new_matrix = shared_ptr<IMatrix<int>>(&(*(left_int_matrix->value) + *(right_int_matrix->value)));
                else if(word == "-") new_matrix = shared_ptr<IMatrix<int>>(&(*(left_int_matrix->value) - *(right_int_matrix->value)));
                else if(word == "*") new_matrix = shared_ptr<IMatrix<int>>(&(*(left_int_matrix->value) * *(right_int_matrix->value)));
//                else if(word == "/") new_matrix = shared_ptr<IMatrix<int>>(&(*(left_int_matrix->value) / *(right_int_matrix->value)));
                name = "$i" + to_string(Scope::global.list_of_int_matrix.getLength());
                Scope::global.list_of_int_matrix.prepend(make_shared<Var<shared_ptr<IMatrix<int>>>>(name, new_matrix));
            } else if(right_flag == left_flag && right_flag == -1) {
                shared_ptr<IMatrix<double>> new_matrix;
                if(word == "+") new_matrix = shared_ptr<IMatrix<double>>(&(*(left_double_matrix->value) + *(right_double_matrix->value)));
                else if(word == "-") new_matrix = shared_ptr<IMatrix<double>>(&(*(left_double_matrix->value) - *(right_double_matrix->value)));
                else if(word == "*") new_matrix = shared_ptr<IMatrix<double>>(&(*(left_double_matrix->value) * *(right_double_matrix->value)));
//                else if(word == "/") new_matrix = shared_ptr<IMatrix<int>>(&(*(left_int_matrix->value) / *(right_int_matrix->value))));
                name = "$d" + to_string(Scope::global.list_of_double_matrix.getLength());
                Scope::global.list_of_double_matrix.prepend(make_shared<Var<shared_ptr<IMatrix<double>>>>(name, new_matrix));
            } else throw exception();
            node->setToken(Token(name, "id"));
        } else {
            shared_ptr<Var<shared_ptr<IMatrix<int>>>> left_int_matrix;
            shared_ptr<Var<shared_ptr<IMatrix<double>>>> left_double_matrix;
            int left_flag;
            get_var(left_token.getWord(), left_int_matrix, left_double_matrix, left_flag);
            string name;
            if(left_flag == 1) {
                shared_ptr<IMatrix<int>> new_matrix;
                if(word == "*") new_matrix = shared_ptr<IMatrix<int>>(&(*(left_int_matrix->value) * stoi(right_token.getWord())));
//                else if(word == "-") new_matrix = shared_ptr<IMatrix<int>>(&(*(left_double_matrix->value) - stoi(right_token.getWord()));
//                else if(word == "+") new_matrix = shared_ptr<IMatrix<int>>(&(*(left_double_matrix->value) + stoi(right_token.getWord()));
                //else if(word == "/") new_matrix = shared_ptr<IMatrix<double>>(&(*(left_double_matrix->value) / stod(right_token.getWord()));
                else throw exception();
                name = "$i" + to_string(Scope::global.list_of_int_matrix.getLength());
                Scope::global.list_of_int_matrix.prepend(make_shared<Var<shared_ptr<IMatrix<int>>>>(name, new_matrix));
            } else if(left_flag == -1) {
                shared_ptr<IMatrix<double>> new_matrix;
                if(word == "*") new_matrix = shared_ptr<IMatrix<double>>(&(*(left_double_matrix->value) * stod(right_token.getWord())));
//                else if(word == "-") new_matrix = shared_ptr<IMatrix<double>>(&(*(left_double_matrix->value) - stod(right_token.getWord()));
//                else if(word == "+") new_matrix = shared_ptr<IMatrix<double>>(&(*(left_double_matrix->value) + stod(right_token.getWord()));
                //else if(word == "/") new_matrix = shared_ptr<IMatrix<double>>(&(*(left_double_matrix->value) / stod(right_token.getWord()));
                else throw exception();
                name = "$d" + to_string(Scope::global.list_of_double_matrix.getLength());
                Scope::global.list_of_double_matrix.prepend(make_shared<Var<shared_ptr<IMatrix<double>>>>(name, new_matrix));
            } else throw exception();
            node->setToken(Token(name, "id"));
        }
    }
}

Token execute_tree(shared_ptr<TreeNode> root) {
    execute_treenode(root);
    return root->getToken();
}

void execute_operations(shared_ptr<SinglyLinkedList<Token>> &tokens) {
    shared_ptr<TreeNode> root = build_tree(tokens);
    execute_tree(root);
}

void execute(shared_ptr<SinglyLinkedList<Token>> &tokens) {
    string word;
    make_matrices(tokens);
    execute_methods(tokens);
    execute_operations(tokens);
    Scope::global.removeTemps();
}

int main() {
    bool stop = false;

    while (!stop) {
        cout << ">>>";
        string line = get_line();
        if (line == "exit \n") break;
        shared_ptr<SinglyLinkedList<Token>> tokens;
        try {
            auto words = get_words(line);
            if(words->getLength() == 0) continue;
            tokens = get_tokens(words);
            execute(tokens);
        } catch (exception &) {
            cout << "SyntaxError" << endl;
            continue;
        }
    }

    return 0;
}
