cmake_minimum_required(VERSION 2.8)

project(test_bitvector)

include_directories(
    include
)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
)