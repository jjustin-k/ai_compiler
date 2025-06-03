#include"../include/ops/relu.hpp"
#include <vector>
#include <algorithm>

void ReLU::operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location)
{
    //In this only aTensor1 is used, pass in null for the other values
    std::vector<float> data = aTensor1.getDataV();
    for(int i = 0; i < aTensor1.size; i++){
        data[i] = std::max(data[i], 0.0f);
    }
}