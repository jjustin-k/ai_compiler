#include "../include/codegen/maxpool_emitter.hpp"
#include "../include/utils/logger.hpp"

void MaxPoolEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    std::string body = "\nint ix = ox * stride + kx;\nint iy = "
                       "oy * stride + ky;\n"
                       "float val = a[iy * width + ix];\n"
                       "if (val > max_val) {\n"
                       "max_val = val;\n"
                       "};";
    std::string kx_loop = writeForLoop("int kx = 0", "kx < pool_size", "kx++", body);
    std::string ky_loop = writeForLoop("int ky = 0", "ky < pool_size", "ky++", kx_loop);
    std::string ox_loop =
        writeForLoop("int ox = 0", "ox < out_w", "ox++",
                     "\nfloat max_val = -1e9;\n" + ky_loop + "\nout[oy * out_w + ox] = max_val;\n");
    std::string oy_loop = writeForLoop("int oy = 0", "oy < out_h", "oy++", ox_loop);

    write_function("void", "maxpool2d",
                   " float *out, float *a, int width, int height, int "
                   "pool_size, int stride",
                   "\nint out_w = (width - pool_size) / stride + 1;"
                   "\nint out_h = (height - pool_size) / stride + 1;\n" +
                       oy_loop);
}

void MaxPoolEmitter::emitInvocation(std::ostream &out, Node *node,
                                    std::unordered_set<std::string> &defined_vars, int general_size) {

    if (!defined_vars.count(node->name)) {
        // this is why I need to add shape to node

        out << "\nfloat " << node->name << "[" << std::to_string(general_size) << "];\n";
        defined_vars.insert(node->name);
    }
    std::cout << node->input[0]->shape.size() << std::endl;
    out << "\n maxpool2d(" << node->name << ", " << node->input[0]->name << ", " << node->input[0]->shape[0]
        << ", " << node->input[0]->shape[1] << ", 2, 2);\n";
    globalLogger.info("Maxpool emitted");
}

std::string MaxPoolEmitter::getOpName() const { return "maxpool"; }