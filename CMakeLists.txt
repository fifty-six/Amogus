cmake_minimum_required(VERSION 3.16)
project(amogus)

set(CMAKE_CXX_STANDARD 17)

add_executable(amogus amogus.cpp)

find_package(OpenCV REQUIRED)

target_link_libraries(amogus ${OpenCV_LIBS})

target_compile_options(amogus PRIVATE -Ofast -march=native -Wall -Wextra)
