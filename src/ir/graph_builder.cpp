#include "../include/ir/graph_builder.hpp"
#include <iostream>

void GraphBuilder::deleteNode(Node *node) {
    // only have to delete the op since all nodes get deleted and inputs are
    // refrences
    delete node->op;
    delete node->tensor;
}

// not pointing at a graph because there should only be one throughout runtime?
void GraphBuilder::deleteGraph(Graph &graph) {
    for (auto &node : graph.getNodes()) {
        deleteNode(node);
    }
}

// Issue in future may be because of passing in my reference instead of values
Node *GraphBuilder::addNode(Graph &graph, std::string name,
                            Operation *operation, std::string op_name,
                            std::vector<Node *> &inputs) {
    Node *new_node = new Node;

    new_node->name = name;
    new_node->op = operation;
    new_node->input = inputs;
    new_node->op_name = op_name;
    new_node->is_constant = false;
    for (auto &node : inputs) {
        node->output.push_back(new_node);
    }
    graph.addNode(new_node);
    return new_node;
}

Node *GraphBuilder::addNode(Graph &graph, std::string name,
                            Operation *operation, std::string op_name,
                            std::vector<Node *> &inputs,
                            std::vector<int> &shape) {
    Node *new_node = addNode(graph, name, operation, op_name, inputs);
    Tensor *tensor = new Tensor();
    tensor->setShape(shape);
    new_node->tensor = tensor;

    return new_node;
}

Node *GraphBuilder::addInputNode(Graph &graph, std::string name,
                                 bool is_constant, Tensor *tensor) {
    Node *new_node = new Node;
    new_node->name = name;
    new_node->is_constant = is_constant;
    new_node->tensor = tensor;
    graph.addNode(new_node);
    return new_node;
}