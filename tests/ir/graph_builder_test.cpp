#include"ir/graph_builder.hpp"
#include<iostream>

void building_test(){
    std::cout << "Hello World" << std::endl;
    Graph graph;

    Node n, n1, n2, n3, n4;

    n.name = "Input";
    n1.name = "Conv";
    n2.name = "Weight";
    n3.name = "MatMul";
    n4.name = "Conv";

    std::vector<Node*> input_layer;
    std::vector<Node*> m1;
    std::vector<Node*> m2;
    std::vector<Node*> output_layer;

    input_layer.push_back(&n);
    m1.push_back(&n1);
    m1.push_back(&n2);
    m2.push_back(&n3);
    output_layer.push_back(&n4);

    GraphBuilder graph_builder;
    //Have to manually add first node
    graph.addNode(&n);
    graph_builder.addNode(graph, input_layer, &n1);
    //adding constant manually
    graph.addNode(&n2);
    graph_builder.addNode(graph, m1, &n3);
    graph_builder.addNode(graph, m2, &n4);
    graph.printGraph();
}

