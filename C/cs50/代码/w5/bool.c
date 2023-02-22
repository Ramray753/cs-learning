#include <stdbool.h>
#include <stdio.h>

int main(void){
    char x[10];
    char *y;
    printf("%p\n", x);
    // printf("%p", y); 报错
    printf("%p\n", &y);
}
