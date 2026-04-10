#!/bin/bash
echo "Compiling program..."
gcc -Wall -Wextra -std=c11 patient_management.c crud_operations.c validation.c sort.c -o app

if [ $? -eq 0 ]; then
    echo "Build successful! Run it by typing ./app"
else
    echo "Build failed!"
    exit 1
fi