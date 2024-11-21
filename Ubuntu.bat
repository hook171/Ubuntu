@echo off

git pull origin main

cmake -S . -B build
cmake --build build

:: Запуск программы
main.exe