@echo off

gcc -c alice.c -DUPPERCASE -o alice.o

gcc -c bob.c -o bob.o

gcc -c main.c -o main.o

gcc alice.o bob.o main.o -o program.exe

program.exe