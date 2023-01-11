#include <stdio.h>
#include <stdlib.h>

int main(void){
    char *p = malloc(10);
    printf("p: %p\n", p);
    free(p);
    char *q = malloc(10);
    printf("q: %p\n", q);
    free(q);
}