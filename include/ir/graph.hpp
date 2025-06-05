#pragma once
#include<vector>
#include<unordered_map>
#include "tensor.hpp"
#include "./ops/operation.hpp"

struct Node{
    std::string name;
    std::string op_name;
    Operation* op;
    std::vector<Node*> output;
    std::vector<Node*> input;
    bool is_constant;
    Tensor* tensor;
};

class Graph{
    private:
        std::vector<Node*> nodes;
        std::unordered_map<std::string, Node*> name_node;
        std::vector<size_t> input_nodes;
    public:
        void addNode(Node* node);
        void printGraph();
        bool nodeExists(std::string node_name);
        Node* getNode(std::string node_name);
        std::vector<Node*> getNodes();
        std::vector<size_t> getInputNodes();
        size_t getNumOfNodes();
        size_t getNumOfInputNodes();
  
};

