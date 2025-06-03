#pragma once
#include "graph.hpp"

class GraphBuilder{
    private:
        void deleteNode(Node* node);
    public:
        void deleteGraph(Graph& graph);
        Node* addNode(Graph& graph, std::string name, Operation* operation, std::string op_name, std::vector<Node *>& inputs);
        Node* addInputNode(Graph& graph, std::string name);
};