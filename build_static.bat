@echo off
setlocal

rem Default parameters
set "PLATFORM=%1"
set "CONFIG=%2"

if "%PLATFORM%"=="" set "PLATFORM=x64"
if "%CONFIG%"=="" set "CONFIG=Release"

rem Map platform to cl.exe target options and output folder
set "ARCH_FLAGS="
set "PLATFORM_FOLDER="

if /I "%PLATFORM%"=="x86" (
    set "ARCH_FLAGS=/arch:IA32"
    set "PLATFORM_FOLDER=x86"
) else if /I "%PLATFORM%"=="x64" (
    set "ARCH_FLAGS=/arch:AVX" 
    set "PLATFORM_FOLDER=x64"
) else if /I "%PLATFORM%"=="arm32" (
    set "ARCH_FLAGS=/arch:ARM"
    set "PLATFORM_FOLDER=arm32"
) else if /I "%PLATFORM%"=="arm64" (
    rem For ARM64, no specific /arch flag needed; set folder only
    set "PLATFORM_FOLDER=arm64"
) else (
    echo Unsupported platform "%PLATFORM%".
    exit /b 1
)

rem Set flags based on config
if /I "%CONFIG%"=="Debug" (
    set "CONFIG_FLAGS=/Zi /MDd /Od"
) else if /I "%CONFIG%"=="Release" (
    set "CONFIG_FLAGS=/O2 /MD"
) else (
    echo Unsupported configuration "%CONFIG%".
    exit /b 1
)

rem Build output directory
set "BUILD_DIR=build\%PLATFORM_FOLDER%\%CONFIG%"
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

rem Call VS dev command
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=%PLATFORM%

rem Compile
cl /c /nologo /W3 %CONFIG_FLAGS% %ARCH_FLAGS% static_lib.c /Fo:"%BUILD_DIR%\static_lib.obj"
if errorlevel 1 (
    echo Compilation failed.
    exit /b 1
)

rem Create library
lib /nologo /OUT:"%BUILD_DIR%\ignis.lib" "%BUILD_DIR%\static_lib.obj"
if errorlevel 1 (
    echo Library creation failed.
    exit /b 1
)

echo Build successful: %BUILD_DIR%\ignis.lib created.
pause
