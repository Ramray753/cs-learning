#include <stdio.h>
#define A 2147483648 // 默认signed，所以2147483648超过了signed int的最大值，变成了signed long int
#define B 2147483647 // 默认signed，为signed int的最大值
#define C 2147483648u // 显式制定为unsigned
int main(void){
    const int a = 2147483648; // 默认是signed，当overflow会转成unsigned
    printf("A: %lu\n", sizeof(A));
    printf("B: %lu\n", sizeof(B));
    printf("C: %lu\n", sizeof(C));
    printf("a: %lu\n", sizeof(a));
}