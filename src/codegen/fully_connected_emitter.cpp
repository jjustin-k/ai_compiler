#include "../include/codegen/fully_connected_emitter.hpp"

void FullyConnectedEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    std::string n_loop = writeForLoop("int k = 0", "k < n", "k++", "sum += a[i * n + k] * b[k * p + j];");
    std::string inner_loop =
        writeForLoop("int j = 0", "j < p", "j++",
                     "float sum = 0.0f;\n" + n_loop + "\nout[i * p + j] = sum + c[i*p + j];\n");
    std::string body = writeForLoop("int i = 0", "i < m", "i++", inner_loop);
    write_function("void", "fully_connected", "float* out, float* a, float* b, float* c, int m, int n, int p",
                   body);
}

void FullyConnectedEmitter::emitInvocation(std::ostream &out, Node *node,
                                           std::unordered_set<std::string> &defined_vars, int general_size) {
    if (!defined_vars.count(node->name)) {
        general_size = node->shape[0] * node->shape[1];

        // this is why I need to add shape to node
        out << "\nfloat " << node->name << "[" << std::to_string(general_size) << "];\n";
        defined_vars.insert(node->name);
    }

    out << "int " << node->name << "_m = " << node->shape[0] << ";\nint " << node->name
        << "_n = " << node->shape[1] << ";\nint " << node->name << "_p = " << node->shape[2] << ";\n";

    out << "\n fully_connected(" << node->name << ", " << node->input[0]->name << ", " << node->input[1]->name
        << ", " << node->input[2]->name << ", " << node->name << "_m, " << node->name << "_n, " << node->name
        << "_p);\n";
}

std::string FullyConnectedEmitter::getOpName() const { return "fully_connected"; }