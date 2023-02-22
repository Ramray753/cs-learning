#include <stdio.h>
 int main(void){
    char *s1 = "Apple";
    char *s2 = "Apple";
    printf("%p\n", s1);
    printf("%p\n", s2);
    printf("%i\n", s1 == s2);
 }