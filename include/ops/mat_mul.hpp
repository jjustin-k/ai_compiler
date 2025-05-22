#pragma once
#include "operation.hpp"

class MatMul : public Operation{
    private:
        bool compareVectorsAndIncr(std::vector<int>& v1, std::vector<int>& v2);
        size_t indexBatch(std::vector<int>& batch, std::vector<int>& strides);
    public:
        void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) override;
};