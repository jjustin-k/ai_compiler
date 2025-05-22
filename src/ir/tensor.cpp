
#include<iostream>
#include "../include/ir/tensor.hpp"


Tensor::Tensor(){}

Tensor::Tensor(std::vector<float> data, std::vector<int> shape)
{
    this->data = data;
    this->shape = shape;
    size = data.size();
    /*
    strides = std::vector<int>(data.size()); //assuming 4d for now
    strides[0] = this->shape[1] * this->shape[2] * this->shape[3];
    strides[1] = this->shape[2] * this->shape[3];
    strides[2] = this->shape[3];
    strides[3] = 1;
    */
    
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
    size = data.size();
    if(!this->data.empty() && !this->shape.empty()){
        valid_tensor = true;
    }
}

std::vector<int> Tensor::getShape()
{
    return this->shape;
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
