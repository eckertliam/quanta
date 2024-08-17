cmake_minimum_required(VERSION 3.27)
project(quanta)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(quanta src/main.cpp
        src/frontend/token.h
        src/frontend/span.h
        src/frontend/span.cpp
        src/frontend/token.cpp
        src/frontend/lexer.cpp
        src/frontend/lexer.h
        src/frontend/parser.cpp
        src/frontend/parser.h
        src/frontend/ast.h
)