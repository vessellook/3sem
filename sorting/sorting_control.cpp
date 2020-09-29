#include "sorting_control.h"

using sem3::SortingControl;
using SortingElement = SortingControl::SortingElement;

bool operator<(const SortingControl::SortingElement &lhs,
               const SortingControl::SortingElement &rhs) {
    if (lhs.parent == rhs.parent) {
        lhs.parent->sendReport("compare", lhs.id, rhs.id);
    }
    return lhs.value < rhs.value;
}

SortingElement *
SortingControl::getElements(const int *array, unsigned count) {
    auto *result = new SortingElement[count];
    for (unsigned i = 0; i < count; i++) {
        result[i].id = elementCounter++;
        result[i].value = array[i];
        result[i].parent = this;
    }
    return result;
}

void SortingControl::sendReport(std::string type, unsigned id1, unsigned id2) {
    reports.push({move(type), id1, id2});
}

