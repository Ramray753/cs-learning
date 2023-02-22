#include <stdio.h>
#include <string.h>

int main(void){
    int n[] = {1, 2, 3};
    for (int i = 0; i < 3; i++){
        printf("%i\n", *(n + i));
        printf("%p\n", n + i);
    }
}