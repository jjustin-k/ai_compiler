#include "../include/codegen/reshape_emitter.hpp"
#include <iostream>
void ReshapeEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    // No function definition
    return;
}

void ReshapeEmitter::emitInvocation(std::ostream &out, Node *node,
                                    std::unordered_set<std::string> &defined_vars, int general_size) {
    std::cout << "HERE" << std::endl;
    if (!defined_vars.count(node->name)) {
        out << "\nfloat* " << node->name << " = " << node->input[0]->name << ";";
        defined_vars.insert(node->name);
    }
}

std::string ReshapeEmitter::getOpName() const { return "reshape"; }