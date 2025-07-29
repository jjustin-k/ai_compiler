#include "../include/utils/json_parser.hpp"
#include "../include/codegen/codegen.hpp"
#include "../include/ir/graph.hpp"
#include "../include/ir/graph_builder.hpp"

#include "../include/optimizer/optimizer.hpp"
#include "../include/utils/algorithms.hpp"
#include <fstream>
#include <iostream>
#include "utils/logger.hpp"
#include <nlohmann/json.hpp>
#include <unordered_set>

using json = nlohmann::json;

std::vector<float> x = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,
                        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
                        33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
                        49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};

std::vector<float> w1 = {0.1, -0.2, 0.3, 0.4, 0.5, 0.6, -0.7, 0.8, -0.9, 1, 0.1, -0.2, 0.3, 0.4, -0.5, 0.6};

std::vector<float> b1 = {0.1, 0, -0.1, 0.2, 0.1, 0, -0.1, 0.2, 0.1, 0, -0.1, 0.2, 0.1, 0, -0.1, 0.2};

void run_cpp(Graph &graph) {
    Tensor A(x, {8, 8});

    Tensor B(w1, {4, 4});

    Tensor C(b1, {4, 4});

    std::vector<Tensor> inputs = {A, B, C};

    if(globalLogger.currentLevel == Logger::LogLevel::DEBUG){
        graph.printGraph();
    }
    
    inference(graph, inputs);
}

void build(json data) {
    globalLogger.info("Starting build...");
    Graph graph;
    GraphBuilder graph_builder;
    globalLogger.info("Creating computation graph from json data...");
    for (auto &node : data["nodes"]) {
        
        globalLogger.debug("Current node in json data: " + node["name"].dump());
        std::vector<Node *> layer;

        globalLogger.debug("Current node's number of inputs: " + std::to_string(node["inputs"].size()));
        for (auto &input : node["inputs"]) {
            
            Tensor *tensor = nullptr;
            OpType input_type = OpType::Input;
            if (data["weights"].contains(input)) {
                input_type = OpType::Constant;
                tensor = new Tensor(data["weights"][input]["values"], data["weights"][input]["shape"]);
            } else if (input == "x") {
                tensor = new Tensor();
                tensor->setShape(data["inputs"][input]["shape"]);
            }

            if (!graph.nodeExists(input)) {
                graph_builder.addInputNode(graph, input, OpType::Constant, tensor);
            }

            layer.push_back(graph.getNode(input));
        }

        if (node["op"] == "add") {
            graph_builder.addNode(graph, node["name"], OpType::Add, layer);
        } else if (node["op"] == "sub") {
            graph_builder.addNode(graph, node["name"], OpType::Sub, layer);
        } else if (node["op"] == "maxpool2d") {
            std::vector<int> dims;
            // Assuming 2d mat mul, getting the dims. temp fix is /2
            /*
            Change this to actually calculate new dimensions of the maxpool
            result using stride and kernel size
            */

            dims.push_back(layer[0]->shape[0] / 2);
            dims.push_back(layer[0]->shape[1] / 2);
            graph_builder.addNode(graph, node["name"], OpType::MaxPool, layer, dims);
        } else if (node["op"] == "matmul") {
            std::vector<int> dims;
            // Assuming 2d mat mul, getting the dims.

            dims.push_back(layer[0]->shape[0]);
            dims.push_back(layer[0]->shape[1]);
            dims.push_back(layer[1]->shape[1]);
            graph_builder.addNode(graph, node["name"], OpType::MatMul, layer, dims);
        } else if (node["op"] == "relu") {
            graph_builder.addNode(graph, node["name"], OpType::ReLU, layer);
        }
        
        globalLogger.debug("Model's input layer");
        for (auto &i : layer) {
            globalLogger.debug(i->name );
            
        }
    }

    if(globalLogger.currentLevel == Logger::LogLevel::DEBUG){
        graph.printGraph();
    }
    CodeGen codegen("/Users/justinkwinecki/Documents/Programming/Term_25-26/"
                    "comp/ai_compiler/out.c");

    Optimizer opt(graph);
    codegen.generateCode(graph);

    if(globalLogger.currentLevel == Logger::LogLevel::DEBUG){
        graph.printGraph();
    }

    run_cpp(graph);
    globalLogger.info("Finished build");
    graph_builder.deleteGraph(graph);
}

void JSONParser::parse(const std::string path) {
    std::ifstream file(path);
    json data;
    file >> data;
    build(data);
}
