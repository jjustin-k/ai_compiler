#include "../include/utils/json_parser.hpp"
#include "../include/codegen/codegen.hpp"
#include "../include/ir/graph.hpp"
#include "../include/ir/graph_builder.hpp"
#include "../include/optimizer/optimizer.hpp"
#include "../include/utils/algorithms.hpp"
#include "utils/logger.hpp"
#include <fstream>
#include <iostream>
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

    if (globalLogger.currentLevel == Logger::LogLevel::DEBUG) {
        graph.printGraph();
    }

    inference(graph, inputs);
}

std::vector<int> new_shape(json attributes, std::vector<int> input_shape) {
    int oh = 0;
    int ow = 0;
    int h = input_shape[input_shape.size() - 2];
    int w = input_shape[input_shape.size() - 1];

    if (attributes.contains("kernel_shape") && attributes.contains("strides") &&
        attributes.contains("pads")) {
        std::vector<int> k = attributes["kernel_shape"];
        std::vector<int> s = attributes["strides"];
        std::vector<int> p = attributes["pads"];

        if (p.size() != 4)
            throw std::runtime_error("pads must contain 4 values");

        oh = ((h - k[0] + p[0] + p[2]) / s[0]) + 1;
        ow = ((w - k[1] + p[1] + p[3]) / s[1]) + 1;

    } else {
        globalLogger.error("Key 'name' does not exist.");
    }
    input_shape[input_shape.size() - 2] = oh;
    input_shape[input_shape.size() - 1] = ow;

    return input_shape;
}

std::vector<int> shape_post_conv(json attributes, std::vector<int> input_shape,
                                 std::vector<int> weight_shape) {
    // only supporting stride one currently
    std::vector<int> s = attributes["strides"];
    std::vector<int> k = attributes["kernel_shape"];
    input_shape[1] = weight_shape[0];
    /*
     if (attributes["auto_pad"] == "SAME_UPPER") {

        input_shape[2] = (input_shape[2] - k[0]) / s[0] + 1;
        input_shape[3] = (input_shape[3] - k[1]) / s[1] + 1;
    }
    */

    return input_shape;
}

void flatten_array(const json &j, std::vector<float> &out) {
    if (j.is_array()) {
        for (const auto &element : j) {
            flatten_array(element, out);
        }
    } else if (j.is_number_integer() || j.is_number_float()) {
        out.push_back(j.get<float>());
    } else {

        throw std::runtime_error("Expected integer value in tensor data");
    }
}

void build_from_onnx(json data) {
    globalLogger.info("Starting build from onnx...");

    Graph graph;
    GraphBuilder graph_builder;
    globalLogger.info("Creating computation graph from json data...");
    json nodes = data["nodes"];
    std::vector<int> last_avail_shape;
    for (auto &node : nodes) {

        globalLogger.debug("Current node in json data: " + node["name"].dump());
        std::vector<Node *> layer;

        globalLogger.debug("Current node's number of inputs: " + std::to_string(node["inputs"].size()));
        for (auto &raw_input : node["inputs"]) {
            globalLogger.debug("Raw input: ");
            globalLogger.debug(raw_input);
            std::string input = raw_input.get<std::string>();
            globalLogger.debug("Input: " + input);
            Tensor *tensor = nullptr;
            OpType input_type = OpType::Input;
            if (data["initializers"].contains(input)) {
                globalLogger.debug("Input: " + input);
                input_type = OpType::Constant;
                if (data["initializers"][raw_input]["values"].is_array()) {
                    globalLogger.debug("Mutiple values");
                    // globalLogger.debug(data["initializers"][raw_input]["values"].dump());
                    // globalLogger.debug(data["initializers"][raw_input]["dims"].dump());

                    std::vector<float> flat_array;
                    flatten_array(data["initializers"][raw_input]["values"], flat_array);
                    if (input.find("shape") != std::string::npos) {
                        tensor = new Tensor(flat_array, data["initializers"][raw_input]["values"]);
                    } else {
                        tensor = new Tensor(flat_array, data["initializers"][raw_input]["dims"]);
                    }
                }

            } else if (input == "Input3") {
                tensor = new Tensor();
                std::vector<int> t_shape;
                for (auto &dim : data["inputs"][0]["shape"]) {
                    t_shape.push_back(dim);
                }
                tensor->setShape(t_shape);
            }

            if (!graph.nodeExists(input)) {
                globalLogger.debug("Input: " + input);
                graph_builder.addInputNode(graph, input, OpType::Constant, tensor);
            }
            globalLogger.debug("Input: " + input);

            layer.push_back(graph.getNode(input));
            globalLogger.debug("Added node to layer");
        }

        globalLogger.debug(node["outputs"]);
        std::string node_name = node["outputs"][0].get<std::string>();
        globalLogger.info("Finished inputs for :" + node_name);

        if (node["op_type"] == "Constant") {
            std::vector<int> c_shape;

            for (auto &a : node["attributes"]["dims"]) {
                c_shape.push_back(a);
            }
            graph_builder.addNode(graph, node_name, OpType::Constant, layer, c_shape);
            continue;
        }

        std::vector<int> node_shape = layer[0]->shape;

        if (node["op_type"] == "Add") {
            std::vector<int> node_shape = layer[0]->shape;
            globalLogger.debug("Optype is Add");

            graph_builder.addNode(graph, node_name, OpType::Add, layer, node_shape);
        } else if (node["op_type"] == "Sub") {
            graph_builder.addNode(graph, node_name, OpType::Sub, layer);
        } else if (node["op_type"] == "MaxPool") {
            if (node["attributes"]["auto_pad"] != "SAME_UPPER") {
                node_shape = new_shape(node["attributes"], node_shape);
            } else {
                node_shape = shape_post_conv(node["attributes"], node_shape,
                                             data["initializers"][layer[1]->name]["dims"]);
            }

            graph_builder.addNode(graph, node_name, OpType::MaxPool, layer, node_shape, node["attributes"]);
        } else if (node["op_type"] == "Conv") {
            if (node["attributes"]["auto_pad"] != "SAME_UPPER") {
                // For conv, only change last two , h,w
                node_shape = new_shape(node["attributes"], node_shape);
            } else {
                node_shape = shape_post_conv(node["attributes"], node_shape,
                                             data["initializers"][layer[1]->name]["dims"]);
            }
            globalLogger.debug("Node shape for conv");
            for (auto &a : node_shape) {
                globalLogger.debug(a);
            }
            graph_builder.addNode(graph, node_name, OpType::Conv, layer, node_shape, node["attributes"]);
        } else if (node["op_type"] == "MatMul") {
            std::vector<int> matmul_shape;
            if (layer[0]->shape.size() > 2) {
                for (int i = 0; i < layer[0]->shape.size() - 2; i++) {
                    matmul_shape.push_back(layer[0]->shape[i]);
                    // assumption leading elements in layer[0] and layer[1] are the same
                }
            }
            matmul_shape.push_back(layer[0]->shape[layer[0]->shape.size() - 2]);
            matmul_shape.push_back(layer[1]->shape[layer[1]->shape.size() - 1]);
            node_shape = matmul_shape;

            // Assuming 2d mat mul, getting the dims.
            graph_builder.addNode(graph, node_name, OpType::MatMul, layer, node_shape);
        } else if (node["op_type"] == "Relu") {

            graph_builder.addNode(graph, node_name, OpType::ReLU, layer, node_shape);
        } else if (node["op_type"] == "Reshape") {
            globalLogger.debug("Optype is Reshape");

            node_shape = layer[1]->shape;

            graph_builder.addNode(graph, node_name, OpType::Reshape, layer, node_shape);
        } else {
            globalLogger.error("Operation not supported");

            graph_builder.addNode(graph, node_name, OpType::ReLU, layer, node_shape);
        }
        last_avail_shape = node_shape;
        globalLogger.debug("Model's input layer");
        for (auto &i : layer) {
            globalLogger.debug(i->name);
        }
    }

    CodeGen codegen("../output/compiled_model.c");

    Optimizer opt(graph);
    globalLogger.debug("Done Optimizations");
    codegen.generateCode(graph);

    // run_cpp(graph);
    globalLogger.info("Finished build");
    graph_builder.deleteGraph(graph);
}

void JSONParser::parse(const std::string path) {
    std::ifstream file(path);
    json data;
    file >> data;
    build_from_onnx(data);
}
