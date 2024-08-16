cmake_minimum_required(VERSION 3.27)
project(nebula)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(quanta src/main.cpp
        src/frontend/token.h
        src/frontend/token.cpp
)