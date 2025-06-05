#include<iostream>

#include"ir/graph_builder.hpp"
#include"utils/algorithms.hpp"
#include"utils/json_parser.hpp"
#include"ops/operation.hpp"
#include"ops/add.hpp"
#include"ops/sub.hpp"
#include"ops/mult.hpp"
#include"ops/mat_mul.hpp"
#include"ops/max_pooling.hpp"

void buildTestGraph(Graph &graph){
  

}


void infer(Graph& graph){
    std::cout << "Infering" << std::endl;
    Tensor A({
        1, 2,
        3, 4,
    
        5, 6,
        7, 8
    }, {2, 1, 2, 2});

    Tensor B({
        10, 20,
        30, 40,
    
        50, 60,
        70, 80
    }, {2, 1, 2, 2});

    Tensor C({
        -1, -2,
        -3, -4,
    
        -5, -6,
        -7, -8
    }, {2, 1, 2, 2});
    
    
    std::vector<Tensor> inputs = {A, B, C};
    std::cout<< "Tensor size : " << A.size << std::endl;
    inference(graph, inputs);

}

void deleteGraph(Graph& graph){
    for(auto& node : graph.getNodes()){
        delete node->op;
        delete node;
    }
}


int main(){

    JSONParser parser;
    parser.parse("/Users/justinkwinecki/Documents/Programming/Term_25-26/comp/ai_compiler/templates/fusion_test.json");
    return 0;
}
