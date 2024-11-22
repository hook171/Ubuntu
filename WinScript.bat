@echo off
git pull origin main
if not exist build_windows mkdir build_windows
cd build_windows
cmake ..
cmake --build .
cd Debug
Ubuntu.exe
pause 0