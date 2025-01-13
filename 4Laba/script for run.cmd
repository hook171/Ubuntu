@echo off
REM Create the compiled_code folder if it doesn't exist
if not exist "C:\Users\hook\Documents\GitHub\Ubuntu\4Laba\compiled_code" (
    mkdir "C:\Users\hook\Documents\GitHub\Ubuntu\4Laba\compiled_code"
)

REM Compile 4Laba.cpp
echo Compiling 4Laba.cpp...
g++ -o "C:\Users\hook\Documents\GitHub\Ubuntu\4Laba\compiled_code\4Laba.exe" 4Laba.cpp -lws2_32
if %errorlevel% equ 0 (
    echo 4Laba successfully compiled.
) else (
    echo Error compiling 4Laba.
    exit /b 1
)

REM Compile temperature_simulator.cpp
echo Compiling temperature_simulator.cpp...
g++ -o "C:\Users\hook\Documents\GitHub\Ubuntu\4Laba\compiled_code\temperature_simulator.exe" temperature_simulator.cpp -lws2_32
if %errorlevel% equ 0 (
    echo temperature_simulator successfully compiled.
) else (
    echo Error compiling temperature_simulator.
    exit /b 1
)

echo Compilation complete.

REM Run 4Laba.exe
start cmd /k "cd /d C:\Users\hook\Documents\GitHub\Ubuntu\4Laba\compiled_code && 4Laba.exe COM3"

REM Run temperature_simulator.exe
start cmd /k "cd /d C:\Users\hook\Documents\GitHub\Ubuntu\4Laba\compiled_code && temperature_simulator.exe COM4"

pause