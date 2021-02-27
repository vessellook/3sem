#pragma once

#include <functional>

#include "weighted_edge.h"
#include "../structures/hash_map.h"
#include "../structures/hash_set.h"
#include "../structures/list_queue.h"

namespace sem3 {
    template<typename Vertex, typename WeightedEdge>
    class BFS {
        static_assert(is_weighted<WeightedEdge>::value,
                      "template parameter WeightedEdge should be the derived from the template class WeightedEdge");

    public:
        using Graph = DirectedGraph<Vertex, WeightedEdge>;
        using Weight = typename WeightedEdge::Weight;

    private:

    private:
        static bool stub1(Vertex, Vertex) { return true; }

        static void stub2(Vertex, Vertex) {}

        static bool stub3() { return false; }

        std::function<bool(Vertex, Vertex)> onNewVertex = stub1;
        std::function<void(Vertex, Vertex)> onVisitedVertex = stub2;
        std::function<bool()> shouldStopSearch = stub3;

    public:
        void addNewVertexCallback(std::function<bool(Vertex, Vertex)> callback) { onNewVertex = callback; }

        void addVisitedVertexCallback(std::function<void(Vertex, Vertex)> callback) { onVisitedVertex = callback; }

        void run(const DirectedGraph<Vertex, WeightedEdge> &g, Vertex startVertex) {
            sem3::HashSet<Vertex> markedVertices;
            markedVertices.add(startVertex);
            ListQueue<Vertex> queue;
            queue.enqueue(startVertex);

            while (!queue.empty()) {
                auto current = queue.front();
                auto edges = g.getOutgoingEdges(current);
                auto edgeIt = edges->getIterator();
                while (edgeIt->next()) {
                    auto edge = edgeIt->getCurrentItem();
                    auto vertex = g.getTail(edge);
                    if (!markedVertices.has(vertex)) {
                        if (onNewVertex(current, vertex)) {
                            markedVertices.add(vertex);
                            queue.enqueue(vertex);
                        }
                    } else {
                        onVisitedVertex(current, vertex);
                    }
                }
                queue.dequeue();
                if (shouldStopSearch()) return;
            }
        }

        // requirement: Weight() == 0
        // requirement: hasNegativeWeights = true if some weights can be negative in the graph
        // requirement: no negative cycles. The result is invalid if the graph has negative cycle
        // requirement: sum is a commutative operation
        static bool
        trySearchShortestPath(const Graph &graph, Vertex startVertex, Vertex finishVertex,
                              sem2::ISequence<Vertex> &resultReversePath, bool hasNegativeWeights = false,
                              std::function<Weight(Weight, Weight)> sum = [](Weight a, Weight b) { return a + b; }) {
            BFS bfs;
            HashMap<Vertex, Vertex> vertexToPreviousVertex;
            HashMap<Vertex, Weight> vertexToMinPathWeight;
            Weight finishPathWeight;
            vertexToMinPathWeight.add(startVertex, Weight());

            bfs.addNewVertexCallback(
                    [&](Vertex fromVertex, Vertex toVertex) {
                        auto edge = graph.getEdge(fromVertex, toVertex);
                        auto edgeWeight = edge.getWeight();
                        auto fromPathWeight = vertexToMinPathWeight.get(fromVertex);
                        vertexToMinPathWeight.add(toVertex, sum(edgeWeight, fromPathWeight));
                        vertexToPreviousVertex.add(toVertex, fromVertex);
                        if (toVertex == finishVertex) {
                            finishPathWeight = vertexToMinPathWeight.get(toVertex);
                        }
                        if (!hasNegativeWeights && vertexToPreviousVertex.containsKey(finishVertex) &&
                            vertexToMinPathWeight.get(toVertex) >= finishPathWeight) {
                            return false;
                        }
                        return true;
                    }
            );

            bfs.addVisitedVertexCallback(
                    [&](Vertex fromVertex, Vertex toVertex) {
                        auto edge = graph.getEdge(fromVertex, toVertex);
                        auto edgeWeight = edge.getWeight();
                        auto fromPathWeight = vertexToMinPathWeight.get(fromVertex);
                        auto &toPathWeight = vertexToMinPathWeight.get(toVertex);
                        if (sum(edgeWeight, fromPathWeight) < toPathWeight) {
                            toPathWeight = sum(edgeWeight, fromPathWeight);
                            vertexToPreviousVertex.add(toVertex, fromVertex);
                            if (toVertex == finishVertex) {
                                finishPathWeight = toPathWeight;
                            }
                        }
                    }
            );

            bfs.run(graph, startVertex);

            if (!vertexToPreviousVertex.containsKey(finishVertex)) {
                return false;
            }
            resultReversePath.append(finishVertex);
            for (auto vertex = finishVertex; vertex != startVertex;) {
                vertex = vertexToPreviousVertex.get(vertex);
                resultReversePath.append(vertex);
            }
            return true;
        }

        // requirement: Weight() == 0
        // requirement: no negative weights in the graph
        // requirement: sum is a commutative operation
        static bool
        trySearchWeightiestPath(const Graph &graph, Vertex startVertex, Vertex finishVertex,
                                sem2::ISequence<Vertex> &resultReversePath,
                                std::function<Weight(Weight, Weight)> sum = [](Weight a, Weight b) { return a + b; }) {
            BFS bfs;
            HashMap<Vertex, Vertex> vertexToPreviousVertex;
            HashMap<Vertex, Weight> vertexToMaxPathWeight;
            vertexToMaxPathWeight.add(startVertex, Weight());

            bfs.addNewVertexCallback(
                    [&](Vertex fromVertex, Vertex toVertex) {
                        auto edge = graph.getEdge(fromVertex, toVertex);
                        auto edgeWeight = edge.getWeight();
                        auto fromPathWeight = vertexToMaxPathWeight.get(fromVertex);
                        vertexToMaxPathWeight.add(toVertex, sum(edgeWeight, fromPathWeight));
                        vertexToPreviousVertex.add(toVertex, fromVertex);
                        return true;
                    }
            );

            bfs.addVisitedVertexCallback(
                    [&](Vertex fromVertex, Vertex toVertex) {
                        auto edge = graph.getEdge(fromVertex, toVertex);
                        auto edgeWeight = edge.getWeight();
                        auto fromPathWeight = vertexToMaxPathWeight.get(fromVertex);
                        auto &toPathWeight = vertexToMaxPathWeight.get(toVertex);
                        if (toPathWeight < sum(edgeWeight, fromPathWeight)) {
                            toPathWeight = sum(edgeWeight, fromPathWeight);
                            vertexToPreviousVertex.add(toVertex, fromVertex);
                        }
                    }
            );

            bfs.run(graph, startVertex);

            if (!vertexToPreviousVertex.containsKey(finishVertex)) {
                return false;
            }
            resultReversePath.append(finishVertex);
            for (auto vertex = finishVertex; vertex != startVertex;) {
                vertex = vertexToPreviousVertex.get(vertex);
                resultReversePath.append(vertex);
            }
            return true;
        }

        // requirement: Weight() == 0
        // requirement: no negative weights in the graph
        // requirement: sum is a commutative operation
        static sem2::ArraySequence<Vertex>
        searchTopologicalOrder(const Graph &graph,
                               std::function<Weight(Weight, Weight)> sum = [](Weight a, Weight b) { return a + b; }) {
            sem2::ArraySequence<Vertex> maxVertices;
            auto vertices = graph.getVertices();
            HashMap<Vertex, unsigned> vertexToLevel;
            for (auto it = vertices->getIterator(); it->next();) {
                auto &vertex = it->getCurrentItem();
                if (graph.getNegativeDegree(vertex) == 0) {
                    maxVertices.append(vertex);
                    vertexToLevel.add(vertex, 0u);
                }
            }
            BFS bfs;

            bfs.addNewVertexCallback(
                    [&](Vertex fromVertex, Vertex toVertex) {
                        if (auto newLevel = vertexToLevel.get(fromVertex) + 1;
                                !vertexToLevel.containsKey(toVertex) || newLevel < vertexToLevel.get(toVertex)) {
                            vertexToLevel.add(toVertex, newLevel);
                        }
                        return true;
                    }
            );

            bfs.addVisitedVertexCallback(
                    [&](Vertex fromVertex, Vertex toVertex) {
                        if (auto newLevel = vertexToLevel.get(fromVertex) + 1;
                                newLevel < vertexToLevel.get(toVertex)) {
                            vertexToLevel.add(toVertex, newLevel);
                        }
                        return true;
                    }
            );

            for (auto it = maxVertices.getIterator(); it->next();) {
                bfs.run(graph, it->getCurrentItem());
            }

            HashMap<unsigned, sem2::ArraySequence<Vertex>> levelToVertices;
            for (auto it = vertexToLevel.getIterator(); it->next();) {
                auto vertex = it->getCurrentItem().first;
                auto level = it->getCurrentItem().second;
                if (!levelToVertices.containsKey(level)) {
                    levelToVertices.add(level, sem2::ArraySequence<Vertex>());
                }
                levelToVertices.get(level).append(vertex);
            }
            sem2::ArraySequence<Vertex> result;
            auto maxLevel = levelToVertices.getCount() - 1;
            for (auto level = 0u; level <= maxLevel; level++) {
                auto &seq = levelToVertices[level];
                result.concat(seq);
            }
            return std::move(result);
        }
    };
}
