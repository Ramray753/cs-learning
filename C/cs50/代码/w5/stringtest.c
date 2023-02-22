#include <stdio.h>
#include <string.h>
 
int main ()
{
    char str1[15];
    char str2[15];
    int ret;


    strcpy(str1, "abcdef");
    strcpy(str2, "abcdef");
    str1[7] = 'A';
    str2[7] = 'B';

    ret = strcmp(str1, str2);
    printf("%i\n", ret);
}