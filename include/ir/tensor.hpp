#pragma once
#include<vector>

class Tensor
{
    private:
        std::vector<float> data;
        std::vector<int> shape;
        std::vector<int> strides;
    public:
        size_t size;
        Tensor(std::vector<float> data, std::vector<int> shape);
        std::vector<int> getShape();
        std::vector<float> getDataV();
        float* getDataA();
        void reshape(std::vector<int> &new_shape);
        void print();

};


