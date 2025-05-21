#pragma once
#include"operation.hpp"

class Divide : Operation{
    public:
        void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) override;
};