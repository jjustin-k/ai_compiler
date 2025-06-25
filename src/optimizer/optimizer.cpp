#include "../include/optimizer/optimizer.hpp"
#include <iostream>

Optimizer::Optimizer(Graph &graph) {

    this->graph = graph;
    int index = 0;
    std::vector<Node *> nodes = graph.getNodes();
    for (auto &n : nodes) {
        // std::cout << n->name << std::endl;
        if (n->op_type != OpType::Input && n->op_type != OpType::Constant) {
            // std::cout << "Proper Comp!!!" << std::endl;

            if (n->op_type == OpType::Add) {

                if (n->output.size() != 1) {
                    continue;
                }

                Node *next_node = n->output[0];
                if (next_node->op_type == OpType::ReLU) {
                    // fuse();
                    Node *fused = new Node;
                    fused->input = n->input;
                    fused->shape = n->shape;
                    fused->name = next_node->name;
                    fused->tensor = n->tensor;
                    fused->output = next_node->output;
                    fused->op_type = OpType::AddReLU;

                    nodes.erase(std::remove(nodes.begin(), nodes.end(), next_node), nodes.end());
                    nodes.erase(std::remove(nodes.begin(), nodes.end(), n), nodes.end());

                    nodes.insert(nodes.begin() + index, fused);

                    graph.setNodes(nodes);

                    std::cout << "____________________OPTIMIZED" << std::endl;
                    graph.optimized_nodes.push_back(next_node);
                    graph.optimized_nodes.push_back(n);
                    // delete n;
                } else {
                    std::cout << n->name << std::endl;
                }
            } else if (n->op_type == OpType::MatMul) {
                if (n->output.size() != 1) {
                    continue;
                }

                Node *next_node = n->output[0];
                if (next_node->op_type == OpType::Add) {
                    // fuse();
                    Node *fused = new Node;
                    std::vector<Node *> inputs;
                    for (auto &i : n->input) {
                        inputs.push_back(i);
                        std::cout << i->name << std::endl;
                    }
                    for (auto &i : next_node->input) {

                        if (i == n) {
                            continue;
                        }
                        std::cout << i->name << std::endl;
                        inputs.push_back(i);
                    }

                    fused->input = inputs;
                    fused->shape = n->shape;
                    fused->name = next_node->name;
                    fused->tensor = n->tensor;
                    fused->output = next_node->output;
                    fused->op_type = OpType::FullyConnected;

                    std::cout << "N: " << fused->input[0]->name << " Next: " << fused->input[2]->name
                              << std::endl;
                    nodes.erase(std::remove(nodes.begin(), nodes.end(), next_node), nodes.end());
                    nodes.erase(std::remove(nodes.begin(), nodes.end(), n), nodes.end());

                    nodes.insert(nodes.begin() + index, fused);

                    graph.setNodes(nodes);

                    std::cout << "____________________OPTIMIZED MATMUL ADD____________________" << std::endl;
                    graph.optimized_nodes.push_back(next_node);
                    graph.optimized_nodes.push_back(n);
                    // delete n;
                    std::cout << fused->name << std::endl;
                } else {
                    std::cout << n->name << std::endl;
                }
            } else {
                std::cout << n->name << std::endl;
            }
        }
        index++;
    }
    for (auto &n : nodes) {
    }
}
