#pragma once

#include <climits>
#include <memory>

#include "weighted_edge.h"
#include "../structures/sparse_matrix.h"

namespace sem3 {

    // requirement: sem3::infinity<Edge>() exist
    // requirement: edge == sem3::infinity<Edge>() only if edge does not exist
    // requirement: std::hash<Edge> and std::hash<Vertex> exist
    template<typename Vertex, typename Edge>
    class AdjacencyMatrixGraph : public DirectedGraph<Vertex, Edge> {
        HashMap<Vertex, unsigned> vertexToIndex;
        HashMap<unsigned, Vertex> indexToVertex;
        HashMap<Edge, Pair<Vertex, Vertex>> edgeToVertices;
        SparseMatrix<Edge> fromToMatrix;
        SparseMatrix<Edge> toFromMatrix;
        Edge defaultValue = infinity<Edge>();
        unsigned counter = 0;

        bool isInfinity(Edge edge) const {
            return edge == defaultValue;
        }

    public:
        [[nodiscard]] bool
        hasEdge(Vertex fromVertex, Vertex toVertex) const override {
            return !isInfinity(fromToMatrix.get(vertexToIndex[toVertex], vertexToIndex[fromVertex]));
        }

        [[nodiscard]] Edge
        getEdge(Vertex fromVertex, Vertex toVertex) const override {
            return fromToMatrix.get(vertexToIndex[toVertex], vertexToIndex[fromVertex]);
        }

        [[nodiscard]] std::shared_ptr<sem2::ISequence<Vertex>> getVertices() const override {
            auto result = std::make_shared<sem2::ArraySequence<Vertex>>();
            auto it = vertexToIndex.getIterator();
            while (it->next()) {
                result->append(it->getCurrentItem().first);
            }
            return result;
        }

        void addVertex(Vertex vertex) override {
            auto index = counter++;
            vertexToIndex.add(vertex, index);
            indexToVertex.add(index, vertex);
        }

        void addEdge(Edge edge,
                     Vertex fromVertex,
                     Vertex toVertex) override {
            fromToMatrix.set(vertexToIndex[toVertex], vertexToIndex[fromVertex], edge);
            toFromMatrix.set(vertexToIndex[fromVertex], vertexToIndex[toVertex], edge);
            edgeToVertices.add(edge, Pair{fromVertex, toVertex});
        }

        void removeEdge(Edge edge) override {
            if (isInfinity(edge)) return;
            if (edgeToVertices.containsKey(edge)) {
                auto pair = edgeToVertices.get(edge);
                auto from = vertexToIndex[pair.first], to = vertexToIndex[pair.second];
                fromToMatrix.set(from, to, defaultValue);
                toFromMatrix.set(to, from, defaultValue);
                edgeToVertices.remove(edge);
            }
            throw std::runtime_error("no such edge in AdjacencyMatrixGraph");
        }

        [[nodiscard]]  Vertex getHead(Edge edge) const override { return edgeToVertices[edge].first; }

        [[nodiscard]]  Vertex getTail(Edge edge) const override { return edgeToVertices[edge].second; }

        [[nodiscard]]  std::shared_ptr<sem2::ISequence<Edge>>
        getOutgoingEdges(Vertex vertex) const override {
            auto result = std::make_shared<sem2::ArraySequence<Edge>>();
            auto id = vertexToIndex[vertex];
            if (fromToMatrix.getNotEmptyRows().indexOf(id) == -1) {
                return result;
            }
            auto &neighbours = fromToMatrix.getColsByRow(id);
            for (auto it = neighbours.getIterator(); it->next();) {
                auto neighbour = it->getCurrentItem();
                auto edge = getEdge(vertex, indexToVertex[neighbour]);
                if (!isInfinity(edge)) {
                    result->append(edge);
                }
            }
            return result;
        }

        [[nodiscard]]  std::shared_ptr<sem2::ISequence<Edge>>
        getIngoingEdges(Vertex vertex) const override {
            auto result = std::make_shared<sem2::ArraySequence<Edge>>();
            auto id = vertexToIndex[vertex];
            if (toFromMatrix.getNotEmptyRows().indexOf(id) == -1) {
                return result;
            }
            auto &neighbours = toFromMatrix.getColsByRow(id);
            for (auto it = neighbours.getIterator(); it->next();) {
                auto neighbour = it->getCurrentItem();
                auto edge = getEdge(indexToVertex[neighbour], vertex);
                if (!isInfinity(edge)) {
                    result->append(edge);
                }
            }
            return result;
        }


        [[nodiscard]] virtual unsigned getPositiveDegree(Vertex vertex) const override {
            auto id = vertexToIndex[vertex];
            if (fromToMatrix.getNotEmptyRows().indexOf(id) == -1) {
                return 0;
            }
            auto result = 0u;
            auto &neighbours = fromToMatrix.getColsByRow(id);
            for (auto it = neighbours.getIterator(); it->next();) {
                auto neighbour = it->getCurrentItem();
                auto edge = getEdge(vertex, indexToVertex[neighbour]);
                if (!isInfinity(edge)) {
                    result++;
                }
            }
            return result;
        }

        [[nodiscard]] virtual unsigned getNegativeDegree(Vertex vertex) const override {
            auto id = vertexToIndex[vertex];
            if (toFromMatrix.getNotEmptyRows().indexOf(id) == -1) {
                return 0;
            }
            auto result = 0u;
            auto &neighbours = toFromMatrix.getColsByRow(id);
            for (auto it = neighbours.getIterator(); it->next();) {
                auto neighbour = it->getCurrentItem();
                auto edge = getEdge(indexToVertex[neighbour], vertex);
                if (!isInfinity(edge)) {
                    result++;
                }
            }
            return result;
        }


        AdjacencyMatrixGraph() : fromToMatrix(UINT_MAX, UINT_MAX), toFromMatrix(UINT_MAX, UINT_MAX) {
            fromToMatrix.setDefault(defaultValue);
            toFromMatrix.setDefault(defaultValue);
        }
    };
}