#pragma once
#include<vector>
#include "tensor.hpp"
#include "./ops/operation.hpp"

struct Node{
    std::string name;
    Operation* op;
    std::vector<Node*> output;
    std::vector<Node*> input;
};

class Graph{
    private:
        std::vector<Node*> nodes;
        std::vector<size_t> input_nodes;
    public:
        void addNode(Node* node);
        void printGraph();
        std::vector<Node*> getNodes();
        std::vector<size_t> getInputNodes();
        size_t getNumOfNodes();
        size_t getNumOfInputNodes();
  
};

