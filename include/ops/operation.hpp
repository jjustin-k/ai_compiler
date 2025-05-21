#pragma once
#include "./ir/tensor.hpp"

class Operation{

    public:
        bool validOperation(Tensor &aTensor1, Tensor &aTensor2);
        virtual void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) = 0;
        virtual void operate(Tensor &aTensor) = 0; // inplace operation on a singular tensor
    
};