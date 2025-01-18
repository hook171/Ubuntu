cmake_minimum_required(VERSION 3.10)

project(Ubuntu)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

message(STATUS "Building Lab 4")

include_directories(4Laba)

add_executable(4Laba 4Laba.cpp)

add_executable(temperature_simulator temperature_simulator.cpp)