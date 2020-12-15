#pragma once

#include <iostream>
#include "../structures/array_sequence.h"

namespace sem3 {
    class TablePrinter {

    public:
        explicit TablePrinter(unsigned width) : colCount(width), colToLength(width) {}

        [[nodiscard]] unsigned getRowsCount() const { return rows.getLength(); }

        [[nodiscard]] unsigned getColsCount() const { return colCount; }

        const TablePrinter &addRow(sem2::ArraySequence<std::string> row);

        const TablePrinter &addEmptyRow();

        /*
         * return string representation of table
         */
        void print(std::ostream &os) const;

    private:
        void getRowString(std::ostream &os, unsigned index) const;

        void getSeparatorString(std::ostream &os) const;

    private:
        unsigned colCount;
        sem2::ArraySequence<sem2::ArraySequence<std::string>> rows;
        sem2::ArraySequence<unsigned> colToLength;
    };

}
