#include "../include/ir/graph_builder.hpp"


void GraphBuilder::addNode(Graph& graph, std::vector<Node *>& inputs, Node* new_node)
{

    for (auto& node : inputs) {
        node->output.push_back(new_node);
        new_node->input.push_back(node);
    }
    graph.addNode(new_node);
}