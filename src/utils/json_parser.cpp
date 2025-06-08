#include "../include/utils/json_parser.hpp"
#include <nlohmann/json.hpp>
#include "../include/ir/graph_builder.hpp"
#include "../include/ops/add.hpp"
#include "../include/ops/sub.hpp"
#include "../include/ops/max_pooling.hpp"
#include "../include/ops/mat_mul.hpp"
#include "../include/ops/relu.hpp"
#include "../include/ir/graph.hpp"
#include "../include/codegen/codegen.hpp"
#include <fstream>
#include <unordered_set>
#include <iostream> 

using json = nlohmann::json;

void build(json data){
    Graph graph;
    GraphBuilder graph_builder;
    for(auto& node : data["nodes"]){
        std::cout << node << std::endl;
        std::vector<Node*> layer;

        std::cout << node["inputs"].size() << std::endl;
        for(auto& input : node["inputs"]){
            //std::cout << input << std::endl;
            bool is_constant = false;   
            Tensor* tensor = nullptr;
            if(data["weights"].contains(input)){
                std::cout<< "Is weight" << std::endl;
                is_constant = true;
                tensor = new Tensor(data["weights"][input]["values"], data["weights"][input]["shape"]);
            }
            
            if(!graph.nodeExists(input)){
              graph_builder.addInputNode(graph, input, is_constant, tensor);
            }
            
            layer.push_back(graph.getNode(input));
               
            
        }
        
        if(node["op"] == "add"){
            graph_builder.addNode(graph, node["name"], new Add(), "add", layer);
        }
        else if(node["op"] == "sub"){
            graph_builder.addNode(graph, node["name"], new Subtract(), "sub", layer);
        }
        else if(node["op"] == "maxpool2d"){
            graph_builder.addNode(graph, node["name"], new MaxPooling(node["params"]["stride"], node["params"]["pads"]), "maxpool", layer);
        }
        else if(node["op"] == "matmul"){
            graph_builder.addNode(graph, node["name"], new MatMul(), "matmul", layer);
        }
        else if(node["op"] == "relu"){
            graph_builder.addNode(graph, node["name"], new ReLU(), "relu", layer);
        }

        for(auto& i : layer){
                std::cout << " ------In input layer : " << i->name << std::endl;
        }
        
    }

    //graph.printGraph();
    CodeGen codegen("/Users/justinkwinecki/Documents/Programming/Term_25-26/comp/ai_compiler/out.c");

    codegen.generateCode(graph);

    graph_builder.deleteGraph(graph);

}


void JSONParser::parse(const std::string path)
{
    std::ifstream file(path);
    json data;
    file >> data;
    build(data);

}