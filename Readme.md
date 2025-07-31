# LiteAI Compiler

LiteAI is a minimal AI compiler that takes a neural network described in JSON, builds a computation graph intermediate representation (IR), applies basic graph optimizations, lowers the graph to a loop-based IR, and generates C code for CPU inference.

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

## Compiler Pipeline

1. **Parse JSON**: User provides a neural network in JSON format (see `template/` or `models/` for examples).
2. **Build IR**: JSON is parsed into a computation graph IR.
3. **Optimize**: Graph optimization passes are applied (operator fusion, constant folding).
4. **Lower**: Graph is lowered to a loop-level IR.
5. **Codegen**: C code is generated for each operation and the main function.
6. **Compile & Run**: Output C code can be compiled and executed for inference.

## Limitations / In Progress

- Operator fusion and constant folding are partially implemented.
- Only basic ops are supported; more advanced ops (e.g., convolutions, activations) are in progress.
- No GPU codegen or runtime environment yet.
- Limited error handling and logging.
- No batch inference or flexible tensor shapes.
- No packaging or installation scripts.

## TODOs

- Calculate tensor sizes after maxpool in JSON parser
- Track function calls for parameter reduction
- Move graph-level optimizations to `Optimizer`
- Move code-level optimizations to `CodeGen`
- Remove hardcoded constants and C++ implementations in favor of symbolic codegen
- Add more tests and error handling

## Future Additions

- Runtime environment for loading and running compiled models
- GPU code generation
- More advanced optimizations (memory planning, quantization)
- Benchmarks and demo scripts
- Packaging (CMake, Docker, etc.)
- Expanded documentation and architecture diagrams

---

LiteAI demonstrates an end-to-end AI compiler pipeline for simple models, with a focus on IR design, graph optimizations, and C code generation. It is a work in progress and serves as a foundation for more advanced compiler features.