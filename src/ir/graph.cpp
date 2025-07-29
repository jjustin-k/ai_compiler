#include "../include/ir/graph.hpp"
#include "utils/logger.hpp"
#include <iostream>
#include <queue>

void Graph::addNode(Node *node) {
    nodes.push_back(node);
    name_node[node->name] = node;
    if (node->input.empty()) {
        input_nodes.push_back(node);
    }
}

void Graph::printGraph() {
    
    for (auto &node : nodes) {
        std::cout << "________________\n"
                  << node->name << "\n________________\n"
                  << "|\n|\n|\n|\nY" << std::endl;
        
    }
}

std::vector<Node *> Graph::getNodes() { return nodes; }

void Graph::setNodes(std::vector<Node *> nodes) { this->nodes = nodes; }

bool Graph::nodeExists(std::string node_name) { return name_node[node_name] != NULL; }

Node *Graph::getNode(std::string node_name) { return name_node[node_name]; }

std::vector<Node *> Graph::getInputNodes() { return input_nodes; }

void Graph::addInputNode(Node *node) { input_nodes.push_back(node); }

size_t Graph::getNumOfNodes() { return nodes.size(); }

size_t Graph::getNumOfInputNodes() { return input_nodes.size(); }
