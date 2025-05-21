#pragma once
#include"operation.hpp"

class Subtract : Operation{
    private:
        void subtract(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location);
    public:
        void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) override;
};