cmake_minimum_required(VERSION 3.10)
project(3Laba)

set(CMAKE_CXX_STANDARD 14)

# Добавляем исполняемый файл
add_executable(3Laba 3Laba.cpp)

# Для Windows нужно добавить библиотеки
if (WIN32)
    target_link_libraries(3Laba kernel32)
else()
    # Для Linux добавляем библиотеки, если они нужны
    target_link_libraries(3Laba pthread)
endif()

# Добавляем поддержку многопоточности
find_package(Threads REQUIRED)
target_link_libraries(3Laba Threads::Threads)