#pragma once
#include<vector>
#include "tensor.hpp"
#include "./ops/operation.hpp"

enum Type{
    CONSTANT,
    OPERATION
};

struct Node{
    Type node_type;
    std::string name;
    Operation* op;
    std::vector<Node*> output;
    std::vector<Node*> input;
};

class Graph{
    private:
        std::vector<Node*> nodes;
    public:
        void addNode(Node* node);
        void printGraph();
};

