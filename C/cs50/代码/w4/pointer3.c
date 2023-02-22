#include <stdio.h>
#include <string.h>

int main(void){
    char s[] = "apple";
    char s1[] = {'a', 'b', 'c', 'd'};
    char *s2 = s;
    char *s4 = s1;
    printf("%lu\n", sizeof(s));
    printf("%lu\n", sizeof(s1));
    printf("%lu\n", sizeof(s2));
    printf("%s\n", s);
    printf("%s\n", s1);
    printf("%s\n", s2);
    for (int i = 0; i < 9; i++){
        printf("%c", s4[i]);
    }
    printf("\n");
}