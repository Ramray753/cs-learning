#include <cs50.h>
#include <stdio.h>

int main(void){
    string a = "apple";
    int i = 0;
    while (a[i] != '\0'){
        printf("%c ", a[i]);
        i++;
    }
    printf("\n");

}