#include <stdio.h>
#include <stdlib.h>

int global = 100;

int code(void){return 100;}

int main(void){
    int local = 100;
    int *heap_large = malloc(sizeof(int) * (1L << 27));
    int *heap_small = malloc(sizeof(int));
    printf("local var (stack): 0x%p\n", &local);
    printf("function printf (shared library): 0x%p\n", printf);
    printf("heap_large var (heap): 0x%p\n", heap_large);
    printf("- - - - - - - - - - - - - - - - - - -\n");
    printf("heap_small var (heap): 0x%p\n", heap_small);
    printf("global var (data): 0x%p\n", &global);
    printf("function code (text): 0x%p\n", code);
    printf("function main (text): 0x%p\n", main);
    free(heap_large);
    free(heap_small);
}