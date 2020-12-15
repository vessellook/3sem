#pragma once

#include "linked_list_sequence.h"
#include "queue.h"

namespace sem3 {
    template<typename T>
    class ListQueue : public IQueue<T> {
        sem2::LinkedListSequence<T> container;
    public:
        void enqueue(T element) { container.append(std::move(element)); }

        virtual void dequeue() { container.remove(0); }

        virtual T &front() { return container.getFirst(); }

        virtual T &back() { return container.getLast(); }

        virtual unsigned size() { return container.getLength(); }
    };

}
