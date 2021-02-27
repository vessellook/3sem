#pragma once

namespace sem3 {
    template<typename TElement>
    class IPriorityQueue {
    public:
        virtual unsigned size() = 0;

        virtual const TElement &peekElement() = 0;

        virtual unsigned peekPriority() = 0;

        virtual void enqueue(TElement element, unsigned priority) = 0;

        virtual TElement dequeue() = 0;

        virtual void changePriority(const TElement &element, unsigned newPriority) = 0;

        bool empty() { return size() == 0; }
    };
}