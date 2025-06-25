#include<string>
#include "./ir/graph.hpp"

class CodeGen{

    private:
        int general_size;
        std::string output_path;
        void writeToFile(std::string data, bool append);
        //start : int i = 0, end : i < size, change : i++, statement  : int j = i
        void matmul(Node* node);
        std::string writeForLoop(std::string start, std::string end, std::string change, std::string statement);
        void writeTestInput(Graph& graph);
        void write_function(std::string return_type, std::string name, std::string parameters, std::string body);
        std::string generateOperations(Graph& graph);
        void generateConstants(Graph& graph);
        void generateMain(Graph& graph, std::string body);

    public:
        CodeGen(std::string output_path);
        void generateCode(Graph& graph);
        
};

/*
 for(int i = 0; i < general_size; i++){
    printf("%f ", add1[i]);
  }
  printf("\n");*/