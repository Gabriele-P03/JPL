@echo off
echo --------------------- Installing JPL ---------------------
echo.


setlocal enabledelayedexpansion

set mingwPath=C:\msys64\mingw64
set compilerPath=%mingwPath%\bin
set includePath=%mingwPath%\include\jpl
set libraryPath=%mingwPath%\lib
set makePath=%compilerPath%\mingw32-make.exe

echo Current Directory %cd%
echo MinGW Path: %mingwPath%
echo CompilerPath: %compilerPath%
echo Include Path: %includePath%
echo Library Path: %libraryPath%

mkdir %includePath%

echo.
echo Coping all header files

if "%~1"=="" (
    echo Compiling all modules
    set "includes=exception utils logger parser network sounds graphics"
    set "builds=utils logger parser network sounds graphics"
) else (
    set "includes="
    set "builds="
    for %%a in (%*) do (
        if exist "%%~a" (
            echo Reserved to %%a
            set "includes=!includes! %%a"
            set "cr=%%a"
            echo Checking for CMakeLists.txt
            if exist "!cr!\CMakeLists.txt" (
                echo Found
                set "builds=!builds! %%a"
            )
        ) else (
            echo Module %%a does not exists
        )
    )
    echo Checks completed for all given modules 
)

for /d %%i in ("*") do (
    for %%d in (%includes%) do (
        if %%i==%%d (
            echo.
            echo Removing old %%i headers folder
            rmdir /s /q %includePath%\%%i >null 2>&1
            echo Creating %%i's headers folder
            mkdir %includePath%\%%i
            robocopy %cd%\%%i\src %includePath%\%%i /S >nul
        )
    )
)
for %%d in (%builds%) do (
    for /d %%i in (*) do (
        if %%i==%%d (
            echo Building %%i
            cd %%i
            echo Clearing build directory
            rmdir /s /q build
            mkdir build
            cd build
            cmake -S .. -B . -G "MinGW Makefiles" >nul
            %makePath%
            
            echo Coping static library lib%%iJPL.a %libraryPath%\
            copy lib%%iJPL.a %libraryPath%\

            cd ..
            cd ..

            echo 
        )
    )
)

echo.
set /p DUMMY=Hit ENTER to exit...