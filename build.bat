@echo off
echo Compiling program...
gcc -Wall -Wextra -std=c11 patient_management.c crud_operations.c validation.c sort.c -o app.exe
if %errorlevel% equ 0 echo Build successful! Run it by typing .\app.exe