cmake_minimum_required(VERSION 3.16)
project(ai_compiler)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(dlc
    src/main.cpp
    src/ir/tensor.cpp
)


target_include_directories(dlc PRIVATE include)
