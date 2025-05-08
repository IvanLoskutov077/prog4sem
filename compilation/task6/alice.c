#include <stdio.h>
#include "alice.h"

static void hello()
{
    printf("Hello from alice:cat\n");
}

void cat()
{
    hello();
}