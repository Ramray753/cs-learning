#include <stdio.h>
#include <stdlib.h>

int main(void){
    int l1[5];
    int *l2 = calloc(sizeof(int), 5);
    l2[0] = 100;
    printf("l1: %p\n", l1);
    printf("l2: %p\n", l2);
    printf("l2[0]: %d\n", l2[0]);
    printf("sizeof(l1): %lu\n", sizeof(l1));
    printf("sizeof(l2): %lu\n", sizeof(l2));
    free(l2);
    printf("l2[0]: %d\n", l2[0]);
    printf("l2: %p\n", l2);
    printf("sizeof(l2): %lu\n", sizeof(l2));
}