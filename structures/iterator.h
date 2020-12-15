#pragma once

#include "pair.h"

namespace sem3 {

    template<typename TElement>
    class IIterator {
    public:
        virtual const TElement &getCurrentItem() = 0;

        [[nodiscard]] virtual bool hasNext() const = 0;

        virtual bool next() = 0;

        virtual Pair<bool, const TElement *> tryGetCurrentItem() = 0;

        virtual ~IIterator() = default;
    };

}
