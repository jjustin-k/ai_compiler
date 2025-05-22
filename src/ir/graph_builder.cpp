#include "../include/ir/graph_builder.hpp"
#include<iostream>

void GraphBuilder::addNode(Graph& graph, std::vector<Node *>& inputs, Node* new_node)
{

    for (auto& node : inputs) {
        node->output.push_back(new_node);
        new_node->input.push_back(node);
        std::cout<< " node : "<< node->name << std::endl;
        std::cout<< "Input node : " << new_node->input.size() << std::endl;
    
    }
    graph.addNode(new_node);
}