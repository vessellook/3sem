#include <string>
#include <string_view>
#include <any>
#include <iostream>
#include <charconv>

#include "../structures/hash_map.h"
#include "../structures/sparse_matrix.h"
#include "../regex/regex.h"
#include "table_printer.h"

using namespace sem3;

std::string_view getFirstWord(std::string_view view) {
    if (auto index = view.find_first_not_of(' '); index != std::string::npos) {
        view = view.substr(index);
        if (view.empty()) return view;
        return view.substr(0, view.find_first_of(" \n"));
    }
    return std::string_view();
}

using Row = sem2::ArraySequence<std::string>;

class Manager {
    HashMap<std::string, SparseMatrix<int>> matrices = HashMap<std::string, SparseMatrix<int>>();
    std::string current;

    static void output(std::ostream &os, const SparseMatrix<int> &matrix) {
        auto rows = matrix.getNotEmptyRows();
        auto cols = SortedSequence<unsigned>();
        for (auto index = 0u; index < rows.getLength(); index++) {
            cols.merge(matrix.getColsByRow(rows.get(index)));
        }
        if (rows.getLength() == 0 || cols.getLength() == 0) return;

        auto colsHaveGap = [&cols](unsigned index) { return cols.get(index - 1) + 1 != cols.get(index); };
        auto rowsHaveGap = [&rows](unsigned index) { return rows.get(index - 1) + 1 != rows.get(index); };

        auto count = 1 + cols.getLength();
        for (auto index = 1u; index < cols.getLength(); index++) {
            if (colsHaveGap(index)) count++;
        }

        auto printer = TablePrinter(count);

        Row header;
        header.append("");
        for (auto colIndex = 0u; colIndex < cols.getLength(); colIndex++) {
            if (colIndex > 0 && colsHaveGap(colIndex)) {
                header.append(cols.get(colIndex) - cols.get(colIndex - 1) == 2 ?
                              std::to_string(cols.get(colIndex) - 1) : "");  // to show gap
            }
            header.append(std::to_string(cols.get(colIndex)));
        }
        printer.addRow(header);


        for (auto rowIndex = 0u; rowIndex < rows.getLength(); rowIndex++) {
            if (rowIndex != 0u && rowsHaveGap(rowIndex)) {
                printer.addEmptyRow();  // to show gap
            }
            Row rowCells;
            auto rowStr = std::to_string(rows.get(rowIndex));
            rowCells.append(std::move(rowStr));
            for (auto colIndex = 0u; colIndex < cols.getLength(); colIndex++) {
                if (colIndex > 0 && colsHaveGap(colIndex)) {
                    rowCells.append(cols.get(colIndex) - cols.get(colIndex - 1) == 2 ?
                                    "0" : "");  // to show gap
                }
                auto value = matrix.get(cols.get(colIndex), rows.get(rowIndex));
                auto valueStr = std::to_string(value);
                rowCells.append(std::move(valueStr));
            }
            printer.addRow(std::move(rowCells));
        }
        printer.print(os);
    }

public:
    const std::string &getCurrentName() {
        return current;
    }

    bool create(const std::string &line) {
        RegEx regex("^ *create +([^ ]+) +([0-9]+) +([0-9]+) *$");
        if (regex.check(line)) {
            auto groups = regex.split(line);
            auto name = std::string(groups[1]);
            unsigned long m, n;
            auto result1 = std::from_chars(groups[2].data(), groups[2].data() + groups[2].size(), m);
            auto result2 = std::from_chars(groups[3].data(), groups[3].data() + groups[3].size(), n);
            if (result1.ec == std::errc::invalid_argument || result2.ec == std::errc::invalid_argument) {
                return false;
            }
            auto matrix = SparseMatrix<int>(m, n);
            matrices.add(name, std::move(matrix));
            current = std::move(name);
            return true;
        }
        return false;
    }

    bool show(const std::string &line) {
        if (current.empty()) return false;
        RegEx regex("^ *show *$");
        if (regex.check(line)) {
            if (matrices.containsKey(current)) {
                const SparseMatrix<int> &matrix = matrices.get(current);
                output(std::cout, matrix);
                return true;
            }
        }
        return false;
    }

    bool switch_name(const std::string &line) {
        RegEx regex("^ *switch +([^ ]+) *$");
        if (regex.check(line)) {
            auto name = std::string(regex.split(line)[1]);
            if (matrices.containsKey(name)) {
                current = name;
                return true;
            }
        }
        return false;
    }

    bool set(const std::string &line) {
        if (current.empty()) return false;
        RegEx regex("^ *set +([0-9]+) +([0-9]+) +(-?[0-9]+) *$");
        if (regex.check(line)) {
            auto groups = regex.split(line);
            unsigned long row, col;
            int value;
            auto result1 = std::from_chars(groups[1].data(), groups[1].data() + groups[1].size(), col);
            auto result2 = std::from_chars(groups[2].data(), groups[2].data() + groups[2].size(), row);
            auto result3 = std::from_chars(groups[3].data(), groups[3].data() + groups[3].size(), value);
            if (result1.ec == std::errc::invalid_argument || result2.ec == std::errc::invalid_argument ||
                result3.ec == std::errc::invalid_argument) {
                return false;
            }
            matrices.get(current).set(col, row, value);
            return true;
        }
        return false;
    }

    bool add(const std::string &line) {
        if (current.empty()) return false;
        RegEx regex("^ *add +([^ ]+) *$");
        if (regex.check(line)) {
            auto name = std::string(regex.split(line)[1]);
            if (matrices.containsKey(name)) {
                try {
                    matrices.add(current, matrices.get(current) + matrices.get(name));
                } catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                    return false;
                }
                return true;
            }
        }
        return false;
    }

    bool multiply(const std::string &line) {
        if (current.empty()) return false;
        RegEx regex("^ *mul +([^ ]+) *$");
        if (regex.check(line)) {
            auto name = std::string(regex.split(line)[1]);
            if (matrices.containsKey(name)) {
                try {
                    matrices.add(current, matrices.get(current) * matrices.get(name));
                } catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                    return false;
                }
                return true;
            }
        }
        return false;
    }
};

class stopException : public std::exception {

};

void execute(Manager &manager, const std::string &line) {
    if (line.empty()) return;
    if (auto firstWord = getFirstWord(line); firstWord.empty()) {
        return;
    } else if (firstWord == "exit") {
        std::cout << "Bye" << std::endl;
        throw stopException();
    } else if (firstWord == "create") {
        // создать матрицу
        if (!manager.create(line)) {
            std::cout << "bad command 'create'" << std::endl;
        }
    } else if (firstWord == "show") {
        // отобразить матрицу
        if (!manager.show(line)) {
            std::cout << "bad command 'show'" << std::endl;
        }
    } else if (firstWord == "switch") {
        // переключиться на матрицу
        if (!manager.switch_name(line)) {
            std::cout << "bad command 'switch'" << std::endl;
        }
    } else if (firstWord == "set") {
        // поменять значения в матрице
        if (!manager.set(line)) {
            std::cout << "bad command 'set'" << std::endl;
        }
    } else if (firstWord == "add") {
        if (!manager.add(line)) {
            std::cout << "bad command 'add'" << std::endl;
        }
    } else if (firstWord == "mul") {
        if (!manager.multiply(line)) {
            std::cout << "bad command 'mul'" << std::endl;
        }
    } else if (firstWord == "name") {
        std::cout << manager.getCurrentName() << std::endl;
    } else {
        std::cout << "bad command" << "'" << firstWord << "'" << std::endl;
    }
}

int main() {
    Manager manager;
    std::string line;
    while (std::cout << ">> " && getline(std::cin, line)) {
        try {
            execute(manager, line);
        } catch (stopException &) {
            break;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
