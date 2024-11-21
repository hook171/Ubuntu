@echo off
git pull origin main
if not exist build mkdir build
cd build
cmake ..
cd ..
cmake --build build.
pause