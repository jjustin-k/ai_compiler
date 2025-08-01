#pragma once
#include<vector>
#include <memory>
#include <typeindex>


class Tensor
{
    private:
        bool valid_tensor;
        std::vector<float> data;
        std::vector<int> shape; //size of shape is how many dimensions
        void calcAndSetStrides();
    public:
        std::vector<int> strides;
        size_t size;
        Tensor();
        Tensor(std::vector<float> data, std::vector<int> shape);
        void setShape(std::vector<int> shape);
        void setData(std::vector<float> data);
        std::vector<int> getShape();
        std::vector<float> &getDataV();
        float* getDataA();
        void reshape(std::vector<int> &new_shape);
        void print(int dim, int offset, std::string indent);

};

class QuantizedTensor
{
    private:
        bool valid_tensor;
        std::vector<float> data;
        std::vector<int> shape; //size of shape is how many dimensions
        void calcAndSetStrides();
    public:
        std::vector<int> strides;
        size_t size;
        QuantizedTensor();
        QuantizedTensor(std::vector<int8_t> data, std::vector<int> shape);
        void setShape(std::vector<int> shape);
        void setData(std::vector<int8_t> data);
        std::vector<int> getShape();
        std::vector<int8_t> &getDataV();
        int8_t* getDataA();
        void reshape(std::vector<int> &new_shape);
        void print(int dim, int offset, std::string indent);

};


