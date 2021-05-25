@echo off

echo Building core...
gcc ./src/core.c -o ./build/core.o -Wall -g -I ./include -L ./lib -l raylib -l opengl32 -l gdi32 -l winmm -shared