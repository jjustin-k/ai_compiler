#include "../include/codegen/codegen.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

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

void CodeGen::write_function(std::string return_type, std::string name,
                             std::string parameters, std::string body) {
    std::ostringstream function_stream;
    function_stream << "\n"
                    << return_type << " " << name << "(" << parameters
                    << "){\n    " << body << "\n}\n";

    std::string function = function_stream.str();
    writeToFile(function, true);
}

std::string CodeGen::writeForLoop(std::string start, std::string end,
                                  std::string change, std::string statement) {
    std::ostringstream loop_stream;
    loop_stream << "\nfor(" << start << "; " << end << "; " << change
                << "){\n    " << statement << "\n}\n";

    std::string loop = loop_stream.str();
    return loop;
}

void CodeGen::generateConstants(Graph &graph) {

    std::vector<Node *> nodes = graph.getNodes();
    std::cout << "Number of input nodes : " << graph.getInputNodes().size()
              << std::endl;
    for (auto &input_index : graph.getInputNodes()) {
        std::cout << input_index << std::endl;
        Node *input = nodes[input_index];

        std::cout << input->name << std::endl;
        if (input->name == "x") {
            general_size = 64;
            std::cout << input->name << std::endl;
            std::cout << input_index << std::endl;
            continue;
        }

        Tensor *tensor = input->tensor;
        int size = 1;
        for (auto &dim : input->shape) {
            size *= dim;
        }

        std::ostringstream constant_stream;
        constant_stream << "\nint " << input->name << "_size = " << size
                        << ";\n";
        constant_stream << "float " << input->name << "[] = {\n    ";

        for (int i = 0; i < size; i++) {

            if (i != 0) {
                constant_stream << " ," << tensor->getDataA()[i];
            } else {
                constant_stream << tensor->getDataA()[i];
            }
            std::cout << "Here" << std::endl;
        }

        constant_stream << "};\n";
        std::cout << constant_stream.str() << std::endl;

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
    for (auto &node : graph.getNodes()) {

        std::cout << node->name << std::endl;
        std::cout << "Input layer size : " << node->input.size() << std::endl;
        for (auto &i : node->input) {
            std::cout << "Has input : " << i->name << std::endl;
        }
        if (node->op_type != OpType::Constant ||
            node->op_type != OpType::Input) {

            std::cout << node->name << std::endl;
            if (node->op_type == OpType::Add) {

                if (!set.count("add")) {
                    std::string body = writeForLoop(
                        "int i = 0", "i < " + std::to_string(general_size),
                        "i++", "out[i] = a[i] + b[i];");
                    write_function("void", "add",
                                   "float* out, float* a, float* b", body);
                    set.insert("add");
                }

                if (!defined_vars.count(node->name)) {
                    function_call_stream << "\nfloat " << node->name << "["
                                         << general_size << "];\n";
                    defined_vars.insert(node->name);
                }
                function_call_stream << "\n add(" << node->name << ", "
                                     << node->input[0]->name << ", "
                                     << node->input[1]->name << ");\n";
            } else if (node->op_type == OpType::Sub) {
                if (!set.count("sub")) {
                    std::string body = writeForLoop(
                        "int i = 0", "i < " + std::to_string(general_size),
                        "i++", "out[i] = a[i] - b[i];");
                    write_function("void", "sub",
                                   "float* out, float* a, float* b", body);
                    set.insert("sub");
                }

                function_call_stream << "\n sub(" << node->name << ", "
                                     << node->input[0]->name << ");\n";
            } else if (node->op_type == OpType::MatMul) {
                if (!set.count("matmul2d")) {
                    std::cout << static_cast<int>(node->op_type) << std::endl;
                    std::string n_loop =
                        writeForLoop("int k = 0", "k < n", "k++",
                                     "sum += a[i * n + k] * b[k * p + j];");
                    std::string inner_loop =
                        writeForLoop("int j = 0", "j < p", "j++",
                                     "float sum = 0.0f;\n" + n_loop +
                                         "\nout[i * p + j] = sum;\n");
                    std::string body =
                        writeForLoop("int i = 0", "i < m", "i++", inner_loop);
                    write_function(
                        "void", "matmul2d",
                        "float* out, float* a, float* b, int m, int n, int p",
                        body);
                    set.insert("matmul2d");
                    std::cout << static_cast<int>(node->op_type) << std::endl;
                }

                function_call_stream
                    << "int " << node->name << "_m = " << node->shape[0]
                    << ";\nint " << node->name << "_n = " << node->shape[1]
                    << ";\nint " << node->name << "_p = " << node->shape[2]
                    << ";\n";

                if (!defined_vars.count(node->name)) {
                    general_size = node->shape[0] * node->shape[1];

                    // this is why I need to add shape to node
                    function_call_stream << "\nfloat " << node->name << "["
                                         << std::to_string(general_size)
                                         << "];\n";
                    defined_vars.insert(node->name);
                }

                function_call_stream
                    << "\n matmul2d(" << node->name << ", "
                    << node->input[0]->name << ", " << node->input[1]->name
                    << ", " << node->name << "_m, " << node->name << "_n, "
                    << node->name << "_p);\n";

            } else if (node->op_type == OpType::ReLU) {
                std::cout << "RELU :" << set.count("relu") << std::endl;
                if (!set.count("relu")) {
                    std::string body = writeForLoop(
                        "int i = 0", "i < " + std::to_string(general_size),
                        "i++", "a[i] = (a[i] > 0.0f) ? a[i] : 0.0f;");
                    write_function("void", "relu", "float* a", body);
                    set.insert("relu");
                    std::cout << "RELU :" << set.count("relu") << std::endl;
                }

                function_call_stream << "\n relu(" << node->input[0]->name
                                     << ");\n";
                node->name = node->input[0]->name;
            } else if (node->op_type == OpType::MaxPool) {
                if (!set.count("maxpool")) {
                    std::string body = "\nint ix = ox * stride + kx;\nint iy = "
                                       "oy * stride + ky;\n"
                                       "float val = a[iy * width + ix];\n"
                                       "if (val > max_val) {\n"
                                       "max_val = val;\n"
                                       "};";
                    std::string kx_loop = writeForLoop(
                        "int kx = 0", "kx < pool_size", "kx++", body);
                    std::string ky_loop = writeForLoop(
                        "int ky = 0", "ky < pool_size", "ky++", kx_loop);
                    std::string ox_loop =
                        writeForLoop("int ox = 0", "ox < out_w", "ox++",
                                     "\nfloat max_val = -1e9;\n" + ky_loop +
                                         "\nout[oy * out_w + ox] = max_val;\n");
                    std::string oy_loop = writeForLoop(
                        "int oy = 0", "oy < out_h", "oy++", ox_loop);

                    write_function(
                        "void", "maxpool2d",
                        " float *out, float *a, int width, int height, int "
                        "pool_size, int stride",
                        "\nint out_w = (width - pool_size) / stride + 1;"
                        "\nint out_h = (height - pool_size) / stride + 1;\n" +
                            oy_loop);
                    set.insert("maxpool");
                }

                if (!defined_vars.count(node->name)) {
                    // this is why I need to add shape to node
                    function_call_stream << "\nfloat " << node->name << "["
                                         << std::to_string(general_size)
                                         << "];\n";
                    defined_vars.insert(node->name);
                }
                function_call_stream
                    << "\n maxpool2d(" << node->name << ", "
                    << node->input[0]->name << ", " << node->input[0]->shape[0]
                    << ", " << node->input[0]->shape[1] << ", 2, 2);\n";
            }
        }
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