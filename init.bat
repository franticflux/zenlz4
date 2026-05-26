@echo off
setlocal enabledelayedexpansion

:: Defaults
set BUILD_TYPE=Release
set COMPILER=msvc
set BUILD_TESTS=OFF

:: Parse arguments
:parse_args
if "%~1"=="" goto args_done

if "%~1"=="--debug" (
    set BUILD_TYPE=Debug
) else if "%~1"=="--release" (
    set BUILD_TYPE=Release
) else if "%~1"=="--clang" (
    set COMPILER=clang
) else if "%~1"=="--msvc" (
    set COMPILER=msvc
) else if "%~1"=="--with-tests" (
    set BUILD_TESTS=ON
) else (
    echo Unknown option: %~1
    exit /b 1
)

shift
goto parse_args
:args_done

:: Always build into "build"
set BUILD_DIR=build

:: Clean and recreate build dir but ask first!
if exist %BUILD_DIR% (
    echo Build directory exists: %BUILD_DIR%
    set /p CONFIRM="Delete and recreate? (Y/N): "
    if /i "%CONFIRM%"=="Y" (
        rmdir /s /q %BUILD_DIR%
        mkdir %BUILD_DIR%
    ) else (
        echo Skipping build directory cleanup.
    )
) else (
    mkdir %BUILD_DIR%
)

:: Compiler setup
if "%COMPILER%"=="clang" (
    set CC=clang
    set CXX=clang++
) else (
    set CC=cl
    set CXX=cl
)

:: Print configuration
echo ================================
echo Build type:   %BUILD_TYPE%
echo Compiler:     %COMPILER%
echo Build tests:  %BUILD_TESTS%
echo Build dir:    %BUILD_DIR%
echo ================================

:: Run CMake configuration
cmake -S . -B %BUILD_DIR% ^
  -G Ninja ^
  -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
  -DBUILD_TESTS=%BUILD_TESTS% ^
  -DCMAKE_C_COMPILER=%CC% ^
  -DCMAKE_CXX_COMPILER=%CXX% ^
  -DCMAKE_TOOLCHAIN_FILE="c:/users/mbutu/documents/projects/2025/vcpkg/scripts/buildsystems/vcpkg.cmake" ^
  -DVCPKG_TARGET_TRIPLET=x64-windows-static ^
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

endlocal

