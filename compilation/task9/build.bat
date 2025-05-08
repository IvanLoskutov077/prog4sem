@echo off

mkdir external\lib
mkdir external\include

copy ..\08create_static\house.lib external\lib\

copy ..\08create_static\alice.h external\include\
copy ..\08create_static\bob.h external\include\

cl main.c /Iexternal\include external\lib\house.lib

main.exe
