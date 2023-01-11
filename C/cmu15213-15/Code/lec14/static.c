#include <stdio.h>

void fun(void){
    int a = 10;
    static int b = 10; 
    b--;
    a--;
    printf("a: %i, %p\t b: %i, %p\n", a, &a, b, &b);  
}


int main(void)
{    
    for(int i = 0; i < 10; i++){
        int _[i * 100];
        fun();
    }
}