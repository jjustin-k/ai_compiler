#include<string>
#include "./ir/graph.hpp"

class CodeGen{

    private:
        std::string output_path;
        void writeToFile(std::string data, bool append);
        //start : int i = 0, end : i < size, change : i++, statement  : int j = i
        std::string writeForLoop(std::string start, std::string end, std::string change, std::string statement);
        void write_function(std::string return_type, std::string name, std::string parameters, std::string body);
        void generateOperations(Graph& graph);
        void generateConstants(Graph& graph);

    public:
        CodeGen(std::string output_path);
        void generateCode(Graph& graph);
        
};