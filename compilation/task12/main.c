#include <stdio.h>
#include <windows.h>

int main() {
    HINSTANCE hDll = LoadLibrary(TEXT("external/lib/house.dll"));
    if (hDll == NULL) {
        fprintf(stderr, "Ошибка загрузки библиотеки\n");
        return 1;
    }

    void (*cat)() = (void(*)())GetProcAddress(hDll, "cat");
    void (*mouse)() = (void(*)())GetProcAddress(hDll, "mouse");
    void (*elephant)() = (void(*)())GetProcAddress(hDll, "elephant");
    void (*dog)() = (void(*)())GetProcAddress(hDll, "dog");
    void (*tiger)() = (void(*)())GetProcAddress(hDll, "tiger");
    void (*axolotl)() = (void(*)())GetProcAddress(hDll, "axolotl");

    cat();
    mouse();
    elephant();
    dog();
    tiger();
    axolotl();

    FreeLibrary(hDll);
    return 0;
}