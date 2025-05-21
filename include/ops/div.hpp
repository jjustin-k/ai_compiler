#pragma once
#include"operation.hpp"

class Divide : Operation{
    private:
        void divide(Tensor &t1, Tensor &t2, Tensor &result_location);
    public:
        void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) override;
};