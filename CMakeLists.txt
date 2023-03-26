cmake_minimum_required(VERSION 3.26.1)
project(physics_engine LANGUAGES CXX CUDA VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(physics_engine library.cpp)
