cmake_minimum_required(VERSION 3.27)
project(nebula)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(quanta src/main.cpp src/IR.cpp include/IR.h)