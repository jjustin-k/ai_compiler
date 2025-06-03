#include "../include/utils/json_parser.hpp"
#include <nlohmann/json.hpp>
#include "../include/ir/graph_builder.hpp"
#include "../include/ops/add.hpp"
#include "../include/ops/sub.hpp"
#include "../include/ops/max_pooling.hpp"
#include "../include/ops/mat_mul.hpp"
#include "../include/ir/graph.hpp"
#include <fstream>
#include <iostream> 

using json = nlohmann::json;

void build(json data){
    Graph graph;
    GraphBuilder graph_builder;
    for(auto& node : data["nodes"]){
        std::cout << node << std::endl;
        std::vector<Node*> layer;
        for(auto& input : node["inputs"]){
            std::cout << input << std::endl;

            if(graph.nodeExists(input)){
                layer.push_back(graph.getNode(input));
            }
            else{
                graph_builder.addInputNode(graph, input);
            }
        }
        
        if(node["op"] == "add"){
            graph_builder.addNode(graph, node["name"], new Add(), layer);
        }
        else if(node["op"] == "sub"){
            graph_builder.addNode(graph, node["name"], new Subtract(), layer);
        }
        else if(node["op"] == "maxpool2d"){
            graph_builder.addNode(graph, node["name"], new MaxPooling(node["params"]["stride"], node["params"]["kernel_size"]), layer);
        }
        else if(node["op"] == "matmul"){
            graph_builder.addNode(graph, node["name"], new MatMul(), layer);
        }
        
    }

    graph.printGraph();
    graph_builder.deleteGraph(graph);

}


void JSONParser::parse(const std::string path)
{
    std::ifstream file(path);
    json data;
    file >> data;
    build(data);

}