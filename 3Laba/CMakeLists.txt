cmake_minimum_required(VERSION 3.10)
project(3Laba)

set(CMAKE_CXX_STANDARD 14)

# Добавляем исполняемый файл
add_executable(3Laba 3Laba.cpp)

# Для Windows нужно добавить библиотеки
if (WIN32)
    target_link_libraries(3Laba kernel32)
endif()