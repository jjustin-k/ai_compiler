#pragma once
#include "./ir/tensor.hpp"
#include <string>
class Operation{

    public:
        std::string op_name;
        virtual ~Operation() = default; //so deleting is normal
        bool validOperation(Tensor &aTensor1, Tensor &aTensor2);
        virtual void operate(Tensor &tensor1, Tensor &tensor2, Tensor &result_location) = 0;
       // virtual void operate(Tensor &aTensor) = 0; // inplace operation on a singular tensor
    
};