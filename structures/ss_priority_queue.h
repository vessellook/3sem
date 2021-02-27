#pragma once

#include <climits>

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

            bool operator==(const PriorityElement &other) const {
                return order == other.order;
            }

            bool operator<(const PriorityElement &other) const {
                return priority < other.priority || priority == other.priority && order > other.order;
            }

            bool operator<=(const PriorityElement &other) const {
                return *this < other || *this == other;
            }
        };

        SortedSequence<PriorityElement> container = SortedSequence<PriorityElement>();
        unsigned counter = 0;
    public:
        unsigned size() override {
            return container.getLength();
        }

        const TElement &peekElement() override {
            return container.getLast().data;
        }

        unsigned peekPriority() override {
            return container.getLast().priority;
        }

        void enqueue(TElement element, unsigned priority) override {
            container.add(PriorityElement{priority, counter++, element});
        }

        void changePriority(const TElement &element, unsigned newPriority) override {
            auto index = 0u;
            auto order = UINT_MAX;
            for (auto it = container.getIterator(); it->next();) {
                if (it->getCurrentItem().data == element) {
                    if (it->getCurrentItem().priority == newPriority) {
                        return; // do nothing
                    }
                    order = it->getCurrentItem().order;
                    break;
                }
                index++;
            }
            if (order == UINT_MAX) { // if such element is not found
                container.add(PriorityElement{newPriority, counter++, element});
                return;
            } else {
                container.remove(index);
                container.add(PriorityElement{newPriority, order, element}); // order of elements is saved
            }
        }

        TElement dequeue() override {
            return std::move(container.remove(container.getLength() - 1).data);
        }
    };
}