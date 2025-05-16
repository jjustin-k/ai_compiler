#include <cassert>
#include"ir/tensor.hpp"
#include"ops/element.hpp"


void testAddition(Tensor &t1, Tensor &t2){
    //expected data

}

int main(){

    std::vector<int> shape1 = {2, 2, 2, 2};
    std::vector<float> data1 = {
        // Batch 0
        // Channel 0
        1, 2,
        3, 4,
        // Channel 1
        5, 6,
        7, 8,
        // Batch 1
        // Channel 0
        9, 10,
        11, 12,
        // Channel 1
        13, 14,
        15, 16
    };

    std::vector<float> data2 = {
        // Batch 0
        // Channel 0
        100, 200,
        300, 400,
        // Channel 1
        500, 600,
        700, 800,
    
        // Batch 1
        // Channel 0
        900, 1000,
        1100, 1200,
        // Channel 1
        1300, 1400,
        1500, 1600
    };

    std::vector<int> shape3 = {1, 2, 2, 2};
    std::vector<float> data3 = {
        // Batch 0
        // Channel 0
        7, 8,
        9, 10,
        // Channel 1
        11, 12,
        13, 14
    };

    Tensor t1(data1, shape1);
    Tensor t2(data2, shape1);
    Tensor t3(data3, shape3);
    
    return 1;
}