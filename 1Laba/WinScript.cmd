@echo off

REM Переход в корневую папку проекта (Ubuntu)
cd /d "C:\Users\ARTEM\Documents\GitHub\Ubuntu"

REM Обновление репозитория
git pull origin main

REM Список лабораторных работ
set LABS=1Laba 2Laba 4Laba 3Laba 5Laba

REM Цикл по каждой лабораторной работе
for %%L in (%LABS%) do (
    echo Building %%L...

    REM Создание папки build_windows, если её нет
    if not exist "%%L\build_windows" mkdir "%%L\build_windows"

    REM Переход в папку build_windows
    cd "%%L\build_windows"

    REM Запуск CMake для конфигурации проекта
    cmake ..

    REM Сборка проекта
    cmake --build .

    REM Возврат в корневую папку проекта
    cd ..\..
)