@echo off
call clean.bat
cls

sdasz80 -l -o -s -g smspte.S
sdasz80 -l -o -s -g font.S

rem --- 128 for normal ROM
rem --- 49152 for RAM base
sdcc -o smspte.ihx -mz80 --no-std-crt0 --code-loc 128 smspte.rel font.rel

makebin -p smspte.ihx smspte.sms

python ..\scripts\fix-checksum.py smspte.sms

pause
