#pragma once
#include"./ir/graph.hpp"

void topological_sort(Graph& graph);

void dfs(bool *visted, Node* node, size_t index);

void layerize(Graph& graph, std::vector<size_t>* layer_sizes);

//figure out the inputs stuff later
void inference(Graph& graph, std::vector<Tensor>& inputs);



