#pragma once
#include <ostream>
#include"./ir/graph.hpp"
#include <unordered_set>
#include "./utils/broadcaster.hpp"

extern std::string type;

class OpEmitter {
        
    public:
        int broad = 0;

        std::string output_path = "../output/compiled_model.c";
        std::string writeForLoop(std::string start, std::string end, std::string change, std::string statement);
        void write_function(std::string return_type, std::string name, std::string parameters, std::string body);
        void writeToFile(std::string data, bool append);
        virtual void emitFunctionDefinition(std::vector<int> &sizes) = 0;
        virtual void emitInvocation(std::ostream &out, Node *node,
                                    std::unordered_set<std::string> &defined_vars, int general_size) = 0;
        virtual std::string getOpName() const = 0;
        virtual ~OpEmitter() = default;
    };

void set_type(std::string n_type);