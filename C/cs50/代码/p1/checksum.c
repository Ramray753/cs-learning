#include <cs50.h>
#include <stdio.h>

int check_validation(long num){
    int sum = 0;
    int order = 1;
    int mod;
    while(num){
        mod = num % 10;
        num /= 10;
        if (order % 2 == 1){
            sum += mod;
            printf("add: %d\n", mod);
        } else {
            mod = mod * 2;
            if (mod < 10){
                sum += mod;
                printf("add: %d\n", mod);
            } else {
                sum += (mod / 10 + mod % 10);
                printf("add: %d\n", (mod / 10 + mod % 10));
            }
        }
        order++;
    }
    
    return sum;
}

int main(void){
    printf("%d\n", check_validation(4003600000000014));
}