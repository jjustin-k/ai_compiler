#include "../include/optimizer/optimizer.hpp"
#include "../include/utils/logger.hpp"

#include <iostream>

Optimizer::Optimizer(Graph &graph) {
    globalLogger.info("Starting model optimizations");
    this->graph = graph;
    int index = 0;
    std::vector<Node *> nodes = graph.getNodes();
    for (auto &n : nodes) {
        globalLogger.debug("Node: " + n->name);
        if (n->op_type != OpType::Input && n->op_type != OpType::Constant) {

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

                    globalLogger.info("Node " + n->name + " optimized");
                    graph.optimized_nodes.push_back(next_node);
                    graph.optimized_nodes.push_back(n);
                    // delete n;
                } else {
                    globalLogger.info("Node " + n->name + " not optimized");
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
                    }
                    for (auto &i : next_node->input) {

                        if (i == n) {
                            continue;
                        }

                        inputs.push_back(i);
                    }

                    fused->input = inputs;
                    fused->shape = n->shape;
                    fused->name = next_node->name;
                    fused->tensor = n->tensor;
                    fused->output = next_node->output;
                    fused->op_type = OpType::FullyConnected;

                    nodes.erase(std::remove(nodes.begin(), nodes.end(), next_node), nodes.end());
                    nodes.erase(std::remove(nodes.begin(), nodes.end(), n), nodes.end());

                    nodes.insert(nodes.begin() + index, fused);

                    graph.setNodes(nodes);

                    globalLogger.info("OPTIMIZED MATMUL ADD");
                    graph.optimized_nodes.push_back(next_node);
                    graph.optimized_nodes.push_back(n);
                    // delete n;

                    globalLogger.debug("Name of fused node: " + fused->name);
                } else {
                    globalLogger.info("Node " + n->name + " not optimized");
                }
            } else {
                globalLogger.info("Node " + n->name + " not optimized");
            }
        }
        index++;
    }
}
