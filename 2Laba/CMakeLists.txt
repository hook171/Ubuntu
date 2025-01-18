cmake_minimum_required(VERSION 3.10)

project(2Laba)

set(CMAKE_CXX_STANDARD 11)

message(STATUS "Building Lab 2")

include_directories(2Laba)

add_library(process_launcher STATIC process_launcher.cpp)

add_executable(2Laba 2Laba.cpp)

target_link_libraries(2Laba process_launcher)