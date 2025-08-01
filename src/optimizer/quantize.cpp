#include "../include/optimizer/quantize.hpp"
#include "../include/utils/logger.hpp"
#include <vector>

void Quantizer::quantize(Node *node, bool per_channel) {
    if (node->name == "Input3") {
        return;
    }
    globalLogger.info("Quantizing " + node->name);
    if (per_channel && node->shape.size() == 4) {
        // only supporting conv for now, must be 4d?

        std::vector<int> shape = node->input[1]->shape;
        std::cout << node->input[1]->shape.size() << std::endl;
        std::cout << node->name << std::endl;
        std::vector<float> data = node->input[1]->tensor->getDataV();
        std::vector<int8_t> quant_data(data.size());

        int out_channels = shape[0];
        int in_channels = shape[1];
        int height = shape[2];
        int width = shape[3];
        globalLogger.debug("Here");
        for (int oc = 0; oc < out_channels; oc++) {
            float min = 0.0;
            float max = 0.0;

            for (int ic = 0; ic < in_channels; ic++) {
                for (int h = 0; h < height; h++) {
                    for (int w = 0; w < width; w++) {
                        int idx = NCHW_INDEX(oc, ic, h, w, in_channels, height, width);

                        float current = data[idx];
                        min = current < min ? current : min;
                        max = current > max ? current : max;
                    }
                }
            }
            float scale = (max - min) / 255.0f;
            if (scale == 0)
                scale = 1e-8f; // avoid division by zero

            int zp = std::round(-128.0f - min / scale);
            zp = std::clamp(zp, -128, 127);

            for (int ic = 0; ic < in_channels; ic++) {
                for (int h = 0; h < height; h++) {
                    for (int w = 0; w < width; w++) {
                        int idx = NCHW_INDEX(oc, ic, h, w, in_channels, height, width);
                        float current = data[idx];
                        int qval = std::round(current / scale) + zp;
                        qval = std::clamp(qval, -128, 127);
                        quant_data[idx] = static_cast<int8_t>(qval);
                    }
                }
            }
        }
        node->input[1]->quant_data = quant_data;
    } else {
        std::vector<int> shape = node->shape;
        std::vector<float> data = node->tensor->getDataV();
        std::vector<int8_t> quant_data(data.size());

        int out_channels = 1;
        int in_channels = 1;
        int height = shape[shape.size() - 2];
        int width = shape[shape.size() - 1];
        if (shape.size() == 4) {
            out_channels = shape[0];
            in_channels = shape[1];
            height = shape[2];
            width = shape[3];
        }

        globalLogger.debug("Here");
        float min = 0.0;
        float max = 0.0;
        for (int oc = 0; oc < out_channels; oc++) {

            for (int ic = 0; ic < in_channels; ic++) {

                for (int h = 0; h < height; h++) {
                    for (int w = 0; w < width; w++) {
                        int idx = NCHW_INDEX(oc, ic, h, w, in_channels, height, width);

                        float current = data[idx];

                        min = current < min ? current : min;
                        max = current > max ? current : max;
                    }
                }
            }

            float scale = (max - min) / 255.0f;
            if (scale == 0)
                scale = 1e-8f; // avoid division by zero

            int zp = std::round(-128.0f - min / scale);
            zp = std::clamp(zp, -128, 127);
            for (int ic = 0; ic < in_channels; ic++) {
                for (int h = 0; h < height; h++) {
                    for (int w = 0; w < width; w++) {
                        int idx = NCHW_INDEX(oc, ic, h, w, in_channels, height, width);
                        float current = data[idx];
                        int qval = std::round(current / scale) + zp;
                        qval = std::clamp(qval, -128, 127);
                        quant_data[idx] = static_cast<int8_t>(qval);
                    }
                }
            }
        }
        float scale = (max - min) / 255.0f;
        if (scale == 0)
            scale = 1e-8f; // avoid division by zero

        int zp = std::round(-128.0f - min / scale);
        zp = std::clamp(zp, -128, 127);
        for (int oc = 0; oc < out_channels; oc++) {
            for (int ic = 0; ic < in_channels; ic++) {
                for (int h = 0; h < height; h++) {
                    for (int w = 0; w < width; w++) {
                        int idx = NCHW_INDEX(oc, ic, h, w, in_channels, height, width);
                        float current = data[idx];
                        int qval = std::round(current / scale) + zp;
                        qval = std::clamp(qval, -128, 127);
                        quant_data[idx] = static_cast<int8_t>(qval);
                    }
                }
            }
        }

        node->quant_data = quant_data;
    }
}