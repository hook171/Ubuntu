@echo off
git pull origin main
if not exist build mkdir build
cd build
cmake ..
cmake --build .
cd Debug
Ubuntu.exe
pause 0