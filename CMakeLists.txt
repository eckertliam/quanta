cmake_minimum_required(VERSION 3.27)
project(quanta)

set(CMAKE_CXX_STANDARD 20)

add_executable(quanta src/main.cpp
        src/frontend/lexer.cpp
        src/frontend/lexer.h
        src/frontend/token.cpp
        src/frontend/token.h
        src/frontend/parser.cpp
        src/frontend/parser.h
        src/frontend/ast.h
        src/frontend/span.cpp
        src/frontend/span.h
)
