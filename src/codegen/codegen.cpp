#include "../include/codegen/codegen.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

CodeGen::CodeGen(std::string output_path)
{
    this->output_path = output_path;
    writeToFile("#include<stdio.h>\n\nint size = 10;\n", false);


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
                statement << "}\n";

    std::string loop = loop_stream.str();
    return loop;
}

void CodeGen::generateCode(Graph& graph)
{
    //call a function for initial setup;
    std::unordered_set<std::string> set;
    for(auto& node : graph.getNodes()){
        std::cout << node->name << std::endl;
        if(node->op_name != ""){
            if(node->op_name == "relu" && !set.count("relu")){
                std::string body = writeForLoop("int i = 0", "i < size", "i++", "out[i] = std::max(0.0f, out[i]);");
                write_function("void", "relu", "float* out, size", body);
                set.insert("relu");
            }
            else if(node->op_name == "add" && !set.count("add")){
                std::string body = writeForLoop("int i = 0", "i < size", "i++", "out[i] = a[i] + b[i];");
                write_function("void", "add", "float* out, float* a, float* b, size", body);
                set.insert("add");
            }
            else if(node->op_name == "sub" && !set.count("sub")){
                std::string body = writeForLoop("int i = 0", "i < size", "i++", "out[i] = a[i] - b[i];");
                write_function("void", "sub", "float* out, float* a, float* b, size", body);
                set.insert("sub");
            }
            else if(node->op_name == "matmul" && !set.count("matmul")){
                std::string body = writeForLoop("int i = 0", "i < size", "i++", "out[i] = a[i] - b[i];");
                write_function("void", "matmul", "float* out, float* a, float* b, size", body);
                set.insert("matmul");
            }
        }
    }


}