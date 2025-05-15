#pragma once
#include<vector>

enum Activation{
    RELU,
    SIGMOID
};

struct Node{
    float value;
    float bias;
    Activation activation;
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