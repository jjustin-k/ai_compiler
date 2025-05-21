#include"tensor.hpp"

class Operation{
    
    public:
        bool validOperation(Tensor &aTensor1, Tensor &aTensor2);
        virtual void operate(Tensor &aTensor1, Tensor &aTensor2, Tensor &result_location);
        virtual void operate(Tensor &aTensor); // inplace operation on a singular tensor
    
};