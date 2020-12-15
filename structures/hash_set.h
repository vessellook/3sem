#pragma once

#include <utility>
#include <functional>
#include <stdexcept>
#include <iostream>

#include "array_sequence.h"
#include "dictionary.h"
#include "hash_table.h"

namespace sem3 {

    template<typename TElement>
    class HashSet {
        HashTable<TElement> table;
    public:
        explicit HashSet(std::function<unsigned(TElement)> hash = std::hash<TElement>()) : table(8, hash) {}

        using Iterator = typename HashTable<TElement>::Iterator;

        std::unique_ptr<IIterator<TElement>>
        GetIterator() const { return table.getIterator(); }

        void Remove(const TElement &element) {
            if (!table.remove(element)) {
                throw std::runtime_error("no such key in HashSet");
            }
            if (table.getCapacity() > 2 * table.getCount()) {
                table.resizeLess();
            }
        }

        void Add(TElement element) {
            table.add(element);
            if (table.getCapacity() < table.getCount()) {
                table.resizeMore();
            }
        }

        bool Has(const TElement &element) const {
            return table.contains(element);
        }

        [[nodiscard]] unsigned GetCount() const {
            return table.getCount();
        }


        [[nodiscard]] unsigned GetCapacity() const {
            return table.getCapacity();
        }
    };
}
