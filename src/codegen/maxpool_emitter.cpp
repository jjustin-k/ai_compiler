#include "../include/codegen/maxpool_emitter.hpp"
#include "../include/utils/logger.hpp"

void MaxPoolEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    std::string body = "\nint ix = ox * stride_w + kx - pad_left;\nint iy = "
                       "oy * stride_h + ky - pad_top;\n"
                       "if (iy >= 0 && iy < height && ix >= 0 && ix < width) {\n"
                       "int in_idx = ((n * C + c) * height + iy) * width + ix;\n" +
                       type +
                       " val = a[in_idx];\n"
                       "if (val > max_val) {\n"
                       "max_val = val;\n"
                       "};"
                       "}";
    std::string kx_loop = writeForLoop("int kx = 0", "kx < kernel_w", "kx++", body);
    std::string ky_loop = writeForLoop("int ky = 0", "ky < kernel_h", "ky++", kx_loop);
    std::string ox_loop = writeForLoop("int ox = 0", "ox < out_w", "ox++",
                                       type + "\n max_val = -180;\n" + ky_loop +
                                           "\nout[((n * C + c) * out_h + oy) * out_w + ox] = max_val;\n");
    std::string oy_loop = writeForLoop("int oy = 0", "oy < out_h", "oy++", ox_loop);
    std::string c_loop = writeForLoop("int c = 0", "c < C", "c++", oy_loop);
    std::string n_loop = writeForLoop("int n = 0", "n < N", "n++", c_loop);

    // Just took out out_w, out_h
    write_function(
        "void", "maxpool",
        type + " *out, " + type +
            " *a, int N, int C, int width, int height, int "
            "kernel_w, int kernel_h, int pad_top, int pad_left, int pad_bottom, int pad_right, int "
            "stride_w, int stride_h",
        "\nint out_w = (width - kernel_w + pad_right + pad_left) / stride_w + 1;"
        "\nint out_h = (height - kernel_h + pad_top + pad_bottom) / stride_h + 1;\n" +
            n_loop);
}

void MaxPoolEmitter::emitInvocation(std::ostream &out, Node *node,
                                    std::unordered_set<std::string> &defined_vars, int general_size) {

    if (!defined_vars.count(node->name)) {
        // this is why I need to add shape to node

        out << "\n " << type << " " << node->name << "[" << std::to_string(general_size) << "];\n";
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

    out << "\n maxpool(" << node->name << ", " << node->input[0]->name << ", " << node->input[0]->shape[0]
        << ", " << node->input[0]->shape[1] << ", " << node->input[0]->shape[2] << ", "
        << node->input[0]->shape[3] << ", " << pt << ", " << pl << ", " << pb << ", " << pr << ", " << kw
        << ", " << kh << "," << sw << ", " << sh << ");\n";
    globalLogger.info("Maxpool emitted");
}

std::string MaxPoolEmitter::getOpName() const { return "maxpool"; }