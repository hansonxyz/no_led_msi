@echo off
echo Building no_led_msi...

if not exist build mkdir build
cd build

cmake .. -G "Visual Studio 16 2019" -A x64
if %errorlevel% neq 0 (
    echo CMake configuration failed
    pause
    exit /b %errorlevel%
)

cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build failed
    pause
    exit /b %errorlevel%
)

echo Build completed successfully!
echo Executable is located at: build\Release\no_led_msi.exe
pause