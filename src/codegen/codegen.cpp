#include "../include/codegen/codegen.hpp"
#include <fstream>
#include <sstream>

CodeGen::CodeGen(std::string output_path)
{
    this->output_path = output_path;

}

void CodeGen::writeToFile(std::string data)
{
    std::ofstream file(this->output_path, std::ios::app); // Open in append mode

    if (file.is_open()) {
        file << data;
        file.close();
    } 

}

void CodeGen::writeForLoop(std::string start, std::string end, std::string change, std::string statement)
{
    std::ostringstream loop_stream;
    loop_stream << "for(" << start << "; " << end << "; " << change << "){\n" <<
                statement << "}\n";

    std::string loop = loop_stream.str();
    writeToFile(loop);
}