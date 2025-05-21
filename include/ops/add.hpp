#pragma once
#include"operation.hpp"

class Add : Operation{
    private:
        void add(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location);
    public:
        void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) override;
};