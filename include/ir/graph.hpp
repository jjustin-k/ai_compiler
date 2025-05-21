#pragma once
#include<vector>
#include "tensor.hpp"

enum Operation{
    RELU,
    SIGMOID,
    ADD,
    SUB,
    MULT,
    MATMULT,
    DIV
};

struct Node{
    float value;
    float bias;
    Operation op;
    std::vector<Edge*> output;
    std::vector<Edge*> input;
};

struct Edge{
    float weight;
    Node* start_node;
    Node* end_node;
};

struct Graph{
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};

void printGraph(Graph &graph);