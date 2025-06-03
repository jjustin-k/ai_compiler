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
    std::cout << "Hello World" << std::endl;

    Node* n = new Node;
    Node* n1 = new Node;

    Node* n3 = new Node;

    n->name = "Input";
    n1->name = "Input";
    n3->name = "Input";

    std::vector<Node*> input_layer;
    std::vector<Node*> m1;
    std::vector<Node*> m2;

    input_layer.push_back(n);
    input_layer.push_back(n1);
    

    GraphBuilder graph_builder;

    //Have to manually add first node
    graph.addNode(n);
    graph.addNode(n1);
    Node* n2 = graph_builder.addNode(graph, "Add", new Add(), input_layer);
    m1.push_back(n2);
    graph.addNode(n3);
    m1.push_back(n3);
    //adding constant manually
   
    graph_builder.addNode(graph, "MatMul", new MatMul(), m1);
    
    graph.printGraph();

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
    parser.parse("/Users/justinkwinecki/Documents/Programming/Term_25-26/comp/ai_compiler/templates/example.json");
    return 0;
}
