cmake_minimum_required(VERSION 3.10)
project(5Laba)

set(CMAKE_CXX_STANDARD 17)

# Указываем пути к заголовочным файлам
include_directories(
    ${CMAKE_SOURCE_DIR}/http
    ${CMAKE_SOURCE_DIR}/Libs/SQLite
)

# Основной исполняемый файл
add_executable(5Laba
    5Laba.cpp
    db/db_handler.cpp
    http/server.cpp
)

# Исполняемый файл для симулятора температуры
add_executable(temperature_simulator
    temperature_simulator.cpp
)

# Подключение библиотек в зависимости от ОС
if (WIN32)
    # Настройки для Windows
    set(SQLITE_LIB "${CMAKE_SOURCE_DIR}/Libs/SQLite/sqlite3.lib")
    target_link_libraries(5Laba PRIVATE ${SQLITE_LIB})
else()
    # Настройки для Linux
    find_package(SQLite3 REQUIRED) # Ищем системную библиотеку SQLite
    if (SQLite3_FOUND)
        target_link_libraries(5Laba PRIVATE SQLite::SQLite3)
    else()
        message(FATAL_ERROR "SQLite3 не найден! Установите библиотеку SQLite3.")
    endif()

    # Подключаем библиотеку pthread (если требуется)
    find_package(Threads REQUIRED)
    target_link_libraries(5Laba PRIVATE Threads::Threads)
endif()