@echo off

git pull origin main

set LABS=1Laba 2Laba

for  %%L in (%LABS%) do (

    if not exist ..\%%L\build_windows mkdir ..\%%L\build_windows
    cd ..\%%L\build_windows

    cmake ..

    cmake --build .

    cd ..\..\%%L

)

pause 0
