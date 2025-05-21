#pragma once
#include "graph.hpp"

class GraphBuilder{
    public:
        void addNode(Graph& graph, std::vector<Node*>& inputs, Node* new_node);
};