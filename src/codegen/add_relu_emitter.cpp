#include "../include/codegen/add_relu_emitter.hpp"

void AddReluEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    std::string body = writeForLoop("int i = 0", "i < " + std::to_string(sizes[0]), "i++",
                                    "out[i] = (a[i] + b[i] > 0.0f) ? (a[i] + b[i]) : "
                                    "0.0f;");
    write_function("void", "add_relu", "float* out, float* a, float* b", body);
}

void AddReluEmitter::emitInvocation(std::ostream &out, Node *node,
                                    std::unordered_set<std::string> &defined_vars, int general_size) {
    if (!defined_vars.count(node->name)) {
        out << "\nfloat " << node->name << "[" << general_size << "];\n";
        defined_vars.insert(node->name);
    }
    out << "\n add_relu(" << node->name << ", " << node->input[0]->name << ", " << node->input[1]->name
        << ");\n";
}

std::string AddReluEmitter::getOpName() const { return "add_relu"; }