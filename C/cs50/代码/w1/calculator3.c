// Addition with int

#include <cs50.h>
#include <stdio.h>

int main(void)
{ 
    // Prompt user for x
    float x = get_float("x: ");

    // Prompt user for y
    float y = get_float("y: ");

    // Perform addition
    printf("%.50f\n", x / y);
}
