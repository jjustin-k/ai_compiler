#include "../include/optimizer/optimizer.hpp"
#include <iostream>

Optimizer::Optimizer(Graph &graph) {
    std::string prev_op = "";
    this->graph = graph;
    for (auto &n : graph.getNodes()) {
        if (!n->is_constant) {

            std::cout << "Current op " << n->name << std::endl;
            std::cout << "Prev op " << prev_op << std::endl;

            prev_op = n->op_name;
        }
    }
}
