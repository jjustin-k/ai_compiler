#pragma once
#include"./ir/graph.hpp"

class Optimizer{
    private:
        Graph graph;
    public:
        Optimizer(Graph& graph);
};