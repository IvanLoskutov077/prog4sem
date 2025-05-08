@echo off

mkdir build
cd build

g++ -std=c++20 -I..\include -I..\external\stb -c ..\src\image.cpp -o image.o

ar rcs libimage.a image.o

g++ -std=c++20 -I..\include -I..\external\stb ..\src\main.cpp -L. -limage -o image_app.exe

copy ..\zlatoust1910.jpg .

echo Сборка завершена.
image_app.exe
