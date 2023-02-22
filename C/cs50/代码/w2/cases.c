#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void){
    string a = "123appLE456";
    for(int i = 0, l = strlen(a); i < l; i++){
        if (a[i] >= 'a' && a[i] <= 'z'){
            printf("%c", a[i] - 32);
        } 
        else if (a[i] >= 'A' && a[i] <= 'Z'){
            printf("%c", a[i] + 32);
        } 
        else {
            printf("%c", a[i]);
        }
    }
    printf("\n");

}