#include <stdio.h>

int main(void){
    int Y;
    int M;
    int D;
    char s[256]; // Note that 256 is an arbitrary choice here
    printf("Input: ");
    scanf("%i-%i-%i Comment: %s", &Y, &M, &D, s);
    printf("Y: %i\n", Y);
    printf("M: %i\n", M);
    printf("D: %i\n", D);
    printf("Comment: %s\n", s);
}