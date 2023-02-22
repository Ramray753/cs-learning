#include <stdio.h>

int main(void){
    char s[256]; // Note that 256 is an arbitrary choice here
    printf("Input: ");
    scanf("%s", s);
    printf("%s\n", s);
}