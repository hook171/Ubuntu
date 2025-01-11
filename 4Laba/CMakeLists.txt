cmake_minimum_required(VERSION 3.10)

project(Ubuntu)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

message(STATUS "Building Lab 4")

include_directories(4Laba)

add_executable(Ubuntu 4Laba.cpp)