cmake_minimum_required(VERSION 3.12)
project(stl_copy_experiment)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER g++)

add_executable(stl_copy_experiment main.cpp include/SequentialAllocator.h include/FancyPointer.h include/config.h)
target_include_directories(stl_copy_experiment PRIVATE include)
