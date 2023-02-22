#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    float amount = get_float("Dollar Amount: ");
    int pennies1 = amount * 100;
    printf("Pennies1: %i\n", pennies1);
    int pennies2 = round(amount * 100);
    printf("Pennies2: %i\n", pennies2);
}
