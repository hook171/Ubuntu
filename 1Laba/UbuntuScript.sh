#!/bin/bash

# Обновление репозитория
git pull origin main

# Список лабораторных работ
LABS=("1Laba" "2Laba" "3Laba" "4Laba" "5Laba")

# Цикл по каждой лабораторной работе
for LAB in "${LABS[@]}"; do
    echo "Building $LAB..."

    # Создание папки build_ubuntu, если её нет
    if [ ! -d "$LAB/build_ubuntu" ]; then
        mkdir -p "$LAB/build_ubuntu"
    fi

    # Переход в папку build_ubuntu
    cd "$LAB/build_ubuntu"

    # Запуск CMake для конфигурации проекта
    cmake ..

    # Сборка проекта
    cmake --build .

    # Возврат в корневую папку проекта
    cd ../..
done