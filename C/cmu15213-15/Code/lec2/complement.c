#include <stdio.h>

int main(void){
    int a = 9999;
    int b = -10000;
    printf("%x\n", a & b);
    printf("%x\n", a | b);
    printf("%x\n", a ^ b);
}