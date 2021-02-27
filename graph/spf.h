#pragma once

#include <functional>

#include "weighted_edge.h"
#include "../structures/hash_map.h"
#include "../structures/hash_set.h"
#include "../structures/ss_priority_queue.h"

namespace sem3 {
    // Djisktra's Shortest Path First algorithm
    template<typename Vertex, typename WeightedEdge>
    class SPF {
        static_assert(is_weighted<WeightedEdge>::value,
                      "template parameter WeightedEdge should be the derived from the template class WeightedEdge");

    public:
        using Graph = DirectedGraph<Vertex, WeightedEdge>;
        using Weight = typename WeightedEdge::Weight;

        std::function<Weight(Weight, Weight)> sum = [](Weight a, Weight b) { return a + b; };


        void setSum(std::function<Weight(Weight, Weight)> sum) {
            this->sum = sum;
        }

    public:
        struct Result {
            HashSet<Vertex> visitedVertices;
            HashMap<Vertex , Vertex> vertexToPreviousVertexInPath;
            HashMap<Vertex , Weight> vertexToMinPathWeight;
        };

        // requirement: Weight() == 0 (default constructor creates zero element)
        // requirement: no negative weights in the graph
        // requirement: sum is a commutative operation
        Result run(const Graph &graph, Vertex startVertex) {
            HashSet<Vertex > visitedVertices;
            HashMap<Vertex, Vertex > vertexToPreviousVertexInPath;
            HashMap<Vertex, Weight> vertexToMinPathWeight;
            vertexToMinPathWeight.add(startVertex, Weight());
            SSPriorityQueue<Vertex > queue;
            auto enqueue = [&queue, &vertexToMinPathWeight](Vertex vertex) {
                queue.enqueue(vertex, vertexToMinPathWeight.get(vertex));
            };
            auto updateQueue = [&queue, &vertexToMinPathWeight](Vertex vertex) {
                queue.changePriority(vertex, vertexToMinPathWeight.get(vertex));
            };
            enqueue(startVertex);

            while (!queue.empty()) {
                auto currentVertex = queue.dequeue();
                auto edges = graph.getOutgoingEdges(currentVertex);
                for (auto edgeIt = edges->getIterator(); edgeIt->next();) {
                    auto edge = edgeIt->getCurrentItem();
                    auto neighbourVertex = graph.getTail(edge);
                    auto newWeight = sum(edge.getWeight(), vertexToMinPathWeight.get(currentVertex));
                    if (!vertexToMinPathWeight.containsKey(neighbourVertex)) {
                        vertexToMinPathWeight.add(neighbourVertex, newWeight);
                        vertexToPreviousVertexInPath.add(neighbourVertex, currentVertex);
                        enqueue(neighbourVertex);
                    } else if (vertexToMinPathWeight.get(neighbourVertex) > newWeight) {
                        vertexToMinPathWeight.add(neighbourVertex, newWeight);
                        vertexToPreviousVertexInPath.add(neighbourVertex, currentVertex);
                        updateQueue(neighbourVertex);
                    }
                }
                visitedVertices.add(std::move(currentVertex));
            }
            return Result{std::move(visitedVertices), std::move(vertexToPreviousVertexInPath),
                          std::move(vertexToMinPathWeight)};
        }
    };
}
