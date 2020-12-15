#include <string>
#include <sstream>

#include "table_printer.h"

using namespace sem3;

namespace Chars {
    std::string vertical() { return "|"; }

    std::string horizontal() { return "-"; }

    std::string getCornerTL() { return "┌"; }

    std::string getCornerBL() { return "└"; }

    std::string getCornerTR() { return "┐"; }

    std::string getCornerBR() { return "┘"; }

    std::string getCross() { return "+"; }
}

const TablePrinter &TablePrinter::addRow(sem2::ArraySequence<std::string> row) {
    if (row.getLength() != colCount) {
        std::ostringstream oss;
        throw std::runtime_error(oss.str());
    }
    for (unsigned i = 0; i < colCount; i++) {
        unsigned t = row.get(i).size();
        if (t > colToLength.get(i)) {
            colToLength.set(i, t);
        }
    }
    rows.append(std::move(row));
    return *this;
}

std::string align_center(const std::string &text, unsigned width) {
    unsigned left = (width - text.size()) / 2;
    unsigned right = width - text.size() - left;
    return std::string(left, ' ') + text + std::string(right, ' ');
}

void TablePrinter::getRowString(std::ostream &os, unsigned index) const {
    const sem2::ArraySequence<std::string> &row = rows.get(index);
    os << Chars::vertical();
    for (unsigned i = 0; i < row.getLength(); i++) {
        os << align_center(row.get(i), colToLength.get(i)) << Chars::vertical();
    }
}

void TablePrinter::getSeparatorString(std::ostream &os) const {
    os << Chars::vertical();
    for (unsigned i = 0; i < colCount; i++) {
        for (unsigned j = 0, len = colToLength.get(i); j < len; j++) {
            os << Chars::horizontal();
        }
        os << Chars::vertical();
    }
}

void TablePrinter::print(std::ostream &os) const {
    if (rows.getLength() > 0) {
        getSeparatorString(os);
        os << '\n';
        getRowString(os, 0);
        os << '\n';
        getSeparatorString(os);
        os << '\n';
    }
    for (unsigned i = 1; i < rows.getLength(); i++) {
        getRowString(os, i);
        os << '\n';
        getSeparatorString(os);
        os << '\n';
    }
}

const TablePrinter &TablePrinter::addEmptyRow() {
    auto row = sem2::ArraySequence<std::string>();
    for (unsigned i = 0; i < colCount; i++) {
        row.append("");
    }
    addRow(std::move(row));
    return *this;
}
