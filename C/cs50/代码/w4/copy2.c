#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
int main()
{
   char *str;
   str = (char *) malloc(3);
   strcpy(str, "abcdefg");
   printf("String = %s,  Address = %p\n", str, str);
   free(str);
}