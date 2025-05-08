@echo off

cl /c alice.c
cl /c bob.c

link /DLL alice.obj bob.obj /OUT:house.dll

cl main.c house.lib

main.exe