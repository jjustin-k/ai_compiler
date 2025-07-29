# LiteAI Compiler

A lightweight AI compiler that lowers a computation graph IR to loop-based CPU code with basic optimizations.

LiteAI is a minimal AI compiler built from scratch. It parses a neural network defined in JSON, converts it into a computation graph IR, applies graph optimizations (e.g., constant folding, op fusion), lowers the graph to a loop-based intermediate representation, and generates executable C code for CPU inference.

## Features

- Custom computation graph IR
- Shape inference and dependency analysis
- Graph optimization passes (operator fusion)
- Lowering to loop-based IR
- C++ code generation for matrix and element-wise operations
- End-to-end inference on MNIST using a simple MLP

## Compiler Pipeline

1. User provides a neural network in JSON format (see the `template/` folder for examples)
2. JSON is parsed and converted into a computation graph IR
3. Graph optimization passes are applied
4. Graph is lowered to a loop-level intermediate representation
5. C code is generated
6. Code is compiled and executed

## In Progress
- Operator fusion


## TO-DO List
- Change every op in maxpool to constants
- refactor prints to logging with debug levels
- Actually calculate the size of the tensor after maxpool in json_parser
- Keep track of function calls -> if only one call, can remove more paramters and add constants
- Make the graph level optimizations with Optimizer
- Make the code level optimizations in CodeGen
- Change these to constants   
int matmul1_m = 4;
int matmul1_n = 4;
int matmul1_p = 4;
- Remove the c++ implementations of operations, instead just have symbolic refernces for cg

## Future additions
- Create a runtime environment to be able to load in already compiled models and run inference
- Generate code on the GPU


Your project already demonstrates a solid grasp of compiler construction, IR design, and code generation for AI workloads—skills that are highly valued by big companies. Here’s an honest assessment based on your Readme and folder structure:

Strengths
End-to-end pipeline: From JSON model parsing to C code generation and execution.
Custom IR and optimization passes: Shows understanding of compiler theory.
Basic graph and code optimizations: CSE, DCE, operator fusion.
Clear documentation and TODOs: Indicates planning and awareness of next steps.
Gaps Before “Impressive” or Resume-Ready
Polish & Robustness

Add more tests (unit, integration, and real model examples).
Improve error handling and logging (as you noted).
Refactor for code clarity and maintainability.
Feature Depth

Support more ops (convolutions, activations, etc.).
Add more advanced optimizations (e.g., memory planning, quantization).
Support for batch inference and more flexible tensor shapes.
Demonstration

Provide benchmarks (speed, memory) vs. hand-written code or other frameworks.
Include a demo notebook or script showing end-to-end usage.
Packaging

Make it easy to install and use (e.g., pip install, Dockerfile, or CMake).
Add a license and contribution guidelines.
Documentation

Expand docs: architecture diagrams, code examples, API docs.
Add a “Why LiteAI?” section—what makes your approach unique?
What Would Impress Big Companies?
A working demo: Compile a real model (e.g., MNIST MLP) and show performance.
Advanced optimizations: Operator fusion, memory reuse, or even simple parallelization.
Clean, modular code: Easy to extend and maintain.
Clear documentation: Shows you can communicate technical ideas.
Summary:
You’re on the right track! With a few more features, polish, and a strong demo, this project could be a standout portfolio piece for big companies and your resume. If you want specific suggestions or a roadmap, let me know!