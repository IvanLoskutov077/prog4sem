@echo off

cl /c alice.c
cl /c bob.c

lib alice.obj bob.obj /OUT:house.lib

cl main.c house.lib

main.exe