#pragma once
#include "graph.hpp"

class GraphBuilder{
    private:
        void deleteNode(Node* node);
    public:
        void deleteGraph(Graph& graph);
        Node* addNode(Graph& graph, std::string name, OpType op_type, std::vector<Node *>& inputs);
        Node* addNode(Graph& graph, std::string name, OpType op_type, std::vector<Node *>& inputs, std::vector<int>& shape);
        Node* addInputNode(Graph& graph, std::string name, OpType op_type, Tensor* tensor);
};