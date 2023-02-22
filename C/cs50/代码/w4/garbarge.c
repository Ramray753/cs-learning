#include <stdio.h>

int a;
int b[3];

int main(void){
    int c;
    int d[3];
    printf("a: %i\n", a);
    for (int i = 0; i < 3; i++){
        printf("b[%i]: %i\n", i, b[i]);
    }
    // 报错: printf("c: %i", c);
    for (int i = 0; i < 3; i++){
        printf("d[%i]: %i\n", i, d[i]);
    }
}