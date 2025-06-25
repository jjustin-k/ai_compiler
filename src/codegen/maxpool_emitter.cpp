#include "../include/codegen/maxpool_emitter.hpp"

void MaxPoolEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    std::string n_loop = writeForLoop("int k = 0", "k < n", "k++", "sum += a[i * n + k] * b[k * p + j];");
    std::string inner_loop = writeForLoop("int j = 0", "j < p", "j++",
                                          "float sum = 0.0f;\n" + n_loop + "\nout[i * p + j] = sum;\n");
    std::string body = writeForLoop("int i = 0", "i < m", "i++", inner_loop);
    write_function("void", "matmul2d", "float* out, float* a, float* b, int m, int n, int p", body);
}

void MaxPoolEmitter::emitInvocation(std::ostream &out, Node *node,
                                    std::unordered_set<std::string> &defined_vars, int general_size) {
    if (!defined_vars.count(node->name)) {
        // this is why I need to add shape to node
        out << "\nfloat " << node->name << "[" << std::to_string(general_size) << "];\n";
        defined_vars.insert(node->name);
    }
    out << "\n maxpool2d(" << node->name << ", " << node->input[0]->name << ", " << node->input[0]->shape[0]
        << ", " << node->input[0]->shape[1] << ", 2, 2);\n";
}

std::string MaxPoolEmitter::getOpName() const { return "maxpool"; }