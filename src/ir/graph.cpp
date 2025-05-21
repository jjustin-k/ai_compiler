#include "../include/ir/graph.hpp"
#include<queue>
#include<iostream>

void Graph::addNode(Node* node)
{
    nodes.push_back(node);
}

void Graph::printGraph()
{
    for(auto& node : nodes){
        std::cout << "________________\n" << node->name << "\n________________\n" << "|\n|\n|\n|\nY" << std::endl;
    }
}