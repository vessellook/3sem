#pragma once

#include <memory>

#include "../structures/pair.h"
#include "../structures/sequence.h"

namespace sem3 {

    template<typename Vertex, typename Edge>
    class Graph {
    public:
        [[nodiscard]] virtual bool
        hasEdge(Vertex fromVertex, Vertex toVertex) const = 0;

        [[nodiscard]] virtual Edge
        getEdge(Vertex fromVertex, Vertex toVertex) const = 0;

        [[nodiscard]] virtual std::shared_ptr<sem2::ISequence<Vertex>> getVertices() const = 0;

        virtual void addVertex(Vertex vertex) = 0;

        virtual void addEdge(Edge edge,
                             Vertex fromVertex,
                             Vertex toVertex) = 0;

        virtual void removeEdge(Edge edge) = 0;

        virtual ~Graph() = default;
    };

    template<typename Edge>
    Edge infinity();

    template<>
    int infinity() { return INT_MAX; }

    template<>
    unsigned infinity() { return UINT_MAX; }


    template<typename Vertex, typename Edge>
    class DirectedGraph : public Graph<Vertex, Edge> {
    public:
        [[nodiscard]] virtual Vertex getHead(Edge edge) const = 0;

        [[nodiscard]] virtual Vertex getTail(Edge edge) const = 0;

        [[nodiscard]] virtual std::shared_ptr<sem2::ISequence<Edge>>
        getOutgoingEdges(Vertex vertex) const = 0;

        [[nodiscard]] virtual std::shared_ptr<sem2::ISequence<Edge>>
        getIngoingEdges(Vertex vertex) const = 0;

        [[nodiscard]] virtual unsigned getPositiveDegree(Vertex vertex) const = 0;

        [[nodiscard]] virtual unsigned getNegativeDegree(Vertex vertex) const = 0;
    };

    template<typename Vertex, typename Edge, typename Directed>
    class UndirectedGraph : public Graph<Vertex, Edge> {
        Directed graph;
        static_assert(std::is_base_of_v<Directed, DirectedGraph<Vertex, Edge>>,
                      "DirectedGraph should be the derived of Graph<Vertex, Edge>");
    public:
        Pair<Vertex, Vertex> getTails(Edge edge) {
            return Pair{graph.getHead(edge), graph.getTail(edge)};
        }

        [[nodiscard]] std::shared_ptr<sem2::ISequence<Edge>>
        getEdges(Vertex vertex) const {
            auto seq1 = graph.getOutgoingEdges(vertex);
            auto seq2 = graph.getIngoingEdges(vertex);
            seq1.concat(seq2);
            return seq1;
        }

        [[nodiscard]] virtual unsigned getDegree(Vertex vertex) const {
            return graph.getPositiveDegree() + graph.getNegativeDegree();
        }

        [[nodiscard]] bool
        hasEdge(Vertex fromVertex, Vertex toVertex) const override {
            return graph.hasEdge(fromVertex, toVertex) || graph.hasEdge(toVertex, fromVertex);
        }

        [[nodiscard]] Edge
        getEdge(Vertex fromVertex, Vertex toVertex) const override {
            if (graph.hasEdge(fromVertex, toVertex)) {
                return graph.getEdge(fromVertex, toVertex);
            }
            return graph.getEdge(toVertex, fromVertex);
        }

        [[nodiscard]] std::shared_ptr<sem2::ISequence<Vertex>> getVertices() const override {
            return graph.getVertices();
        }

        void addVertex(Vertex vertex) override {
            graph.addVertex(vertex);
        }

        void addEdge(Edge edge,
                     Vertex fromVertex,
                     Vertex toVertex) override {
            if (graph.hasEdge(toVertex, fromVertex)) {
                graph.addEdge(edge, toVertex, fromVertex);
            }
            graph.addEdge(edge, fromVertex, toVertex);
        }

        void removeEdge(Edge edge) override {
            graph.removeEdge(edge);
        }
    };

    /*
    template<typename Vertex, typename Edge>
    class templateDirectedGraphImplementation : DirectedGraph<Vertex, Edge> {
     public:
        [[nodiscard]]  bool
        hasEdge(Vertex fromVertex, Vertex toVertex) const override;

        [[nodiscard]]  Edge
        getEdge(Vertex fromVertex, Vertex toVertex) const override;

        [[nodiscard]]  std::shared_ptr<sem2::ISequence<Vertex>> getVertices() const override;

         void addVertex(Vertex vertex) override;

         void removeVertex(Vertex vertex) override;

         void addEdge(Edge edge,
                             Vertex fromVertex,
                             Vertex toVertex) override;

         void removeEdge(Edge edge) override;

        [[nodiscard]]  Vertex getHead(Edge edge) const override;

        [[nodiscard]]  Vertex getTail(Edge edge) const override;

        [[nodiscard]]  std::shared_ptr<sem2::ISequence<Edge>>
        getOutgoingEdges(Vertex vertex) const override;

        [[nodiscard]]  std::shared_ptr<sem2::ISequence<Edge>>
        getIngoingEdges(Vertex vertex) const override;
    };
     */
}
