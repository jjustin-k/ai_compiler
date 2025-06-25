#include "../include/codegen/sub_emitter.hpp"

void SubEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    int loop_size = sizes[0];
    std::string body =
        writeForLoop("int i = 0", "i < " + std::to_string(loop_size), "i++", "out[i] = a[i] - b[i];");
    write_function("void", "add", "float* out, float* a, float* b", body);
}

void SubEmitter::emitInvocation(std::ostream &out, Node *node, std::unordered_set<std::string> &defined_vars,
                                int general_size) {
    if (!defined_vars.count(node->name)) {
        out << "\nfloat " << node->name << "[" << general_size << "];\n";
        defined_vars.insert(node->name);
    }
    out << "\n add(" << node->name << ", " << node->input[0]->name << ", " << node->input[1]->name << ");\n";
}

std::string SubEmitter::getOpName() const { return "sub"; }