#include <stdio.h>

int main(void){
    char s[] = "apple";
    char *s2 = s; // s本身就是指针
    // 报错 char s3[] = s; 
    printf("%p\n", s);
    printf("%p\n", &s);
    printf("%p\n", *&s);
    printf("%p\n", s2);
}