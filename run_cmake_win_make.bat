@echo off

rem Make sure NMake is available on the PATH
set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin

goto menu

:start
set choice=E

md Build
cd Build
cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-sdcc.cmake -G "Unix Makefiles" ..
make clean
make
cd ..
goto menu

:dataset
cd Build
cd z80-dataset
make
cd ..
cd ..
goto menu

:prompt
cmd
goto menu

:menu

echo.
echo (R) Rebuild
echo (D) Build dataset only
echo (C) Command prompt
echo (E) Exit

set /p choice=Option: 
if '%choice%'=='R' goto start
if '%choice%'=='r' goto start

if '%choice%'=='D' goto dataset
if '%choice%'=='d' goto dataset

if '%choice%'=='C' goto prompt
if '%choice%'=='c' goto prompt