#include<iostream>

#include"ir/graph_builder.hpp"
#include"utils/algorithms.hpp"
#include"ops/operation.hpp"
#include"ops/add.hpp"
#include"ops/sub.hpp"
#include"ops/mult.hpp"

void buildTestGraph(Graph &graph){
    std::cout << "Hello World" << std::endl;

    Node* n = new Node;
    Node* n1 = new Node;
    Node* n2 = new Node;
    Node* n3 = new Node;
    Node* n4 = new Node;

    n->name = "Input";
    n1->name = "Add";
    n2->name = "Input";
    n3->name = "Subtract";
    n4->name = "Multiply";

    Operation* add = new Add();
    Operation* sub = new Subtract();
    Operation* mult = new Multiply();

    n1->op = add;
    n3->op = sub;
    n4->op = mult;
    

    std::vector<Node*> input_layer;
    std::vector<Node*> m1;
    std::vector<Node*> m2;
    std::vector<Node*> output_layer;

    input_layer.push_back(n);
    m1.push_back(n1);
    m1.push_back(n2);
    m2.push_back(n3);
    output_layer.push_back(n4);

    GraphBuilder graph_builder;
    //Have to manually add first node
    graph.addNode(n);
    graph_builder.addNode(graph, input_layer, n1);
    //adding constant manually
    graph.addNode(n2);
    graph_builder.addNode(graph, m1, n3);
    graph_builder.addNode(graph, m2, n4);
    graph.printGraph();

}


void infer(Graph& graph){
    std::cout << "Infering" << std::endl;
    Tensor tensor1({1.2f, 2.3f}, {2});
    Tensor tensor2({4.5f, 5.5f}, {2});
    std::vector<Tensor> inputs = {tensor1, tensor2};
    std::cout<< "Tensor size : " << tensor1.size << std::endl;
    inference(graph, inputs);

}

void deleteGraph(Graph& graph){
    for(auto& node : graph.getNodes()){
        delete node->op;
        delete node;
    }
}


int main(){
    Graph graph;
    buildTestGraph(graph);
    infer(graph);

    deleteGraph(graph);
    return 0;
}
