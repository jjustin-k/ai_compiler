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
    src/optimizer/optimizer.cpp
    src/codegen/codegen.cpp
    src/utils/algorithms.cpp
    src/utils/json_parser.cpp

)

add_custom_target(format
    COMMAND clang-format -i ${CMAKE_SOURCE_DIR}/out.c
    COMMENT "Formatting generated code"
)

include_directories("/opt/homebrew/include")

target_include_directories(dlc PRIVATE include)
