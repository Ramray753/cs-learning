#include <stdio.h>

int main(void)
{
    extern int x, y;
    printf("x + y = %d\n", x + y);
}

int x = 100, y = 200;