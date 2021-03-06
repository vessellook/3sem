#pragma once

#include <queue>
#include <utility>
#include <string>
#include <stdexcept>
#include "../utils.h"

namespace sem3 {

    class SortingControl {
        static unsigned counter;
        const unsigned id;
        unsigned elementCounter = 0;
    public:
        struct Report {
            const std::string type;
            const unsigned id1;
            const unsigned id2;
        };

        struct SortingElement {
            unsigned id;
            int value;
            SortingControl *parent;

            operator int() { return value; }

            operator int() const { return value; }

            friend void swap(SortingElement &lhs, SortingElement &rhs) {
                if (lhs.parent != rhs.parent) throw std::runtime_error("Elements of different SortingControl objects");
                lhs.parent->sendReport("swap", lhs.id, rhs.id);
            }
        };

    public:
        static std::pair<SortingElement *, SortingControl>
        getSortingControl(int *array, unsigned count) {
            SortingControl sc;
            return {sc.getElements(array, count), std::move(sc)};
        }

        std::queue<Report> extractReports() { return move(reports); }

        bool operator==(const SortingControl &other) const {
            return id == other.id;
        }

        void sendReport(std::string type, unsigned id1, unsigned id2);

    private:
        SortingControl() : id(counter++) {}

        SortingElement *getElements(const int *array, unsigned count);

    private:
        std::queue<Report> reports;
    };

    bool operator<(const SortingControl::SortingElement &lhs,
                   const SortingControl::SortingElement &rhs);
}
