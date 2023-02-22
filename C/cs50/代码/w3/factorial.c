#include <stdio.h>

int factorial(int n){
    if (n == 1){
        return n;
    }
    return n * factorial(n - 1);
}

int main(void){
    printf("%d\n", factorial(10));
}