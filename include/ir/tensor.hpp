#pragma once
#include<vector>

class Tensor
{
    private:
        bool valid_tensor;
        std::vector<float> data;
        std::vector<int> shape;
        std::vector<int> strides;
    public:
        size_t size;
        Tensor();
        Tensor(std::vector<float> data, std::vector<int> shape);
        void setShape(std::vector<int> shape);
        void setData(std::vector<float> data);
        std::vector<int> getShape();
        std::vector<float> getDataV();
        float* getDataA();
        void reshape(std::vector<int> &new_shape);
        void print();

};


