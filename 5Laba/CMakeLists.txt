cmake_minimum_required(VERSION 3.10)

project(Ubuntu)

set(CMAKE_CXX_STANDARD 11)

message(STATUS "Building Lab 5")

add_executable(Ubuntu 5Laba.cpp)

# Указываем путь к заголовочным файлам
target_include_directories(Ubuntu PRIVATE ${CMAKE_SOURCE_DIR}/Libs/SQLite)
target_include_directories(Ubuntu PRIVATE ${CMAKE_SOURCE_DIR}/Libs/cpp-httplib-master)

    # Указываем путь к библиотекам SQLite
target_link_directories(Ubuntu PRIVATE ${CMAKE_SOURCE_DIR}/Libs/SQLite)

    # Линкуем с SQLite
target_link_libraries(Ubuntu sqlite3)

    # Копируем DLL в папку с результатом сборки (только для Windows)
    if(WIN32)
        add_custom_command(TARGET Ubuntu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/Libs/SQLite/sqlite3.dll
            $<TARGET_FILE_DIR:Ubuntu>)

    endif()
