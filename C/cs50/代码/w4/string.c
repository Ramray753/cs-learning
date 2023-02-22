#include <stdio.h>
 int main(void){
    char *s = "Apple";
    char c = s[0];
    char *p1 = &c;
    char *p2 = &s[0];
    printf("s:-----------\n");
    printf("%p\n", s);
    printf("p1:----------\n");
    printf("%p\n", p1);
    printf("%c\n", *p1);
    printf("%s\n", p1);
    printf("p2:----------\n");
    printf("%p\n", p2);
    printf("%c\n", *p2);
    printf("%s\n", p2);
 }