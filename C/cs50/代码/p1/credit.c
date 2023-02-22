#include <cs50.h>
#include <stdio.h>

int check_validation(long num);
int get_length(long num);
int get_header(long num, int l);
int get_number();

int main(void)
{
    long number = get_long("Number: ");
    int l = get_length(number);
    int header = get_header(number, l);
    if (header == 34 || header == 37){
        if (l == 15 && check_validation(number)){
            printf("AMEX\n");
            return 0;
        }
    } else if (header >= 51 && header <= 55){
        if (l == 16 && check_validation(number)){
            printf("MASTERCARD\n");
            return 0;
        }
    } else if (header >= 40 && header <= 49){
        if ((l == 16 || l == 13) && check_validation(number)){
            printf("VISA\n");
            return 0;
        }
    }
    printf("INVALID\n");
}

int get_length(long num){
    int count = 0;
    while(num){
        num /= 10;
        count++;
    }
    return count;
}

int get_header(long num, int l){
    while(l > 2){
        num /= 10;
        l--;
    }
    return num;
}

int check_validation(long num){
    int sum = 0;
    int order = 1;
    int mod;
    while(num){
        mod = num % 10;
        num /= 10;
        if (order % 2 == 1){
            sum += mod;
        } else {
            mod = mod * 2;
            if (mod < 10){
                sum += mod;
            } else {
                sum += (mod / 10 + mod % 10);
            }
        }
        order++;
    }
    if (sum % 10 == 0){
        return 1;
    }
    return 0;
}