#include "../../asserts.h"
#include "../adjacency_matrix_graph.h"
#include "../bfs.h"
#include "../spf.h"
#include "../output_functions.h"

class MyWeightedEdge : public sem3::WeightedEdge<unsigned> {
    unsigned number;
    static unsigned counter;
public:
    explicit MyWeightedEdge(unsigned weight = 0) : WeightedEdge<unsigned>(weight), number(counter++) {}

    bool operator==(const MyWeightedEdge &other) const {
        return number == other.number;
    }
};

unsigned MyWeightedEdge::counter = 0;

std::ostream &operator<<(std::ostream &os, const MyWeightedEdge &edge) {
    os << "[" << edge.getWeight() << "]";
    return os;
}

namespace std { // just an example
    template<>
    struct hash<MyWeightedEdge> {
        std::size_t operator()(MyWeightedEdge const &edge) const noexcept {
            return std::hash<unsigned>{}(edge.getWeight());
        }
    };
}

sem3::AdjacencyMatrixGraph<int, MyWeightedEdge> getSmallGraph() {
    auto g = sem3::AdjacencyMatrixGraph<int, MyWeightedEdge>();
    g.addVertex(2);
    g.addVertex(4);
    g.addVertex(3);
    g.addVertex(5);
    g.addVertex(7);
    g.addEdge(MyWeightedEdge(1), 2, 3);
    g.addEdge(MyWeightedEdge(1), 4, 3);
    g.addEdge(MyWeightedEdge(3), 3, 5);
    g.addEdge(MyWeightedEdge(5), 3, 7);
    g.addEdge(MyWeightedEdge(1), 5, 7);
    //g=    2       5
    //     (1)\  /(3) \(1)
    //         3 -(5)> 7
    //     (4)/
    //      4
    return std::move(g);
}

void test_topological_order() {
    auto g = getSmallGraph();

    // right order: 2 -> 4 -> 3 -> 5 -> 7
    // or
    // right order: 4 -> 2 -> 3 -> 5 -> 7
    auto order = sem3::BFS<int, MyWeightedEdge>::searchTopologicalOrder(g);
    std::cout << order << std::endl;
    ASSERT_EQUAL(order.getLength(), 5u);
    ASSERT_EQUAL(order.get(0) % 2, 0);
    ASSERT_EQUAL(order.get(1) % 2, 0);
    ASSERT_EQUAL(order.get(2), 3);
    ASSERT_EQUAL(order.get(3), 5);
    ASSERT_EQUAL(order.get(4), 7);
}

void test_spf() {
    auto g = getSmallGraph();

    sem3::SPF<int, MyWeightedEdge> spf;
    auto r = spf.run(g, 2);
    std::cout << r.vertexToMinPathWeight << '\n' << r.vertexToPreviousVertexInPath << '\n'
              << r.visitedVertices << std::endl;
    ASSERT_EQUAL(r.visitedVertices.getCount(), 4u);
    ASSERT_EQUAL(r.vertexToMinPathWeight.get(2), 0u);
    ASSERT_EQUAL(r.vertexToMinPathWeight.get(3), 1u);
    ASSERT_EQUAL(r.vertexToMinPathWeight.get(5), 4u);
    ASSERT_EQUAL(r.vertexToMinPathWeight.get(7), 5u);
    ASSERT_EQUAL(r.vertexToMinPathWeight.containsKey(4), false);

    ASSERT_EQUAL(r.vertexToPreviousVertexInPath.get(3), 2);
    ASSERT_EQUAL(r.vertexToPreviousVertexInPath.get(5), 3);
    ASSERT_EQUAL(r.vertexToPreviousVertexInPath.get(7), 5);
}

void test_weightiest_path() {
    auto g = getSmallGraph();
    sem2::ArraySequence<int> seq;
    tests::assert_equal(sem3::BFS<int, MyWeightedEdge>::trySearchWeightiestPath(g, 2, 7, seq),
                        true, __FILE__, __LINE__);
    std::cout << seq << std::endl;
    ASSERT_EQUAL(seq.getLength(), 3u);
    ASSERT_EQUAL(seq.get(0), 7);
    ASSERT_EQUAL(seq.get(1), 3);
    ASSERT_EQUAL(seq.get(2), 2);
}

int main() {
    test_topological_order();
    test_weightiest_path();
    test_spf();
    std::cout << "FINISH" << std::endl;
}