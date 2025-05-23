#pragma once
#include"operation.hpp"

class MaxPooling : public Operation{
    private:
        std::vector<int> strides;
        std::vector<int> kernel_shape;
        std::vector<int> pads;
    public:
        MaxPooling(std::vector<int> strides, std::vector<int> pads);
        //First tensor is layer, second is the kernel
        void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) override;
};