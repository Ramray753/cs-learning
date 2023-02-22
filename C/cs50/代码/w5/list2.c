#include <stdio.h>
#include <stdlib.h>

int main(void){
    int *l1 = calloc(sizeof(int), 1000);
    l1[0] = 123;
    int *l2 = realloc(l1, sizeof(int) * 100000);
    
    printf("l1: %p\n", l1);
    printf("l2: %p\n", l2);
    printf("l2[0]: %d\n", l2[0]);

    // free(l1); 不需要
    free(l2);
}