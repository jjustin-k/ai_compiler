#include "../include/ops/max_pooling.hpp"
#include <iostream>
/*
    All the following code is for a 2d layer, change to Nd later

*/

MaxPooling::MaxPooling(std::vector<int> strides, std::vector<int> pads) {
    this->strides = strides;
    // assuming 2d for now [top, left, bottom, right]
    this->pads = pads;
}

float findMax(std::vector<float> data) {
    float max_val = 0.0f;

    for (auto &val : data) {
        max_val = (val > max_val) ? val : max_val;
    }

    return max_val;
}

void scanTensor(Tensor &tensor, std::vector<int> &kernel_shape,
                Tensor &result_location) {
    int tensor_height = tensor.getShape()[0];
    int tensor_width = tensor.getShape()[1];

    int checks = result_location.getShape()[0] * result_location.getShape()[1];
    std::vector<float> data(kernel_shape[0] * kernel_shape[1]);
    std::vector<float> result(checks);
    // specific to 4x4 and 2x2 kernel

    for (size_t i = 0; i < checks; i++) {
        data[0] = tensor.getDataA()[i * kernel_shape[0]];
        data[1] = tensor.getDataA()[i * kernel_shape[0] + 1];
        data[2] = tensor.getDataA()[i * kernel_shape[0] + tensor_width];
        data[3] = tensor.getDataA()[i * kernel_shape[0] + tensor_width + 1];

        result[i] = findMax(data);
    }

    result_location.setData(result);
}

void MaxPooling::operate(Tensor &aTensor1, Tensor &aTensor2,
                         Tensor &result_location) // aTensor2 is the kernel
{

    std::vector<int> kernel_shape = aTensor2.getShape();
    // assuming 2d
    int kernel_height = kernel_shape[0];
    int kernel_width = kernel_shape[1];

    int tensor_height = aTensor1.getShape()[0];
    int tensor_width = aTensor1.getShape()[1];

    int pad_top = pads[0];
    int pad_bottom = pads[2];
    int pad_left = pads[1];
    int pad_right = pads[3];

    int stride_vert = strides[0];
    int stride_horz = strides[1];

    int out_height =
        floor((tensor_height + pad_top + pad_bottom - kernel_height) /
              stride_vert) +
        1;
    int out_width = floor((tensor_width + pad_left + pad_right - kernel_width) /
                          stride_horz) +
                    1;

    // potential bug, when this function finishes this may make shape null
    result_location.setShape({out_height, out_width});

    scanTensor(aTensor1, kernel_shape, result_location);
}