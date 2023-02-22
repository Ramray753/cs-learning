#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void){
    string a = "apple";
    printf("%d\n", strcmp(a, "apple"));
    printf("%d\n", strcmp(a, "appLe"));
    printf("%d\n", 1 > 2);
    printf("%d\n", 1 < 2);
}