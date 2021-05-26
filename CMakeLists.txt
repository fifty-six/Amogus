set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")

cmake_minimum_required(VERSION 3.19)
project(amogus)

set(CMAKE_CXX_STANDARD 20)

add_executable(amogus amogus.cpp)

find_package(OpenCV REQUIRED)

target_link_libraries( amogus ${OpenCV_LIBS} -fopenmp -L/usr/local/opt/llvm/lib)

target_compile_options( amogus PRIVATE -fopenmp -Ofast -march=native -Wall -Wextra -fcolor-diagnostics)
