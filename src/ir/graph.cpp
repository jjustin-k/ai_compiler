#include "../include/ir/graph.hpp"
#include<queue>
#include<iostream>

void Graph::addNode(Node* node)
{
    nodes.push_back(node);
    if (node->input.empty()) {
        input_nodes.push_back(nodes.size() - 1);
    }
}

void Graph::printGraph()
{
    for(auto& node : nodes){
        std::cout << "________________\n" << node->name << "\n________________\n" << "|\n|\n|\n|\nY" << std::endl;
    }
}


std::vector<Node *> Graph::getNodes()
{
    return nodes;
}

std::vector<size_t> Graph::getInputNodes()
{
    return input_nodes;
}

size_t Graph::getNumOfNodes()
{
    return nodes.size();
}

size_t Graph::getNumOfInputNodes()
{
    return input_nodes.size();
}
