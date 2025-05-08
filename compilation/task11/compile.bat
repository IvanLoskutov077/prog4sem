@echo off
mkdir external\lib
mkdir external\include
copy ..\10create_dynamic\house.dll external\lib\
copy ..\10create_dynamic\alice.h external\include\
copy ..\10create_dynamic\bob.h external\include\
gcc main.c -Iexternal\include -Lexternal\lib -lhouse -o program.exe
echo Программа скомпилирована
