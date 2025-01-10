cmake_minimum_required(VERSION 3.10)

project(Ubuntu)

set(CMAKE_CXX_STANDARD 11)

option(LAB2 "Build Lab 2" OFF)
option(LAB4 "Build Lab 4" OFF)
option(LAB5 "Build Lab 5" ON)

message(STATUS "LAB2 is set to ${LAB2}")
message(STATUS "LAB4 is set to ${LAB4}")
message(STATUS "LAB5 is set to ${LAB5}")

if(LAB2)
    message(STATUS "Building Lab 2")

    include_directories(2Laba)

    add_library(process_launcher STATIC 2Laba/process_launcher.cpp)

    add_executable(Ubuntu 2Laba/2Laba.cpp)

    target_link_libraries(Ubuntu process_launcher)
endif()

if(LAB4)
    # Скрыл ошибки с localtime
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    message(STATUS "Building Lab 4")

    include_directories(4Laba)

    add_executable(Ubuntu 4Laba/4Laba.cpp)
endif()

if(LAB5)

    message(STATUS "Building Lab 5")
    # Указываем путь к исходным файлам
    add_executable(Ubuntu 5Laba/5Laba.cpp)

    # Указываем путь к заголовочным файлам
    target_include_directories(Ubuntu PRIVATE ${CMAKE_SOURCE_DIR}/5Laba/SQLite)
    target_include_directories(Ubuntu PRIVATE ${CMAKE_SOURCE_DIR}/5Laba/cpp-httplib-master)

    # Указываем путь к библиотекам SQLite
    target_link_directories(Ubuntu PRIVATE ${CMAKE_SOURCE_DIR}/5Laba/SQLite)

    # Линкуем с SQLite
    target_link_libraries(Ubuntu sqlite3)

    # Копируем DLL в папку с результатом сборки (только для Windows)
    if(WIN32)
        add_custom_command(TARGET Ubuntu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/5Laba/SQLite/sqlite3.dll
            $<TARGET_FILE_DIR:Ubuntu>)

    endif()
endif()



