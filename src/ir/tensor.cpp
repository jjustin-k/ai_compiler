#include "ir/tensor.hpp"
#include<iostream>
#include "tensor.hpp"


Tensor::Tensor(){}

Tensor::Tensor(std::vector<float> data, std::vector<int> shape)
{
    this->data = data;
    this->shape = shape;
    size = data.size();
    strides = std::vector<int>(data.size()); //assuming 4d for now
    strides[0] = this->shape[1] * this->shape[2] * this->shape[3];
    strides[1] = this->shape[2] * this->shape[3];
    strides[2] = this->shape[3];
    strides[3] = 1;
}

void Tensor::setShape(std::vector<int> shape)
{
    this->shape = shape;
    if(!this->data.empty() && !this->shape.empty()){
        valid_tensor = true;
    }
}

void Tensor::setData(std::vector<float> data)
{
    this->data = data;
    if(!this->data.empty() && !this->shape.empty()){
        valid_tensor = true;
    }
}

std::vector<int> Tensor::getShape()
{
    return std::vector<int>();
}


std::vector<float> Tensor::getDataV()
{
    return std::vector<float>();
}


float *Tensor::getDataA()
{
    return this->data.data();
}


void Tensor::reshape(std::vector<int> &new_shape)
{

}

void Tensor::print()
{
    for(size_t batch = 0; batch < shape[0]; batch++){
        std::cout << "Batch : " << batch << std::endl;
        for(size_t channel = 0; channel < shape[1]; channel ++){
            std::cout << "Channel : " << channel << std::endl;
            for(size_t row = 0; row < shape[2]; row ++){
                for(size_t col = 0; col < shape[3]; col++){
                    std::cout << data[batch*strides[0] + channel*strides[1] + row*strides[2] + col] << " ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
        std::cout << "--------------------------\n--------------------------\n";
    }
}
