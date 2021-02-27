#pragma once

#include "abstract_graphs.h"


namespace sem3 {

    template<typename Weight>
    class WeightedEdge;

    template<typename WeightedEdge>
    WeightedEdge infinity() { return WeightedEdge(infinity<typename WeightedEdge::Weight>()); }

    template<typename W>
    class WeightedEdge {
    public:
        using Weight = W;

        explicit WeightedEdge(Weight weight = Weight()) : weight(weight) {}

        Weight getWeight() const { return weight; }

    private:
        Weight weight;
    };

//     SFINAE
    template<typename T>
    struct is_weighted {
    private:
        static void detect(...);

        template<typename U>
        static decltype(std::declval<U>().getWeight()) detect(const U &);

    public:
        static constexpr bool value = !std::is_same<void, decltype(detect(std::declval<T>()))>::value;
    };
}


namespace std {
    template<typename Weight>
    struct hash<sem3::WeightedEdge<Weight>> { // you'd better to override that
        std::size_t operator()(sem3::WeightedEdge<Weight> const &edge) const noexcept {
            return std::hash<Weight>{}(edge.getWeight());
        }
    };
}