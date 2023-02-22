#include <stdio.h>

int main(void){
    int n1 = 100;
    int n2 = n1;
    int * p1 = &n1;
    int * p2 = p2;
    printf("%p\n", &n1);
    printf("%p\n", &n2);
    printf("%p\n", &p1);
    printf("%p\n", &p2);
}