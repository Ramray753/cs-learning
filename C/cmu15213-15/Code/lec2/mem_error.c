#include <stdio.h>

typedef struct{
    int a[2];
    double b;
} struct_t;

double fun(int i){
    volatile struct_t s;
    s.b = 3.14;
    s.a[i] = 1073741824; /* Possibly out of bounds */
    return s.b;
}

int main(void){
    for (int i = 0; i < 7; i++){
        printf("fun(%i): %.10f\n", i, fun(i));
    }
}