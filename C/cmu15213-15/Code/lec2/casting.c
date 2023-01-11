#include <stdio.h>

int main(void){
    char a = 127; // 0111 1111 -> 127/127
    char b = -1; // 1111 1111 -> -1/255
    char unsigned c = 255; // 1111 1111 -> -1/255
    char unsigned d = 127; // 0111 1111 -> 127/127
    printf("%i\n", (char unsigned) a);
    printf("%i\n", (char unsigned) b);
    printf("%i\n", (char) c);
    printf("%i\n", (char) d);
}