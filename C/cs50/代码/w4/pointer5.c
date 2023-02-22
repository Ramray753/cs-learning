#include <stdio.h>
#include <string.h>

int main(void){
    char *s = "Apple";
    for (int i = 0, l = strlen(s); i < l; i++){
        printf("%c\n", *(s + i));
    }
    printf("%s\n", s - 100);
}