cmake_minimum_required(VERSION 3.26.1)
project(cuda-module LANGUAGES CXX CUDA VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} src/cuda_acceleration_calculation.cu)

target_include_directories(${PROJECT_NAME} PUBLIC include/)
# A CMake pattern to have headers that are not seen by the client of this library.
# So headers which not have to be visible for client-code should be placed in src/
target_include_directories(${PROJECT_NAME} PRIVATE src)

set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

# Add modules
add_subdirectory(lib/cuda-toolkit)

target_link_libraries(${PROJECT_NAME} cuda-toolkit physics-engine)
