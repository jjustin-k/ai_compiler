#pragma once
#include"operation.hpp"

class Multiply : Operation{
    private:
        void multiply(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location);
    public:
        void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) override;
};