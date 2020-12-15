#pragma once

#include <utility>
#include <memory>

#include "iterator.h"
#include "sequence.h"
#include "pair.h"

namespace sem3 {
    template<typename TKey, typename TElement>
    class IDictionary {
    public:
        virtual std::unique_ptr<IIterator <Pair<TKey, TElement>>> getIterator() const = 0;

        virtual void add(const TKey &key, TElement value) = 0;

        virtual void remove(const TKey &key) = 0;

        virtual const TElement &get(const TKey &key) const = 0;

        virtual TElement &get(const TKey &key) = 0;

        virtual bool containsKey(const TKey &key) const = 0;

        [[nodiscard]] virtual unsigned getCount() const = 0;

        [[nodiscard]] virtual unsigned getCapacity() const = 0;

        virtual TElement &operator[](const TKey &key) = 0;

        virtual const TElement &operator[](const TKey &key) const = 0;

        virtual ~IDictionary() = default;
    };
}
