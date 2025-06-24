#include "../include/ir/graph_builder.hpp"
#include <iostream>

void GraphBuilder::deleteNode(Node *node) {
    // only have to delete the op since all nodes get deleted and inputs are
    // refrences
    delete node->tensor;
}

// not pointing at a graph because there should only be one throughout runtime?
void GraphBuilder::deleteGraph(Graph &graph) {
    for (auto &node : graph.getNodes()) {
        deleteNode(node);
        delete node;
    }
    for (auto &node : graph.optimized_nodes) {
        deleteNode(node);
        // delete node;
    }
}

// Issue in future may be because of passing in my reference instead of values
Node *GraphBuilder::addNode(Graph &graph, std::string name, OpType op_type,
                            std::vector<Node *> &inputs) {
    Node *new_node = new Node;

    new_node->name = name;
    new_node->input = inputs;
    new_node->op_type = op_type;
    for (auto &node : inputs) {
        node->output.push_back(new_node);
    }
    graph.addNode(new_node);
    return new_node;
}

Node *GraphBuilder::addNode(Graph &graph, std::string name, OpType op_type,
                            std::vector<Node *> &inputs,
                            std::vector<int> &shape) {
    Node *new_node = addNode(graph, name, op_type, inputs);
    new_node->shape = shape;

    return new_node;
}

Node *GraphBuilder::addInputNode(Graph &graph, std::string name, OpType op_type,
                                 Tensor *tensor) {
    Node *new_node = new Node;
    new_node->name = name;
    new_node->op_type = op_type;
    new_node->tensor = tensor;
    new_node->shape = tensor->getShape();
    graph.addNode(new_node);
    return new_node;
}