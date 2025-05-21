#include "../include/ops/operation.hpp"

bool Operation::validOperation(Tensor &aTensor1, Tensor &aTensor2)
{
    std::vector<int> shape1 = aTensor1.getShape();
    std::vector<int> shape2 = aTensor2.getShape();

    for (size_t i = 0; i < shape1.size(); ++i) {
        if(shape1[i] != shape2[i]){
            return false;
        }
    }

    return true;
}

