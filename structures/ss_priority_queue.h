#pragma once

#include "priority_queue.h"
#include "sorted_sequence.h"
#include "pair.h"

namespace sem3 {
    template<typename TElement>
    class SSPriorityQueue : public IPriorityQueue<TElement> {
        struct PriorityElement {
            unsigned priority;
            unsigned order;
            TElement data;

            bool operator==(const PriorityElement &other) {
                return order == other.order;
            }

            bool operator<(const PriorityElement &other) {
                return priority < other.priority || priority == other.priority && order > other.order;
            }
        };

        SortedSequence<PriorityElement> container = SortedSequence<PriorityElement>();
        unsigned counter = 0;
    public:
        unsigned size() override {
            return container.getLength();
        }

        TElement &peekElement() override {
            return container.getLast().data;
        }

        unsigned peekPriority() override {
            return container.getLast().priority;
        }

        void enqueue(TElement element, unsigned priority) override {
            container.add(PriorityElement{priority, counter++, element});
        }

        void dequeue() override {
            container.remove(container.getLength() - 1);
        }
    };
}