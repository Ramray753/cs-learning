#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void){
    string a = "apple";
    for(int i = 0, l = strlen(a); i < l; i++){
        printf("%c ", a[i]);
    }
    printf("\n");

}