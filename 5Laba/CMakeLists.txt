cmake_minimum_required(VERSION 3.10)
project(5Laba)

set(CMAKE_CXX_STANDARD 17)

# Указываем пути к заголовочным файлам
include_directories(
    ${CMAKE_SOURCE_DIR}/Libs/cpp-httplib-master
    ${CMAKE_SOURCE_DIR}/Libs/SQLite
)

# Указываем путь к библиотеке SQLite
set(SQLITE_LIB_DIR "${CMAKE_SOURCE_DIR}/Libs/SQLite")
set(SQLITE_LIB "${SQLITE_LIB_DIR}/sqlite3.lib")

# Основной исполняемый файл
add_executable(5Laba
    5Laba.cpp
    db/db_handler.cpp
    http/server.cpp
)
add_executable(temperature_simulator
    temperature_simulator.cpp
)

# Подключаем библиотеку SQLite
target_link_libraries(5Laba PRIVATE ${SQLITE_LIB})