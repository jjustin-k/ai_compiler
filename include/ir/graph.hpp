#pragma once
#include<vector>
#include<unordered_map>
#include "tensor.hpp"
#include <nlohmann/json.hpp>

enum class OpType {
    Add,
    Sub,
    Conv2D,
    ReLU,
    MatMul,
    MaxPool,
    Constant,
    Input,
    AddReLU,
    FullyConnected
};

struct Node{
    std::string name;
    std::string output_name;
    OpType op_type;
    std::vector<Node*> output;
    std::vector<Node*> input;
    std::vector<int> shape;
    Tensor* tensor;
    nlohmann::json attributes; 
};



class Graph{
    private:
        std::vector<Node*> nodes;
        std::unordered_map<std::string, Node*> name_node;
        std::vector<Node*> input_nodes;
        
    public:
        std::vector<Node*> optimized_nodes; //store nodes that have been fused to delete
        void addNode(Node* node);
        void printGraph();
        bool nodeExists(std::string node_name);
        Node* getNode(std::string node_name);
        void setNodes(std::vector<Node*> nodes);
        std::vector<Node*> getNodes();
        std::vector<Node*> getInputNodes();
        void addInputNode(Node* node);
        size_t getNumOfNodes();
        size_t getNumOfInputNodes();
  
};

