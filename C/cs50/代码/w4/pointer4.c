#include <stdio.h>
#include <string.h>

int main(void){
    printf("string:\n");
    char *s = "Apple";
    for (int i = 0, l = strlen(s); i < l; i++){
        printf("%p\n", &s[i]);
    }
    printf("int:\n");
    int *n = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++){
        printf("%p\n", &n[i]);
    }
}