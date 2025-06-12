#include "../include/optimizer/optimizer.hpp"
#include <iostream>

Optimizer::Optimizer(Graph &graph) {
    std::string prev_op = "";
    this->graph = graph;
    for (auto &n : graph.getNodes()) {
    }
}
