#include<iostream>

#include"ir/tensor.hpp"

int main(){
    std::cout << "Hello World" << std::endl;

    std::vector<int> shape = {2, 3, 2, 2}; // 2 batches, 3 channels, 2x2 spatial size
    std::vector<float> data = {
        // Batch 0
        // Channel 0
        1, 2,
        3, 4,
        // Channel 1
        5, 6,
        7, 8,
        // Channel 2
        9, 10,
        11, 12,

        // Batch 1
        // Channel 0
        13, 14,
        15, 16,
        // Channel 1
        17, 18,
        19, 20,
        // Channel 2
        21, 22,
        23, 24
    };


    Tensor tensor(data, shape);

    tensor.print();


    return 0;
}