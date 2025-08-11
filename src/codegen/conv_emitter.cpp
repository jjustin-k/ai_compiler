#include "../include/codegen/conv_emitter.hpp"
#include "../include/utils/logger.hpp"

void ConvEmitter::emitFunctionDefinition(std::vector<int> &sizes) {
    std::string body = "int in_h = oy * stride_h - pad_top + kh;\n"
                       "int in_w = ox * stride_w - pad_left + kw;\n"
                       "if (in_h >= 0 && in_h < height && in_w >= 0 && in_w < width) {\n"
                       "  int in_idx = ((n * C + ic) * height + in_h) * width + in_w;\n"
                       "  float in_val = a[in_idx];\n"
                       "  int w_idx = ((oc * C + ic) * kernel_h + kh) * kernel_w + kw;\n"
                       "  float weight_val = b[w_idx];\n"
                       "  sum += weight_val * in_val;\n"
                       "}\n";

    std::string kx_loop = writeForLoop("int kw = 0", "kw < kernel_w", "kw++", body);
    std::string ky_loop = writeForLoop("int kh = 0", "kh < kernel_h", "kh++", kx_loop);
    std::string ic_loop = writeForLoop("int ic = 0", "ic < C", "ic++", ky_loop);

    // Calculate output height and width inside the function:
    std::string out_size_calc = "int out_h = (height + pad_top + pad_bottom - kernel_h) / stride_h + 1;\n"
                                "int out_w = (width + pad_left + pad_right - kernel_w) / stride_w + 1;\n";

    // innermost loops over output height and width:
    std::string ox_loop = writeForLoop("int ox = 0", "ox < out_w", "ox++",
                                       type + " sum = 0;\n" + ic_loop +
                                           "\nint out_idx = ((n * out_c + oc) * out_h + oy) * out_w + ox;\n"
                                           "out[out_idx] = sum;\n");
    std::string oy_loop = writeForLoop("int oy = 0", "oy < out_h", "oy++", ox_loop);
    std::string oc_loop = writeForLoop("int oc = 0", "oc < out_c", "oc++", oy_loop);
    std::string n_loop = writeForLoop("int n = 0", "n < N", "n++", oc_loop);

    // Compose full function body:
    std::string full_body = out_size_calc + n_loop;

    write_function(
        "void", "conv",
        " " + type + " *out, " + type + " *a, " + type +
            " *b, "
            "int out_c, int N, int C, int width, int height, int "
            "kernel_w, int kernel_h, int pad_top, int pad_left, int pad_bottom, int pad_right, int "
            "stride_w, int stride_h",
        full_body);
}

#include <algorithm>
#include <cmath>
#include <iostream>
#include <tuple>

std::tuple<int, int, int, int> same_upper_padding(int input_h, int input_w, int kernel_h, int kernel_w,
                                                  int stride_h = 1, int stride_w = 1, int dilation_h = 1,
                                                  int dilation_w = 1) {
    int effective_kh = (kernel_h - 1) * dilation_h + 1;
    int effective_kw = (kernel_w - 1) * dilation_w + 1;

    int out_h = static_cast<int>(std::ceil((float)input_h / stride_h));
    int out_w = static_cast<int>(std::ceil((float)input_w / stride_w));

    int pad_h_total = std::max((out_h - 1) * stride_h + effective_kh - input_h, 0);
    int pad_w_total = std::max((out_w - 1) * stride_w + effective_kw - input_w, 0);

    int pad_top = pad_h_total / 2;
    int pad_bottom = pad_h_total - pad_top;
    int pad_left = pad_w_total / 2;
    int pad_right = pad_w_total - pad_left;

    std::cout << pad_top << " " << pad_bottom << " " << pad_left << " " << pad_right << " " << std::endl;
    return {pad_top, pad_left, pad_bottom, pad_right};
}

void ConvEmitter::emitInvocation(std::ostream &out, Node *node, std::unordered_set<std::string> &defined_vars,
                                 int general_size) {
    if (!defined_vars.count(node->name)) {
        // this is why I need to add shape to node

        out << "\n" << type << " " << node->name << "[" << std::to_string(general_size) << "];\n";
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

    if (node->attributes.contains("kernel_shape")) {
        kh = node->attributes["kernel_shape"][0];
        kw = node->attributes["kernel_shape"][1];
    }

    if (node->attributes.contains("strides")) {
        sh = node->attributes["strides"][0];
        sw = node->attributes["strides"][1];
    }
    if (node->attributes.contains("pads")) {
        pt = node->attributes["pads"][0];
        pl = node->attributes["pads"][1];
        pb = node->attributes["pads"][2];
        pr = node->attributes["pads"][3];
    } else if (node->attributes["auto_pad"] == "SAME_UPPER") {

        auto [p_t, p_l, p_b, p_r] =
            same_upper_padding(node->input[0]->shape[0], node->input[0]->shape[0], kh, kw, sh, sw, 1, 1);
        pt = p_t;
        pl = p_l;
        pb = p_b;
        pr = p_r;
    }

    out << "\n conv(" << node->name << ", " << node->input[0]->name << ", " << node->input[1]->name << ", "
        << node->input[1]->shape[0] << ", " << node->input[0]->shape[0] << ", " << node->input[0]->shape[1]
        << ", " << node->input[0]->shape[2] << ", " << node->input[0]->shape[3] << ", " << kw << ", " << kh
        << "," << pt << ", " << pl << ", " << pb << ", " << pr << ", " << sw << ", " << sh << ");\n";
    std::cout << node->input[1]->shape[0] << ", " << node->input[1]->shape[1] << ", "
              << node->input[1]->shape[2] << ", " << node->input[1]->shape[3] << std::endl;
}

std::string ConvEmitter::getOpName() const { return "conv"; }