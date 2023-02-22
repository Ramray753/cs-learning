#include <stdio.h>

void swap1(int a, int b);
void swap2(int *a, int *b);

int main(void){
    int a = 100;
    int b = 200;
    printf("a: %i, b: %i\n", a, b);
    swap1(a, b);
    printf("a: %i, b: %i\n", a, b);
    swap2(&a, &b);
    printf("a: %i, b: %i\n", a, b);
}

void swap1(int a, int b){
    int tmp = a;
    a = b;
    b = tmp;
}

void swap2(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}