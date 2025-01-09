cmake_minimum_required(VERSION 3.10)

# Устанавливаем проект
project(Ubuntu)

# Задаем стандарт C++
set(CMAKE_CXX_STANDARD 11)

# Добавляем настройки для разных лабораторных работ
option(LAB2 "Build Lab 2" OFF)
option(LAB4 "Build Lab 4" ON)

# Выводим информацию о выбранных опциях
message(STATUS "LAB2 is set to ${LAB2}")
message(STATUS "LAB4 is set to ${LAB4}")

# Если выбрана Лаба 2
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

# Если выбрана Лаба 4
if(LAB4)

    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    message(STATUS "Building Lab 4")

    include_directories(4Laba)
    
    add_executable(Ubuntu 4Laba/4Laba.cpp)  # Укажи свой файл для Лабы 4
endif()
