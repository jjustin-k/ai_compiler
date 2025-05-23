#pragma once
#include"operation.hpp"

class Multiply : public Operation{
    public:
        void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) override;
        
};