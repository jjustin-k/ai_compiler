#include"../include/ops/mat_mul.hpp"

#include<iostream>

bool MatMul::compareVectorsAndIncr(std::vector<int>& v1, std::vector<int>& v2){
    //assuming same length
    bool same = true;
    for(size_t i = 0; i < v1.size(); i++){
        if(v1[i] != v2[i]){
            v1[i] ++; 
            return false;
        }
    }

    return same;
}


size_t MatMul::indexBatch(std::vector<int>& batch, std::vector<int>& strides){
    size_t index = 0;
    for(size_t i = 0; i < batch.size(); i++){
        index += batch[i] * strides[i];
    }

    return index;
}


void MatMul::operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location)
{
   
    std::vector<int> shape_1 = aTensor1.getShape();
    std::vector<int> shape_2 = aTensor2.getShape();

    size_t dim_1 = shape_1.size();
    size_t dim_2 = shape_2.size();

    std::vector<int> result_shape(shape_1);

    result_shape[dim_1 - 1] = shape_2[dim_2 - 1];
    
    result_location.setShape(result_shape);

    //Implement broadcasting later
    if((dim_1 != dim_2) || shape_1[dim_1 - 1] != shape_2[dim_2 - 2]){
        throw std::logic_error("Tensors are not same shape");
    }

    // std::cout << data[batch*strides[0] + channel*strides[1] + row*strides[2] + col] << " ";
 
    std::vector<int> batch(dim_1 - 3); // taking all dims that arent the last two

    std::vector<float> result_data; //optimize by reserving space;

    size_t current = 0;
    do{ 
      
    /*
    can just pass in the shape since the loop in compareVectors
    since it just looks at the size of the first vector
    */
        for(size_t row = 0; row < shape_1[dim_1 - 2]; row++){ //shape 1's row dim
            for(size_t col = 0; col < shape_2[dim_2 - 1]; col++){ //shape 2's col dim

                //std::cout << col << std::endl;
                size_t index_tensor_1 = 0;
                size_t index_tensor_2 = 0;

                float sum = 0.0f;
                for(size_t shared = 0; shared < shape_1[dim_1 - 1]; shared++){
                   
                    //since broadcasting not implemented, indexBatch should return the same thing for both calls 
                    //std::cout << dim_1 -2 << "  " << aTensor1.strides.size() << std::endl;
                    index_tensor_1 = row * aTensor1.strides[dim_1 - 2] + shared * aTensor1.strides[dim_1 - 1] + indexBatch(batch, aTensor1.strides);
                    
                    index_tensor_2 = col * aTensor2.strides[dim_1 - 1] + shared * aTensor2.strides[dim_1 - 2] + indexBatch(batch, aTensor2.strides);

                    sum += aTensor1.getDataA()[index_tensor_1] * aTensor2.getDataA()[index_tensor_2];

                    current++;

                }   
                
                result_data.push_back( sum);
                

            }
        }
    } while (!compareVectorsAndIncr(batch, shape_1));

    result_location.setData(result_data);
}