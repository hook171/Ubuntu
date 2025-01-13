cmake_minimum_required(VERSION 3.10)

project(Ubuntu)

set(CMAKE_CXX_STANDARD 11)

message(STATUS "Building Lab 1")

include_directories(1Laba)

add_executable(Ubuntu 1Laba.cpp)