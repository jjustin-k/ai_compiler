# LiteAI Compiler

A lightweight AI compiler that lowers a computation graph IR to loop-based CPU code with basic optimizations.

LiteAI is a minimal AI compiler built from scratch. It parses a neural network defined in JSON, converts it into a computation graph IR, applies graph optimizations (e.g., constant folding, op fusion), lowers the graph to a loop-based intermediate representation, and generates executable C++ code for CPU inference.

## Features

- Custom computation graph IR
- Shape inference and dependency analysis
- Graph optimization passes (CSE, dead code elimination, operator fusion)
- Lowering to loop-based IR
- C++ code generation for matrix and element-wise operations
- End-to-end inference on MNIST using a simple MLP

## Compiler Pipeline

1. User provides a neural network in JSON format (see the `template/` folder for examples)
2. JSON is parsed and converted into a computation graph IR
3. Graph optimization passes are applied
4. Graph is lowered to a loop-level intermediate representation
5. C++ code is generated
6. Code is compiled and executed

## TO-DO List
- Finish c codegen for fusion test
- Maybe add shape to node
- run example of test
- refactor prints to logging with debug levels
- Finish defining the variables in main for codegen
- Actually calculate the size of the tensor after maxpool in json_parser
- Refactor lines like this " else if(node->op_name == "matmul" && !set.count("matmul2d"))" to be nested
- Change general_size to just be a constant throughout program
- Fix pool layer