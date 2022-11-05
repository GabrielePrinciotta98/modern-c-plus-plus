cmake_minimum_required(VERSION 3.14)

add_library(
    hw02
    combinatorics.h
    combinatorics.cpp
)

add_executable(
    runhw02
    run.cpp
)

target_include_directories(
    hw02 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    runhw02 PUBLIC
    hw02
)