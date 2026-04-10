# group8_dsa

A command-line Patient Management System implemented in C using singly linked lists. This is a Data Structures and Algorithms university project.

## Prerequisites
To build and run this program, you need a C compiler (`gcc`) installed on your system.
- **Windows**: Install GCC via MSYS2 (MinGW-w64).
- **Mac**: Install Command Line Tools using `xcode-select --install` in the terminal.
- **Linux**: Install build-essential (e.g., `sudo apt install build-essential`).

## How to Build and Run

We have provided multiple ways to compile and run the program depending on your operating system.

### Option 1: Windows (Using Batch Script)
If you are on Windows and don't have `make` installed, you can use the provided batch script.

1. Open your terminal (Command Prompt or PowerShell) in the project directory.
2. Run the build script:
   ```cmd
   .\build.bat
   ```
3. If the build is successful, start the program:
   ```cmd
   .\app.exe
   ```

### Option 2: Mac and Linux (Using Bash Script)
If you are on Mac or Linux, you can use the provided shell script.

1. Open your terminal in the project directory.
2. Make the script executable (only needed the first time):
   ```bash
   chmod +x build.sh
   ```
3. Run the build script:
   ```bash
   ./build.sh
   ```
4. If the build is successful, start the program:
   ```bash
   ./app
   ```

### Option 3: Using Make (Cross-Platform)
If you have `make` installed on your system, you can use the `Makefile` to quickly build and run the application.

1. Compile the program: `make all`
2. Run the program: `make run`
3. Clean up compiled files: `make clean`