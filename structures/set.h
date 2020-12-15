#pragma once

#include <utility>
#include <memory>

#include "iterator.h"
#include "sequence.h"
#include "pair.h"

namespace sem3 {
    template<typename TElement>
    class ISet {
    public:
        virtual std::unique_ptr<IIterator<TElement>> getIterator() const = 0;

        virtual void add(TElement value) = 0;

        virtual void remove(const TElement &element) = 0;

        virtual bool contains(const TElement &element) const = 0;

        [[nodiscard]] virtual unsigned getCount() const = 0;

        [[nodiscard]] virtual unsigned getCapacity() const = 0;

        virtual ~ISet() = default;
    };
}
