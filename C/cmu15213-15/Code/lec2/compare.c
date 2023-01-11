#include <stdio.h>

int abs(int x){
    if (x < 0){
        return -x;
    } else {
        return x;
    }
}

int main(void){
    printf("%d\n", abs(-2147483648)); // overflow
    printf("%d\n", abs(-2147483647));
    printf("%ld\n", -(-2147483648));
    printf("%d\n", (int) 2147483648);
}