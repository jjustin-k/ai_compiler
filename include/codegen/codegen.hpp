#include<string>
#include "./ir/graph.hpp"

class CodeGen{

    private:
        int general_size;
        std::string output_path;
        void writeToFile(std::string data, bool append);
        void writeTestInput(Graph& graph);
        std::string generateOperations(Graph& graph);
        void generateConstants(Graph& graph);
        void generateMain(Graph& graph, std::string body);

    public:
        CodeGen(std::string output_path);
        void generateCode(Graph& graph);
        
};
