cmake_minimum_required(VERSION 3.10)

# Определяем проект
project(Ubuntu)

# Указываем флаги для сборки
set(CMAKE_CXX_STANDARD 11)

# Добавляем путь к заголовочным файлам
include_directories(2Laba)

# Указываем файлы библиотеки
add_library(process_launcher STATIC 2Laba/process_launcher.cpp)

# Указываем файл для тестовой программы
add_executable(Ubuntu main.cpp)

# Линкуем библиотеку с тестовой программой
target_link_libraries(Ubuntu process_launcher)
