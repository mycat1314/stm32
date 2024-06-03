cmake_minimum_required(VERSION 3.28)
project(cuda_01 CUDA)

set(CMAKE_CUDA_STANDARD 17)

add_executable(cuda_01 main.cu)

set_target_properties(cuda_01 PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
