#pragma once
#include"tensor.h"

class ElementWiseOps{
    private:
        bool sameShape(tensor &t1, tensor &t2);
    public:
        tensor add(tensor &t1, tensor &t2);
        tensor subtract(tensor &t1, tensor &t2);
        tensor divide(tensor &t1, tensor &t2);
        tensor mutiply(tensor &t1, tensor &t2);
};