cmake_minimum_required(VERSION 3.27)
project(quanta)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE FRONTEND_SOURCES ${SOURCE_DIR}/frontend/*.cpp ${SOURCE_DIR}/frontend/*.h)
add_library(frontend_lib OBJECT ${FRONTEND_SOURCES})

add_executable(quanta ${SOURCE_DIR}/main.cpp $<TARGET_OBJECTS:frontend_lib>)

add_executable(quanta_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp $<TARGET_OBJECTS:frontend_lib>)
target_link_libraries(quanta_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(quanta_tests)