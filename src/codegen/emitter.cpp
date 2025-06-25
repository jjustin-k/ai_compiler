#include "../include/codegen/emitter.hpp"
#include <fstream>
#include <sstream>

void OpEmitter::writeToFile(std::string data, bool append) {
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

std::string OpEmitter::writeForLoop(std::string start, std::string end, std::string change,
                                    std::string statement) {
    std::ostringstream loop_stream;
    loop_stream << "\nfor(" << start << "; " << end << "; " << change << "){\n    " << statement << "\n}\n";

    std::string loop = loop_stream.str();
    return loop;
}

void OpEmitter::write_function(std::string return_type, std::string name, std::string parameters,
                               std::string body) {

    std::ostringstream function_stream;
    function_stream << "\n"
                    << return_type << " " << name << "(" << parameters << "){\n    " << body << "\n}\n";

    std::string function = function_stream.str();
    writeToFile(function, true);
}