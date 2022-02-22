@echo off
mkdir out
nasm -f win32 ./main.asm -o ./out/main.obj
gcc -m32 ./out/main.obj -o ./out/main.exe
clear
"./out/main.exe"
echo Program finished with exit code %errorlevel%
pause
exit