#include "../include/codegen/codegen.hpp"
#include "../include/codegen/add_emitter.hpp"
#include "../include/codegen/add_relu_emitter.hpp"
#include "../include/codegen/conv_emitter.hpp"
#include "../include/codegen/emitter.hpp"
#include "../include/codegen/fully_connected_emitter.hpp"
#include "../include/codegen/matmul_emitter.hpp"
#include "../include/codegen/maxpool_emitter.hpp"
#include "../include/codegen/relu_emitter.hpp"
#include "../include/codegen/reshape_emitter.hpp"
#include "../include/codegen/sub_emitter.hpp"
#include "utils/logger.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

std::string opTypeToString(OpType op) {
    switch (op) {
    case OpType::Add:
        return "Add";
    case OpType::Sub:
        return "Sub";
    case OpType::Conv:
        return "Conv2D";
    case OpType::ReLU:
        return "ReLU";
    case OpType::MatMul:
        return "MatMul";
    case OpType::MaxPool:
        return "MaxPool";
    case OpType::Constant:
        return "Constant";
    case OpType::Input:
        return "Input";
    case OpType::AddReLU:
        return "AddReLU";
    case OpType::FullyConnected:
        return "FullyConnected";
    default:
        return "UnknownOp";
    }
}
CodeGen::CodeGen(std::string output_path) {
    this->output_path = output_path;
    writeToFile("#include<stdio.h>\n\n", false);
}

void CodeGen::writeToFile(std::string data, bool append) {
    if (append) {
        std::ofstream file(this->output_path,
                           std::ios::app); // Open in append mode
        if (file.is_open()) {
            file << data;
            file.close();
        }

    } else {
        std::ofstream file(this->output_path);
        if (file.is_open()) {
            file << data;
            file.close();
        }
    }
}

void CodeGen::generateConstants(Graph &graph) {

    std::vector<Node *> nodes = graph.getNodes();
    globalLogger.info("Generating constants");
    for (auto &input : graph.getInputNodes()) {

        if (input->name == "Input3") {
            globalLogger.debug(input->tensor->size);
            general_size = input->tensor->size;

            continue;
        }

        Tensor *tensor = input->tensor;
        int size = 1;
        for (auto &dim : input->shape) {
            size *= dim;
        }

        std::ostringstream constant_stream;
        constant_stream << "\nint " << input->name << "_size = " << size << ";\n";
        constant_stream << "float " << input->name << "[] = {\n    ";

        for (int i = 0; i < size; i++) {

            if (i != 0) {
                constant_stream << " ," << tensor->getDataA()[i];
            } else {
                constant_stream << tensor->getDataA()[i];
            }
        }

        constant_stream << "};\n";

        writeToFile(constant_stream.str(), true);
    }
}
/*
Change it so that the size being used is the size of the input, unless a
function that changes its size is called
*/

std::string CodeGen::generateOperations(Graph &graph) {
    std::ostringstream function_call_stream;
    std::unordered_set<std::string> set;
    std::unordered_set<std::string> defined_vars;
    std::unordered_map<OpType, OpEmitter *> emitters = {{OpType::Add, new AddEmitter()},
                                                        {OpType::AddReLU, new AddReluEmitter()},
                                                        {OpType::MatMul, new MatMulEmitter()},
                                                        {OpType::Sub, new SubEmitter()},
                                                        {OpType::MaxPool, new MaxPoolEmitter()},
                                                        {OpType::Conv, new ConvEmitter()},
                                                        {OpType::ReLU, new ReLUEmitter()},
                                                        {OpType::Reshape, new ReshapeEmitter()},
                                                        {OpType::FullyConnected, new FullyConnectedEmitter()}

    };
    globalLogger.info("Generating Operations");

    for (auto &node : graph.getNodes()) {
        std::vector<int> sizes;

        if (node->op_type == OpType::Constant || node->op_type == OpType::Input) {
            continue;
        }

        int s = 1;
        for (auto &a : node->shape) {
            s *= a;
        }
        /* Change this hardcoded value*/

        sizes.push_back(s);

        OpEmitter *emitter = emitters[node->op_type];

        if (!set.count(emitter->getOpName())) {
            emitter->emitFunctionDefinition(sizes);
            set.insert(emitter->getOpName());
        }

        std::cout << node->name << std::endl;
        std::cout << opTypeToString(node->op_type) << std::endl;
        emitter->emitInvocation(function_call_stream, node, defined_vars, sizes[0]);
    }

    for (auto &a : emitters) {
        delete a.second;
    }

    return function_call_stream.str();
}

void CodeGen::generateMain(Graph &graph, std::string body) {

    std::ostringstream main_stream;
    main_stream << "\nint main(){\n" << body << "\nreturn 0;\n};\n";

    writeToFile(main_stream.str(), true);
}

void CodeGen::writeTestInput(Graph &graph) {

    std::string input = "\nint x_size = 64;\nfloat x[] = {"
                        "1, 2, 3, 4, 5, 6, 7, 8,"
                        "9, 10, 11, 12, 13, 14, 15, 16,"
                        "17, 18, 19, 20, 21, 22, 23, 24,"
                        "25, 26, 27, 28, 29, 30, 31, 32,"
                        "33, 34, 35, 36, 37, 38, 39, 40,"
                        "41, 42, 43, 44, 45, 46, 47, 48,"
                        "49, 50, 51, 52, 53, 54, 55, 56,"
                        "57, 58, 59, 60, 61, 62, 63, 64"
                        "};\n";

    writeToFile(input, true);
}

void CodeGen::generateCode(Graph &graph) {
    // call a function for initial setup
    writeTestInput(graph);
    generateConstants(graph);
    generateMain(graph, generateOperations(graph));
}