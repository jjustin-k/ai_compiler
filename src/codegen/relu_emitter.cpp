#include "../include/codegen/relu_emitter.hpp"

void ReLUEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    std::string body = writeForLoop("int i = 0", "i < " + std::to_string(sizes[0]), "i++",
                                    "a[i] = (a[i] > 0.0f) ? a[i] : 0.0f;");
    write_function("void", "relu", "float* a", body);
}

void ReLUEmitter::emitInvocation(std::ostream &out, Node *node, std::unordered_set<std::string> &defined_vars,
                                 int general_size) {
    out << "\n relu(" << node->input[0]->name << ");\n";
    node->name = node->input[0]->name;
}

std::string ReLUEmitter::getOpName() const { return "relu"; }