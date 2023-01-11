#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
  
// Driver code
int main()
{
  
    printf("Output is: %p\n", sbrk(0));
    void *a = malloc(3);
    printf("Output is: %p\n", sbrk(0));
    void *b = malloc(3);
    printf("Output is: %p\n", sbrk(0));
    free(a);
    free(b);
}