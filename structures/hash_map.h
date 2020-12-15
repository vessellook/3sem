#pragma once

#include <utility>
#include <functional>
#include <stdexcept>
#include <climits>
#include <memory>
#include <iostream>

#include "array_sequence.h"
#include "dictionary.h"
#include "hash_table.h"

namespace sem3 {

    template<typename TKey, typename TElement>
    class HashMap : public IDictionary<TKey, TElement> {
        using TPair = Pair<TKey, TElement>;
        HashTable<TPair> table;
        std::function<unsigned(TKey)> hash;

        class Hash {
            std::function<unsigned(TKey)> hash;
        public:
            explicit Hash(std::function<unsigned(TKey)> hash) : hash(hash) {}

            unsigned operator()(TPair pair) {
                return hash(pair.first);
            }
        };

        class IsMatch {
            TKey key;
        public:
            explicit IsMatch(TKey key) : key(key) {}

            bool operator()(const TPair &pair) {
                return key == pair.first;
            }
        };

    public:
        explicit HashMap(std::function<unsigned(TKey)> hash = std::hash<TKey>()) : table(8, Hash(hash)), hash(hash) {}

        using Iterator = typename HashTable<TPair>::Iterator;

        std::unique_ptr<IIterator<TPair>>
        getIterator() const override { return table.getIterator(); }

//
        void remove(const TKey &key) override {
            if (!table.remove(hash(key), IsMatch(key))) {
                throw std::runtime_error("no such key in HashMap");
            }
            if (table.getCapacity() > 2 * table.getCount()) {
                table.resizeLess();
            }
        }

        void add(const TKey &key, TElement value) override {
            table.add(TPair{key, value}, IsMatch(key));
            if (table.getCapacity() < table.getCount()) {
                table.resizeMore();
            }
        }

        const TElement &get(const TKey &key) const override {
            return table.get(hash(key), IsMatch(key)).second;
        }

        TElement &get(const TKey &key) override {
            return table.get(hash(key), IsMatch(key)).second;
        }

        TElement &operator[](const TKey &key) override {
            return get(key);
        }

        const TElement &operator[](const TKey &key) const override {
            return get(key);
        }

        bool containsKey(const TKey &key) const override {
            return table.contains(hash(key), IsMatch(key));
        }

        [[nodiscard]] unsigned getCount() const override {
            return table.getCount();
        }

        [[nodiscard]] unsigned getCapacity() const override {
            return table.getCapacity();
        }
    };
}
