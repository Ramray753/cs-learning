#include <stdio.h>

typedef struct {
    short i;
    float v;
    short j;
} s1;

typedef struct {
    float v;
    short i;
    short j;
} s2;

int main(void){
    printf("s1: %lu\n", sizeof(s1));
    printf("s2: %lu\n", sizeof(s2));
}

