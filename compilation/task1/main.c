#include "dynarray.h"

int main() {
    Dynarray a;
    init(&a, 0);
    push_back(&a, 10);
    push_back(&a, 20);
    print(&a);

    set(&a, 0, 50);
    set(&a, 1, 90);
    push_back(&a, 30);
    print(&a);
    
    destroy(&a);
    return 0;
}