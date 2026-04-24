@echo off
setlocal
title BTK
cd /d "%~dp0"

echo [BTK] Starting...
where go >nul 2>nul
if errorlevel 1 (
    echo [BTK] go not found. Please install it.
    pause
    exit /b 1
)

go run .

if errorlevel 1 (
    echo [BTK] Exited with error code %errorlevel%.
    pause
)
endlocal
