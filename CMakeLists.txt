cmake_minimum_required(VERSION 3.16)
project(ai_compiler C CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Your normal C++ sources
set(CPP_SOURCES
    src/main.cpp
    src/ir/tensor.cpp
    src/ir/graph.cpp
    src/ir/graph_builder.cpp
    src/optimizer/optimizer.cpp
    src/optimizer/quantize.cpp
    src/codegen/codegen.cpp
    src/codegen/emitter.cpp
    src/codegen/matmul_emitter.cpp
    src/codegen/maxpool_emitter.cpp
    src/codegen/add_relu_emitter.cpp
    src/codegen/relu_emitter.cpp
    src/codegen/add_emitter.cpp
    src/codegen/conv_emitter.cpp
    src/codegen/fully_connected_emitter.cpp
    src/codegen/sub_emitter.cpp
    src/codegen/reshape_emitter.cpp
    src/utils/algorithms.cpp
    src/utils/json_parser.cpp
    src/utils/broadcaster.cpp
    src/utils/logger.cpp
)

set(C_MODEL output/compiled_model.c)

# Apply special compile flags for the C file
set_source_files_properties(${C_MODEL} PROPERTIES
    LANGUAGE C
    COMPILE_FLAGS "-O3 -march=native -flto -funroll-loops -ffast-math"
)

add_executable(dlc ${CPP_SOURCES} ${C_MODEL})

add_custom_target(format
    COMMAND clang-format -i ${CMAKE_SOURCE_DIR}/output/compiled_model.c
    COMMENT "Formatting generated code"
)

include_directories("/opt/homebrew/include")
target_include_directories(dlc PRIVATE include)
