#include "../include/codegen/conv_emitter.hpp"
#include "../include/utils/logger.hpp"

void ConvEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    std::string body = "int in_h = oy * kernel_h + kh;\n"
                       "int in_w = ox * kernel_w + kw;\n"

                       "float in_val = a[in_h * width + in_w];\n"
                       "float weight_val = b[oc * (C * kernel_h * kernel_w) + ic * (kernel_h * kernel_w) + "
                       "kh * kernel_h + kw];\n"

        ;
    std::string kx_loop = writeForLoop("int kw = 0", "kw < kernel_w", "kw++", body);
    std::string ky_loop = writeForLoop("int kh = 0", "kh < kernel_h", "kh++", kx_loop);
    std::string ic_loop = writeForLoop("int ic = 0", "ic < C", "ic++", ky_loop);
    std::string ox_loop = writeForLoop("int ox = 0", "ox < width", "ox++",
                                       "float sum = 0.0f;\n" + ic_loop +
                                           "\nout[oc * height * width + oy * width + ox] = sum;\n");
    std::string oy_loop = writeForLoop("int oy = 0", "oy < height", "oy++", ox_loop);
    std::string oc_loop = writeForLoop("int oc = 0", "oc < out_c", "oc++", oy_loop);

    write_function("void", "conv",
                   " float *out, float *a, float *b, int out_c, int N, int C, int width, int height, int "
                   "kernel_w, int kernel_h, int pad_top, int pad_left, int pad_bottom, int pad_right, int "
                   "stride_w, int stride_h",

                   oc_loop);
}

void ConvEmitter::emitInvocation(std::ostream &out, Node *node, std::unordered_set<std::string> &defined_vars,
                                 int general_size) {
    if (!defined_vars.count(node->name)) {
        // this is why I need to add shape to node

        out << "\nfloat " << node->name << "[" << std::to_string(general_size) << "];\n";
        defined_vars.insert(node->name);
    }
    std::cout << node->input[0]->shape.size() << std::endl;

    int pt = 0;
    int pl = 0;
    int pb = 0;
    int pr = 0;
    int kh = 0;
    int kw = 0;
    int sh = 1;
    int sw = 1;
    if (node->attributes.contains("pads")) {
        pt = node->attributes["pads"][0];
        pl = node->attributes["pads"][1];
        pb = node->attributes["pads"][2];
        pr = node->attributes["pads"][3];
    }

    if (node->attributes.contains("kernel_shape")) {
        kh = node->attributes["kernel_shape"][0];
        kw = node->attributes["kernel_shape"][1];
    }

    if (node->attributes.contains("strides")) {
        sh = node->attributes["strides"][0];
        sw = node->attributes["strides"][1];
    }

    out << "\n conv(" << node->name << ", " << node->input[0]->name << ", " << node->input[1]->name << ", "
        << node->input[1]->shape[1] << ", " << node->input[0]->shape[0] << ", " << node->input[0]->shape[1]
        << ", " << node->input[0]->shape[2] << ", " << node->input[0]->shape[3] << ", " << pt << ", " << pl
        << ", " << pb << ", " << pr << ", " << kw << ", " << kh << "," << sw << ", " << sh << ");\n";
    globalLogger.info("Maxpool emitted");
}

std::string ConvEmitter::getOpName() const { return "conv"; }