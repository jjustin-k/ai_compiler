#include<iostream>
#include "element.hpp"

bool ElementWiseOps::validOperation(Tensor &t1, Tensor &t2)
{
    std::vector<int> shape1 = t1.getShape();
    std::vector<int> shape2 = t2.getShape();

    for (size_t i = 0; i < shape1.size(); ++i) {
        if(shape1[i] != shape2[i]){
            return false;
        }
    }

    return true;
}


Tensor ElementWiseOps::add(Tensor &t1, Tensor &t2)
{

    if(!validOperation(t1, t2)){
        throw std::runtime_error("Tensors have different shapes");
    }

    float* data1 = t1.getDataA();
    float* data2 = t2.getDataA();

    std::vector<float> new_data(t1.size);

    // Add threads later
    for(size_t i = 0; i < t1.size; i++){
        new_data[i] = data1[i] + data2[i];
    }
    
    return Tensor(new_data, t1.getShape());
}


Tensor ElementWiseOps::subtract(Tensor &t1, Tensor &t2)
{
    if(!validOperation(t1, t2)){
        throw std::runtime_error("Tensors have different shapes");
    }

    float* data1 = t1.getDataA();
    float* data2 = t2.getDataA();

    std::vector<float> new_data(t1.size);

    // Add threads later
    for(size_t i = 0; i < t1.size; i++){
        new_data[i] = data1[i] - data2[i];
    }
    
    return Tensor(new_data, t1.getShape());
}


Tensor ElementWiseOps::divide(Tensor &t1, Tensor &t2)
{
    if(!validOperation(t1, t2)){
        throw std::runtime_error("Tensors have different shapes");
    }

    float* data1 = t1.getDataA();
    float* data2 = t2.getDataA();

    std::vector<float> new_data(t1.size);

    // Add threads later
    for(size_t i = 0; i < t1.size; i++){
        const float epsilon = 1e-7f;
        new_data[i] = data1[i] / (data2[i] + epsilon); //saving division by 0
    }
    
    return Tensor(new_data, t1.getShape());
}


Tensor ElementWiseOps::mutiply(Tensor &t1, Tensor &t2)
{
    if(!validOperation(t1, t2)){
        throw std::runtime_error("Tensors have different shapes");
    }

    float* data1 = t1.getDataA();
    float* data2 = t2.getDataA();

    std::vector<float> new_data(t1.size);

    // Add threads later
    for(size_t i = 0; i < t1.size; i++){
        new_data[i] = data1[i] * data2[i];
    }
    
    return Tensor(new_data, t1.getShape());
}
