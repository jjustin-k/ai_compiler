cmake_minimum_required(VERSION 3.16)
project(ai_compiler)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(dlc
    src/main.cpp
    src/ir/tensor.cpp
    src/ir/graph.cpp
    src/ir/graph_builder.cpp
    src/ops/add.cpp
    src/ops/div.cpp
    src/ops/mult.cpp
    src/ops/mat_mul.cpp
    src/ops/max_pooling.cpp
    src/ops/operation.cpp
    src/ops/sub.cpp
    src/codegen/codegen.cpp
    src/utils/algorithms.cpp
    src/utils/json_parser.cpp
)

include_directories("/opt/homebrew/include")

target_include_directories(dlc PRIVATE include)
