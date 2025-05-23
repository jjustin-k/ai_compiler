
#include<iostream>
#include "../include/ir/tensor.hpp"

Tensor::Tensor(){}

Tensor::Tensor(std::vector<float> data, std::vector<int> shape)
{
    this->data = data;
    this->shape = shape;
    size = data.size();
    
    calcAndSetStrides();
}


void Tensor::calcAndSetStrides()
{
    strides = std::vector<int>(shape.size()); 

    for(size_t i = 0; i < shape.size(); i++){
        for(size_t j = i ; j < shape.size(); j++){
            if(j != i ){
                strides[i] *= this->shape[j];
            }
            else{
                strides[i] = 1;
            }
        }
    }   
}


void Tensor::setShape(std::vector<int> shape)
{
    this->shape = shape;
    if(!this->data.empty() && !this->shape.empty()){
        valid_tensor = true;
        calcAndSetStrides();
    }
}


void Tensor::setData(std::vector<float> data)
{
    this->data = data;
    size = data.size();
    if(!this->data.empty() && !this->shape.empty()){
        valid_tensor = true;
        calcAndSetStrides();
    }
}

std::vector<int> Tensor::getShape()
{
    return this->shape;
}


std::vector<float> Tensor::getDataV()
{
    return this->data;
}


float *Tensor::getDataA()
{
    return this->data.data();
}


void Tensor::reshape(std::vector<int> &new_shape)
{

}

void Tensor::print(int dim, int offset, std::string indent) {
    int dimSize = shape[dim];
    int stride = 1;
    for (int i = dim + 1; i < shape.size(); ++i) {
        stride *= shape[i];
    }

    std::cout << indent << "[";
    for (int i = 0; i < dimSize; ++i) {
        int index = offset + i * stride;

        if (dim == shape.size() - 1) {
            // Leaf value
            std::cout << data[index];
        } else {
            // Nested dimension
            std::cout << "\n";
            print(dim + 1, index, indent + "  ");
        }

        if (i != dimSize - 1)
            std::cout << ", ";
    }

    if (dim != shape.size() - 1)
        std::cout << "\n" << indent;

    std::cout << "]";
    if (dim == 0)
        std::cout << "\n";

}
