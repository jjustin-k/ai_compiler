# LiteAI Compiler

LiteAI is a minimalist AI compiler designed to convert simple neural network models described in JSON into optimized C code for CPU inference. It serves as a foundation to explore compiler design, graph optimizations, and code generation for ML workloads.

## What LiteAI Actually Does

- **Parses a neural network from JSON**: Reads a model definition (e.g., for MNIST MLP) and constructs a computation graph.
- **Builds a computation graph IR**: Each node represents an operation (Add, Sub, MatMul, ReLU, MaxPool, etc.), with shape inference and dependency tracking.
- **Applies basic graph optimizations**: Operator fusion and constant folding are partially implemented.
- **Lowers to loop-based IR**: Converts the computation graph into a sequence of loop-based operations suitable for C code generation.
- **Generates C code**: Emits C functions for each operation (e.g., matmul, relu, add, maxpool) and a main function that runs inference on test input.
- **Runs end-to-end on simple models**: Can parse, optimize, lower, and generate code for simple MLPs (e.g., MNIST), producing a compilable C file.
- **Logging and debugging**: Uses a custom logger for debug output.

## Current Features

- Custom computation graph IR with support for:
- Add, Sub, MatMul, ReLU, MaxPool, FullyConnected, Reshape, Conv2D, AddReLU
- Shape inference and dependency analysis
- Basic graph optimizations (operator fusion, constant folding in progress)
- Loop-based IR lowering
- C code generation for matrix and element-wise ops
- Simple test input generation for generated C code
- End-to-end pipeline for small models

## Current Benchmarks
- **Optimized Analytics Summary (MNIST-12)**
- Accuracy: 98.9
- Average time: 368.85140 mircoseconds
- Total Time: 368.8514 milliseconds
- Count: 10000
- Time standard deviation: 30.15194 units

- **Un-Optimized Analytic Summary (MNIST-12)**
- Accuracy: 98.9
- Average time: 377.06990 microseconds
- Total Time: 3770.699 milliseconds
- Count: 10000
- Time standard deviation: 138.07530 units

## Compiler Pipeline

1. **Parse JSON**: User provides a neural network in JSON format (`models/` for examples).
2. **Build IR**: JSON is parsed into a computation graph IR.
3. **Optimize**: Graph optimization passes are applied (operator fusion, constant folding).
4. **Lower**: Graph is lowered to a loop-level IR.
5. **Codegen**: C code is generated for each operation and the main function (`output/compiled_model.c` for example).
6. **Compile & Run**: Output C code can be compiled and executed for inference.

## Limitations / In Progress

- Operator fusion and constant folding are partially implemented.
- Only basic ops are supported; more advanced ops (e.g., convolutions, activations) are in progress.
- No GPU codegen or runtime environment yet.
- Limited error handling and logging.
- No batch inference or flexible tensor shapes.
- No packaging or installation scripts.

## Future Additions

- Runtime environment for loading and running compiled models
- GPU code generation
- More advanced optimizations (memory planning, quantization)
- Benchmarks and demo scripts
- Packaging (CMake, Docker, etc.)
- Expanded documentation and architecture diagrams

---

LiteAI demonstrates an end-to-end AI compiler pipeline for simple models, with a focus on IR design, graph optimizations, and C code generation. It is a work in progress and serves as a foundation for more advanced compiler features.