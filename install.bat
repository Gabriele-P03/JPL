@echo off
echo --------------------- Installing JPL ---------------------
echo.

set includes=(exception utils logger parser network sounds)
Rem (exception utils graphics logger math network parser physics sounds)
set builds=(utils logger parser network sounds)

Rem (utils graphics logger math network parser physics sounds)

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

echo.

echo Removing old jpl folder and creating new one into Include Path
rmdir /s /q %includePath% >null 2>&1
mkdir %includePath%

echo.
echo Coping all header files

for /d %%i in (*) do (
    for %%d in %includes% do (
        if %%i==%%d (
            echo.
            echo Creating %%i's headers folder
            mkdir %includePath%\%%i
            robocopy %cd%\%%i\src %includePath%\%%i /S >nul
        )
    )
)
for %%d in %builds% do (
    for /d %%i in (*) do (
        if %%i==%%d (
            echo Building %%i
            cd %%i
            echo Clearing build directory
            rmdir /s /q build
            mkdir build
            cd build
            cmake -S .. -B . -G "MinGW Makefiles" -DUSE_STACKTRACE_W_EXCEPTION_JPL=TRUE -DAUTO_LOG_EXCEPTION_JPL=TRUE >nul
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