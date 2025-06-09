#include "../include/codegen/codegen.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

CodeGen::CodeGen(std::string output_path)
{
    this->output_path = output_path;
    writeToFile("#include<stdio.h>\n\nint general_size;\n\n", false);
}

void CodeGen::writeToFile(std::string data, bool append)
{
    if(append){
        std::ofstream file(this->output_path, std::ios::app); // Open in append mode
        if (file.is_open()) {
            file << data;
            file.close();
        } 
    
    }
    else{
        std::ofstream file(this->output_path);
        if (file.is_open()) {
            file << data;
            file.close();
        } 
    }
}

void CodeGen::write_function(std::string return_type, std::string name, std::string parameters, std::string body){
    std::ostringstream function_stream;
    function_stream << 
                        "\n" << return_type << " " << name << "(" << parameters << "){\n    " <<
                        body << "\n}\n";

    std::string function = function_stream.str();
    writeToFile(function, true);
}

std::string CodeGen::writeForLoop(std::string start, std::string end, std::string change, std::string statement)
{
    std::ostringstream loop_stream;
    loop_stream << "\nfor(" << start << "; " << end << "; " << change << "){\n    " <<
                statement << "\n}\n";

    std::string loop = loop_stream.str();
    return loop;
}


void CodeGen::generateConstants(Graph& graph){
    
    std::vector<Node*> nodes = graph.getNodes();
    std::cout << "Number of input nodes : " << graph.getInputNodes().size() << std::endl;
    for(auto& input_index : graph.getInputNodes()){
        std::cout << input_index << std::endl;
        Node* input = nodes[input_index];
        
        if(!input->is_constant){
            std::cout << input_index << std::endl;
            continue;
        }

        Tensor* tensor = input->tensor;
        int size = 1;
        for(auto& dim : input->tensor->getShape()){
            size *= dim;
        }

        std::ostringstream constant_stream;
        constant_stream << "\nint " <<  input->name << "_size = " << size << ";\n";
        constant_stream << "float " << input->name << "[] = {\n    ";
        float* values = tensor->getDataA();
        for(int i = 0; i < size; i++){
            if(i != 0){
                constant_stream << " ," << values[i]; 
            }
            else{
                constant_stream << values[i];
            }
           
        }
        constant_stream << "};\n";
        std::cout << constant_stream.str() << std::endl;
        writeToFile(constant_stream.str(), true);
    }
}
/*
Change it so that the size being used is the size of the input, unless a function that changes its size is called
*/

std::string CodeGen::generateOperations(Graph& graph){
    std::ostringstream function_call_stream;
    function_call_stream << "\ngeneral_size = x_size;\n\n";
    for(auto& node : graph.getNodes()){
        std::unordered_set<std::string> set;
        std::unordered_set<std::string> defined_vars;
        std::cout << node->op_name << std::endl;
        std::cout << "Input layer size : " << node->input.size() << std::endl;
        for(auto& i : node->input){
            std::cout << "Has input : " << i->name << std::endl;
        }
        if(node->op_name != ""){

            std::cout << node->op_name << std::endl;
            if(node->op_name == "add" && !set.count("add")){
                std::string body = writeForLoop("int i = 0", "i < size", "i++", "out[i] = a[i] + b[i];");
                write_function("void", "add", "float* out, float* a, float* b, int size", body);
                set.insert("add");
                if(!defined_vars.count(node->name)){
                    function_call_stream << "\nfloat " << node->name << "[general_size];\n";
                    defined_vars.insert(node->name);
                }
                function_call_stream << "\n add(" << node->name << ", " << node->input[0]->name << ", " << node->input[1]->name <<  ", general_size);\n";
            }
            else if(node->op_name == "sub" && !set.count("sub")){
                std::string body = writeForLoop("int i = 0", "i < size", "i++", "out[i] = a[i] - b[i];");
                write_function("void", "sub", "float* out, float* a, float* b, int size", body);
                set.insert("sub");
                function_call_stream << "\n sub(" << node->name << ", " << node->input[0]->name << ", general_size);\n";
            }
            else if(node->op_name == "matmul" && !set.count("matmul2d")){
                std::string n_loop = writeForLoop("int k = 0", "k < n", "k++", "sum += a[i * n + k] * b[k * p + j];");
                std::string inner_loop = writeForLoop("int j = 0", "j < p", "j++", "float sum = 0.0f;\n" + n_loop + "\nout[i * p + j] = sum;\n");
                std::string body = writeForLoop("int i = 0", "i < m", "i++", inner_loop);
                write_function("void", "matmul2d", "float* out, float* a, float* b, int m, int n, int p", body);
                set.insert("matmul2d");
                function_call_stream <<  "int " << node->name << "_m = " << node->tensor->getShape()[0] << ";\nint " << node->name << "_n = " << node->tensor->getShape()[1] << ";\nint " <<  node->name << "_p = " << node->tensor->getShape()[2] <<";\n";
                if(!defined_vars.count(node->name)){
                    
                    function_call_stream << "\ngeneral_size = " << node->tensor->getShape()[0] * node->tensor->getShape()[1] << ";\n";
                    //this is why I need to add shape to node
                    function_call_stream << "\nfloat " << node->name << "[general_size];\n";
                    defined_vars.insert(node->name);
                }
                function_call_stream << "\n matmul2d(" << node->name << ", " << node->input[0]->name << ", " << node->input[1]->name << ", "<< node->name << "_m, " << node->name << "_n, " << node->name << "_p);\n";
            }
            else if(node->op_name == "relu" && !set.count("relu")){
                std::string body = writeForLoop("int i = 0", "i < size", "i++", "a[i] = (a[i] > 0.0f) ? a[i] : 0.0f;");
                write_function("void", "relu", "float* a, int size", body);
                set.insert("relu");
                function_call_stream << "\n relu(" << node->input[0]->name << ", general_size);\n";
                node->name = node->input[0]->name;
            }
            else if(node->op_name == "maxpool" && !set.count("maxpool")){
                std::string body = writeForLoop("int i = 0", "i < size", "i++", "out[i] = (a[i] > 0.0f) ? a[i] : 0.0f;");
                write_function("void", "maxpool", "float* out, float* a, int size", body);
                set.insert("maxpool");
                if(!defined_vars.count(node->name)){
                    //this is why I need to add shape to node
                    function_call_stream << "\nfloat " << node->name << "[general_size];\n";
                    defined_vars.insert(node->name);
                }
                function_call_stream << "\n maxpool(" << node->name << ", " << node->input[0]->name << ", " << "general_size);\n";
            }
        }
    }
    return function_call_stream.str();
}

void CodeGen::generateMain(Graph& graph, std::string body){

    std::ostringstream main_stream;
    main_stream << "\nint main(){\n" << body << "\nreturn 0;\n};\n";

    writeToFile(main_stream.str(), true);

}

void CodeGen::writeTestInput(Graph& graph){

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

void CodeGen::generateCode(Graph& graph)
{
    //call a function for initial setup
    writeTestInput(graph);
    generateConstants(graph);
    generateMain(graph, generateOperations(graph));

}