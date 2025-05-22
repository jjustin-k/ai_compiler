#include "../include/ir/graph_builder.hpp"
#include<iostream>

void GraphBuilder::deleteNode(Node * node)
{
    //only have to delete the op since all nodes get deleted and inputs are refrences
    delete node->op;
}

//not pointing at a graph because there should only be one throughout runtime?
void GraphBuilder::deleteGraph(Graph& graph)
{
    for(auto& node : graph.getNodes()){
        deleteNode(node);
    }
}


Node* GraphBuilder::addNode(Graph& graph, std::string name, Operation* operation, std::vector<Node *>& inputs)
{
    Node* new_node = new Node;

    new_node->name = name;
    new_node->op = operation;
    new_node->input = inputs;

    for (auto& node : inputs) {
        node->output.push_back(new_node);
        new_node->input.push_back(node);
    }
    graph.addNode(new_node);
    return new_node;
}