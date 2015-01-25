project(yadif)
cmake_minimum_required(VERSION 2.6)
enable_testing()

add_definitions(-Wall -std=c++11 -pedantic)

include_directories(include)
include_directories(extern/Catch)

add_subdirectory(test)
add_subdirectory(example)
