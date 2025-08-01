#pragma once
#include"./ir/graph.hpp"

class Quantizer{
    public:
        enum class DataType  {INT8, INT16, INT32};
        
        void quantize(Node* node, bool per_channel);
};