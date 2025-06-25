#pragma once
#include "emitter.hpp"

class FullyConnectedEmitter : public OpEmitter {
    public:
        void emitFunctionDefinition(std::vector<int> &sizes) override;
    
        void emitInvocation(std::ostream &out, Node *node,
                            std::unordered_set<std::string> &defined_vars, int general_size) override ;
    
        std::string getOpName() const override ;
};
    