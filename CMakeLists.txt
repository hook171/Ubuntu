cmake_minimum_required(VERSION 3.10)

# Устанавливаем проект
project(Ubuntu)

# Задаем стандарт C++
set(CMAKE_CXX_STANDARD 11)

# ставим ON там, какую лабу хотим запустить, на остальных OFF
option(LAB2 "Build Lab 2" ON)
option(LAB4 "Build Lab 4" OFF)

# в bash выводится информация
message(STATUS "LAB2 is set to ${LAB2}")
message(STATUS "LAB4 is set to ${LAB4}")

if(LAB2)
    message(STATUS "Building Lab 2")

    # Добавляем путь к заголовочным файлам
    include_directories(2Laba)

    # Указываем файлы библиотеки
    add_library(process_launcher STATIC 2Laba/process_launcher.cpp)

    # Указываем файл для тестовой программы
    add_executable(Ubuntu 2Laba/2Laba.cpp)

    # Линкуем библиотеку с тестовой программой
    target_link_libraries(Ubuntu process_launcher)

endif()

if(LAB4)

    #скрыл ошибки с localtime
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    message(STATUS "Building Lab 4")

    include_directories(4Laba)
    
    add_executable(Ubuntu 4Laba/4Laba.cpp)  # Укажи свой файл для Лабы 4
endif()
