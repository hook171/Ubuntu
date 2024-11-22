cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Ubuntu)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем путь к исходному файлу
add_executable(Ubuntu main.cpp)