#include"../include/ops/add.hpp"

void Add::operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location) 
{
    if(!Operation::validOperation(aTensor1, aTensor2)){
        throw std::runtime_error("Tensors have different shapes");
    }

    float* data1 = aTensor1.getDataA();
    float* data2 = aTensor2.getDataA();

    std::vector<float> new_data(aTensor1.size);

    // Add threads later
    for(size_t i = 0; i < aTensor1.size; i++){
        new_data[i] = data1[i] + data2[i];
    }
    
    result_location.setData(new_data);
    result_location.setShape(aTensor1.getShape());
  
}

