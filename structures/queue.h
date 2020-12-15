#pragma once

namespace sem3 {
    template<typename TElement>
    class IQueue {
    public:
        virtual void enqueue(TElement element) = 0;

        virtual void dequeue() = 0;

        virtual TElement &front() = 0;

        virtual TElement &back() = 0;

        virtual unsigned size() = 0;

        bool empty() { return size() == 0; }
    };
}