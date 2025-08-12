#include "../include/codegen/add_emitter.hpp"
#include <iostream>
void AddEmitter::emitFunctionDefinition(std::vector<int> &sizes) {

    int broad = sizes[0];
    std::string w_loop = "";
    std::string h_loop = "";
    std::string c_loop = "";

    if (broad == 0 || broad == 3) {
        w_loop = writeForLoop("int w = 0", "w < W", "w++",
                              "int idx = ((n * C + c) * H + h) * W + w;\nout[idx] += bias[idx];");
    } else {
        w_loop = writeForLoop("int w = 0", "w < W", "w++",
                              "int idx = ((n * C + c) * H + h) * W + w;\nout[idx] += b;");
    }
    if (broad == 2) {
        h_loop = writeForLoop("int h = 0", "h < H", "h++", "float b = bias[c];\n" + w_loop);
    } else {
        h_loop = writeForLoop("int h = 0", "h < H", "h++", w_loop);
    }

    if (broad == 1) {
        c_loop = writeForLoop("int c = 0", "c < C", "c++", "float b = bias[c];\n" + h_loop);
    } else {
        c_loop = writeForLoop("int c = 0", "c < C", "c++", h_loop);
    }

    std::string n_loop = writeForLoop("int n = 0", "n < N", "n++", c_loop);
    write_function("void", "add_" + std::to_string(broad),
                   type + "* out, " + type + "* bias, " + "int N, int C, int H, int W", n_loop);
}

void AddEmitter::emitInvocation(std::ostream &out, Node *node, std::unordered_set<std::string> &defined_vars,
                                int general_size) {

    int broad = broadcast_val(node->input[0]->shape, node->input[1]->shape);

    if (!defined_vars.count(node->name)) {
        out << "\n" << type << "* " << node->name << " = " << node->input[0]->name << ";\n";
        defined_vars.insert(node->name);
    }
    int dims[] = {1, 1, 1, 1};
    int r = 0;
    for (int i = node->input[0]->shape.size() - 1; i >= 0; i--) {

        dims[3 - r] = node->input[0]->shape[i];

        r++;
    }

    out << "\n add_" << broad << "(" << node->name << ", " << node->input[1]->name << ", " << dims[0] << ", "
        << dims[1] << ", " << dims[2] << ", " << dims[3] << ");\n";
}

std::string AddEmitter::getOpName() const { return "add"; }
