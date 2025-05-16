#pragma once
#include"tensor.hpp"

class ElementWiseOps{
    private:
        bool validOperation(Tensor &t1, Tensor &t2);
    public:
        Tensor add(Tensor &t1, Tensor &t2);
        Tensor subtract(Tensor &t1, Tensor &t2);
        Tensor divide(Tensor &t1, Tensor &t2);
        Tensor mutiply(Tensor &t1, Tensor &t2);
};