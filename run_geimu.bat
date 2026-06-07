@echo off

chcp 65001 > nul

if exist "geimu.exe" (
    geimu.exe
) else (
    echo error! no such file as Geimu.exe
)

pause