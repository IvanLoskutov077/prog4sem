@echo off

gcc -E hello.c -o hello.i

gcc -S hello.c -o hello.s

gcc -c hello.c -o hello.o

gcc hello.c -o hello.exe

echo Все этапы выполнены:
dir hello.i hello.s hello.o hello.exe